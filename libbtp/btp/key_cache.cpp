#include "key_cache.hpp"
#include "key_aggregator.hpp"
#include "logger.hpp"
#include <wrtstat/aggregator/aggregator.hpp>
#include <functional>
#include <deque>

namespace wamba{ namespace btp{

namespace
{
  typedef std::pair<std::string, aggregated_info> aggregated_info_named;
  typedef std::function< bool(const aggregated_info_named&, const aggregated_info_named&) > compare_fun_t;

  inline bool check_prefix(const std::string& value, const std::string& prefix);
  inline bool check_suffix(const std::string& value, const std::string& suffix);
  inline value_type get_by_value(sort_by by, const aggregated_info& ai);
  inline compare_fun_t create_compare( sort_by by);
}


key_cache::~key_cache()
{
}

key_cache::key_cache(const key_cache_options& opt)
  : _mutex_list(opt.mutex_hash)
  , _options(opt)
  , _resulution(static_cast<time_type>(opt.resolution) )
  , _key_counter()
{
}

key_id_t key_cache::create(const std::string& name, time_type now_ts)
{
  if ( auto pkey = this->get_or_create(name, now_ts) )
  {
    return pkey->get_id();
  }
  return bad_key;
}

size_t key_cache::size() const
{
  std::lock_guard<mutex_type> lk(_mutex);
  return _key_map.size();
}

key_id_t key_cache::get_id(const std::string& name) const
{
  std::lock_guard<mutex_type> lk(_mutex);
  auto itr = _key_map.find(name);
  if ( itr!=_key_map.end() )
    return itr->second->get_id();
  return bad_key;
}

key_id_t key_cache::add(
  const std::string& name,
  aggregated_data&& data,
  stored_key* up_key,
  std::vector<aggregated_info>* up_data,
  bool* is_last
)
{
  if ( auto pkey = this->get_or_create(name, data.ts) )
  {
    if ( pkey->add( std::move(data), up_data, is_last) )
    {
      if ( up_key!=nullptr && *is_last)
      {
        *up_key = pkey->get_key_info();
      }
      return pkey->get_id();
    }
  }
  return bad_key;
}

key_id_t key_cache::merge(const std::string& name, aggregated_list&& data, stored_key* up_key, std::vector<aggregated_info>* up_data)
{
  if ( data.empty() )
    return bad_key;

  if ( auto pkey = this->get_or_create(name, data.back().ts) )
  {
    if ( pkey->merge( std::move(data), up_data) )
    {
      if ( up_key!=nullptr )
      {
        *up_key = pkey->get_key_info();
      }
      return pkey->get_id();
    }
  }
  return bad_key;
}

bool key_cache::init(const stored_name& info)
{
  std::lock_guard<mutex_type> lk(_mutex);
  if ( _key_counter < info.id )
    _key_counter = info.id;
  auto pkey = std::make_shared<key_aggregator>( this->get_mutex_(info.id), info.id, info.last_update, _options);
  pkey->set_key_info(info);
  auto upd_time = info.last_update / _resulution;
  this->update_time_(info.name, upd_time!=0 ? upd_time : 1 ) ;
  return _key_map.insert( std::make_pair(info.name, pkey) ).second;
}

void key_cache::release(stored_list* stored)
{
  if ( stored == nullptr )
    return;

  std::lock_guard<mutex_type> lk(_mutex);
  for (auto& item : _key_map )
  {
    stored_info si;
    static_cast<stored_key&>(si.first) = item.second->get_key_info();
    si.first.name = item.first;
    item.second->aggregate_last_point_if(&si.second, true);
    stored->push_back( std::move(si) );
  }
}


selected_names_t key_cache::select(
  const std::string& prefix,
  const std::string& suffix,
  size_t offset,
  size_t limit
) const
{
  selected_names_t result;
  if ( limit == 0 )       result.reserve(16);
  else if (limit < 1024)  result.reserve(limit);
  else result.reserve(1024);

  std::lock_guard<mutex_type> lk(_mutex);

  auto lower = _key_map.lower_bound(prefix);
  for ( ; lower != _key_map.end(); ++lower )
  {
    if ( !check_prefix(lower->first, prefix) )
      break;

    if ( !check_suffix(lower->first, suffix) )
      continue;

    if ( offset!=0)
    {
      --offset;
      continue;
    }

    result.emplace_back(lower->first, 0);

    if ( limit!=0 && result.size() == limit )
      break;
  }

  return result;
}


selected_names_t key_cache::select(
  const std::string& prefix,
  const std::string& suffix,
  size_t offset,
  size_t limit,
  sort_by by,
  bool power
) const
{
  if ( by == sort_by::NOSORT )
    return this->select(prefix, suffix, offset, limit);

  selected_names_t result;

  std::vector<aggregated_info_named> selection;

  std::lock_guard<mutex_type> lk(_mutex);

  auto lower = _key_map.lower_bound(prefix);
  for ( ; lower != _key_map.end(); ++lower )
  {
    if ( !check_prefix(lower->first, prefix) )
      break;
    if ( check_suffix(lower->first, suffix) )
      selection.push_back( std::make_pair(lower->first, lower->second->get_aggregated_info(power) ) );
  }

  if ( selection.size() < offset )
    return result;

  std::sort(selection.begin(), selection.end(), create_compare(by) );
  auto beg = selection.begin() + offset;
  auto end = selection.end();
  if ( limit!=0 && selection.size() > (offset + limit) )
    end = beg + limit;

  result.reserve( std::distance(beg, end) );

  std::transform(
    beg,
    end,
    std::back_inserter(result),
    [by](aggregated_info_named& agn)
    {
      return name_value_t(std::move(agn.first), get_by_value(by, agn.second));
    }
  );
  return result;
}

selected_names_t key_cache::tree(
  const std::string& prefix,
  const std::string& sep,
  size_t depth,
  size_t offset,
  size_t limit,
  tree_node node,
  sort_by by,
  bool power
) const
{
  if ( depth == 0 )
    return selected_names_t();

  std::string head = prefix.empty() ? std::string() : prefix + sep;
  selected_names_t lst = this->select(head, "", 0, 0, node==tree_node::LEAF ? by : sort_by::NOSORT, power);

  if ( lst.empty() )
    return lst;

  selected_names_t res;
  res.reserve(lst.size() );

  for ( name_value_t& item : lst)
  {
    size_t pos = head.size();
    for ( size_t i =0; i < depth; ++i )
    {
      pos = item.first.find(sep, pos );
      if ( pos == std::string::npos )
        break;
    }

    if ( pos == std::string::npos )
    {
      if ( node != tree_node::BRANCH )
        res.emplace_back( item.first.substr(head.size()), item.second );
    }
    else if ( node != tree_node::LEAF )
    {
      pos = item.first.find(sep, pos );
      size_t size1 = pos != std::string::npos
              ? pos - head.size()
              : item.first.size() - head.size();
      res.emplace_back( item.first.substr(head.size(), size1), item.second );
    }
  }


  if ( node != tree_node::LEAF )
  {
    std::sort(res.begin(), res.end(), [](const name_value_t& left, const name_value_t& right )
      { return left.first < right.first;});
    auto itr = std::unique(res.begin(), res.end(), [](const name_value_t& left, const name_value_t& right )
      { return left.first == right.first;});
    res.erase(itr, res.end());
  }

  if ( offset==0 )
  {
    if ( limit!=0 && res.size() > limit)
      res.resize(limit);
    return res;
  }

  if ( res.size() < offset)
    return selected_names_t();

  auto beg = res.begin() + offset;
  auto end = beg;
  if ( res.size() < offset + limit )
    end = res.end();
  else
    end += limit;

  return selected_names_t(beg, end);
}


size_t key_cache::gc()
{
  std::lock_guard<mutex_type> lk(_mutex);

  if ( _options.gc_interval == 0 || _time_key_set.empty() )
    return 0;

  time_t death_time = time(0) - _options.TTL;
  BTPLOG_DEBUG( "GC death_time=" << death_time << " TTL=" << _options.TTL)
  auto beg = _time_key_set.begin();
  if ( beg->first > death_time ) return 0;
  auto end = _time_key_set.lower_bound( std::make_pair(death_time, std::string() ) );
  size_t count =0;
  for(;beg!=end; ++count)
  {
    BTPLOG_DEBUG( "GC erase name=" << beg->second << " ts=" << beg->first);
    _key_map.erase(beg->second);
    _key_time_map.erase( beg->second );
    _time_key_set.erase(beg++);
  }

  if ( _key_time_map.size() != _time_key_set.size() || _key_time_map.size() != _key_map.size())
  {
    BTPLOG_FATAL("Нарушена консистентность индексов при GC")
    return 0;
  }
  return count;
}

key_id_t key_cache::del(const std::string& name)
{
  std::lock_guard<mutex_type> lk(_mutex);
  key_id_t key_id = bad_key;

  auto itr2 = _key_map.find(name);
  if ( itr2 == _key_map.end() )
    return bad_key;
        
  key_id = itr2->second->get_id();
 _key_map.erase(itr2);
 
  if ( _options.gc_interval != 0 )
  {
    auto itr1 = _key_time_map.find(name);
    if ( itr1 != _key_time_map.end() )
    {
      _time_key_set.erase( std::make_pair(itr1->second, name) );
      _key_time_map.erase(itr1);

      if ( _key_time_map.size() != _time_key_set.size() || _key_time_map.size() != _key_map.size())
      {
        BTPLOG_FATAL("Нарушена консистентность индексов после удаления : " << name)
        return bad_key;
      }
    }
  }

  return key_id;
}


void key_cache::update_time_(const std::string& name, time_type now)
{
  if ( _options.gc_interval == 0 )
    return;

  if ( now == 0)
    now = time(0);
  auto itr = _key_time_map.find(name);
  if ( itr!= _key_time_map.end() )
  {
    if ( itr->second > now - _options.gc_interval)
      return;

    _time_key_set.erase( std::make_pair(itr->second, itr->first) );
    _key_time_map.erase( itr );
  }

  _key_time_map[name] = now;
  _time_key_set.insert( std::make_pair(now, name) );

}

key_cache::key_ptr key_cache::get_or_create(const std::string& name, time_type now_ts)
{
  std::lock_guard<mutex_type> lk(_mutex);
  auto itr = _key_map.find(name);
  if ( itr == _key_map.end() )
  {
    auto key_id = ++_key_counter;
    auto pkey = std::make_shared<key_aggregator>( this->get_mutex_(key_id), key_id, now_ts, _options);
    itr = _key_map.insert( std::make_pair(name, pkey) ).first;
  }
  this->update_time_(name);
  return itr->second;
}

key_cache::mutex_type& key_cache::get_mutex_(const key_id_t& k) const
{
  return _mutex_list[ k % _mutex_list.size() ];
}

namespace {

  template<typename I, typename T, T I::* value>
  inline bool compare_by(const aggregated_info_named& left, const aggregated_info_named& right);


  inline bool check_prefix(const std::string& value, const std::string& prefix)
  {
    if ( !prefix.empty() )
    {
      if ( value.size() < prefix.size() )
        return false;

      if ( 0!=value.compare(0, prefix.size(), prefix) )
        return false;
    }
    return true;
  }

  inline bool check_suffix(const std::string& value, const std::string& suffix)
  {
    if ( !suffix.empty() )
    {
      if ( value.size() < suffix.size() )
        return false;

      if ( 0!=value.compare(value.size() - suffix.size(), suffix.size(), suffix) )
        return false;
    }

    return true;
  }

  template<typename I, typename T, T I::* value>
  inline bool compare_by(const aggregated_info_named& left, const aggregated_info_named& right)
  {
    return left.second.*value > right.second.*value;
  }

  inline compare_fun_t create_compare( sort_by by)
  {
    using namespace std::placeholders;
    switch (by)
    {
      case sort_by::COUNT:
        return compare_by< wrtstat::reduced_info, wrtstat::size_type, &wrtstat::reduced_info::count>;
      case sort_by::LOSSY:
        return compare_by< wrtstat::reduced_info, wrtstat::size_type, &wrtstat::reduced_info::lossy>;
      case sort_by::MIN:
        return compare_by< wrtstat::reduced_info, wrtstat::value_type, &wrtstat::reduced_info::min>;
      case sort_by::PERC50:
        return compare_by< wrtstat::aggregated_perc, wrtstat::value_type, &wrtstat::aggregated_perc::perc50>;
      case sort_by::PERC80:
        return compare_by< wrtstat::aggregated_perc, wrtstat::value_type, &wrtstat::aggregated_perc::perc80>;
      case sort_by::PERC95:
        return compare_by< wrtstat::aggregated_perc, wrtstat::value_type, &wrtstat::aggregated_perc::perc95>;
      case sort_by::PERC99:
        return compare_by< wrtstat::aggregated_perc, wrtstat::value_type, &wrtstat::aggregated_perc::perc99>;
      case sort_by::PERC100:
        return compare_by< wrtstat::aggregated_perc, wrtstat::value_type, &wrtstat::aggregated_perc::perc100>;
      case sort_by::MAX:
        return compare_by< wrtstat::reduced_info, wrtstat::value_type, &wrtstat::reduced_info::max>;
      case sort_by::AVG:
        return compare_by< wrtstat::reduced_info, wrtstat::value_type, &wrtstat::reduced_info::avg>;
      case sort_by::NOSORT:
        return nullptr;
    }
    return nullptr;
  }

  inline value_type get_by_value(sort_by by, const aggregated_info& ai)
  {
    switch ( by )
    {
      case sort_by::NOSORT:  return 0;
      case sort_by::AVG:     return ai.avg;
      case sort_by::COUNT:   return ai.count;
      case sort_by::PERC50:  return ai.perc50;
      case sort_by::PERC80:  return ai.perc80;
      case sort_by::PERC95:  return ai.perc95;
      case sort_by::PERC99:  return ai.perc99;
      case sort_by::PERC100: return ai.perc100;
      case sort_by::MIN:     return ai.min;
      case sort_by::MAX:     return ai.max;
      case sort_by::LOSSY:   return ai.lossy;
      default:
        return 0;
    };
    return 0;
  }

}

}}
