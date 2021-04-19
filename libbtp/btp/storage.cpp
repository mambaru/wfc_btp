#include "storage.hpp"

#include <btp/key_cache.hpp>
#include <btp/key_storage.hpp>
#include <btp/data_storage.hpp>
#include <btp/stored_key.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <sstream>

#define BTP_LOG(fun, params)\
if (fun!=nullptr) { std::stringstream ss; ss << params; fun(ss.str()); }

namespace wamba{ namespace btp{

storage::~storage()
{
}

size_t storage::gc()
{

  size_t del = _key_cache->gc();
  return del;
}

bool storage::open(const storage_options& opt, std::string* err)
{
  _ttl = opt.data_db.TTL;
  _resolution = static_cast<time_type>(opt.key_cache.resolution);

  storage_options::trace_fun_t trace = opt.trace;
  _key_cache = std::make_shared<key_cache>(opt.key_cache);
  _key_storage = std::make_shared<key_storage>();
  BTP_LOG(trace, "Open key storage...");
  if ( !_key_storage->open(opt.key_db, err) )
    return false;

  _data_storage = std::make_shared<data_storage>();
  BTP_LOG(trace, "Open data storage...");
  if ( !_data_storage->open(opt.data_db, err) )
    return false;

  BTP_LOG(trace, "Restore key cache ...");
  size_t key_counter = 0;
  auto ks = _key_storage;
  auto kc = _key_cache;
  auto load_fun = [&key_counter, kc, ks, trace](std::vector<stored_name> vsn)
  {
    BTP_LOG(trace, "Load " << vsn.size() << " keys");
    key_counter += vsn.size();
    for ( const auto& sn : vsn)
    {
      BTP_LOG(trace, "Loaded key " << sn.name << " count=" << sn.count.value << " count.ts=" << sn.count.ts 
              << " last_update=" << sn.last_update);
      if ( !kc->init(sn) )
      {
        BTP_LOG(trace, "Stored error key '" << sn.name << "'");
      }
    }
    BTP_LOG(trace, "Current stored " << key_counter << "keys");
  };

  if ( !_key_storage->load(1000, load_fun, err) )
    return false;

  BTP_LOG(trace, "Clear cache name's...");
  size_t gc_count = _key_cache->gc();
  BTP_LOG(trace, "Clear cache name's:" << gc_count);
  BTP_LOG(trace, "BTP open succeeded!");
  return true;
}

bool storage::compact(std::string* err)
{
  if ( !_key_storage->compact(err))
    return false;
  if ( !_data_storage->compact(err))
    return false;
  return true;
}

/**
 * @param [in]  name
 * @param [in]  data
 * @param [out] err
 * @return
 *
 *
 */
bool storage::add(const std::string& name, aggregated_data&& data, std::string* err)
{
  // 1. cache
  stored_key up_key;
  std::vector<aggregated_info> up_data;
  key_id_t key_id = _key_cache->add(name, std::move(data), &up_key, &up_data);

  if ( key_id == bad_key )
  {
    if ( err != nullptr )
      *err = std::string("Bad Key");
    return false;
  }

  // 2. keys
  if ( !_key_storage->set( name, up_key, err) )
  {
    return false;
  }

  // 3. data
  for (const aggregated_info& ud : up_data)
  {
    if ( !_data_storage->set(key_id, ud, err) )
    {
      return false;
    }
  }

  return true;
}

bool storage::merge(const std::string& name, aggregated_list&& data, std::string* err)
{
  if ( data.empty() )
    return false;

  // 1. cache
  stored_key up_key;
  std::vector<aggregated_info> up_data;
  up_data.reserve(data.size());
  key_id_t key_id = _key_cache->merge(name, std::move(data), &up_key, &up_data);

  if ( key_id == bad_key )
  {
    if ( err != nullptr )
      *err = std::string("Bad Key");
    return false;
  }

  // 2. keys
  if ( !_key_storage->set( name, up_key, err) )
  {
    return false;
  }

  // 3. data
  for (const aggregated_info& ud : up_data)
  {
    if ( !_data_storage->set(key_id, ud, err) )
    {
      return false;
    }
  }

  return true;

}

bool storage::get(const std::string& name, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit)
{
  key_id_t key_id = _key_cache->get_id(name);
  if ( key_id == bad_key )
  {
    if ( err != nullptr )
      *err = std::string("Bad Key");
    return false;
  }

  if ( ts == 0 )
    ts = (time(nullptr) - _ttl) * _resolution;

  if ( !_data_storage->get( key_id, result, err, ts, offset, limit) )
    return false;

  /*
   * Больше не нужно, storage умеет работать с offset и limit
  if ( result!=nullptr && !result->empty() )
  {
    if ( result->begin()->ts < death )
    {
      aggregated_info ai;
      ai.ts = death;
      auto itr = std::upper_bound(result->begin(), result->end(), ai, [](const aggregated_info& f, const aggregated_info& s) { return f.ts < s.ts;} );
      if ( itr == result->end() )
        result->clear();
      else if ( itr != result->begin() )
      {
        std::move(itr, result->end(), result->begin() );
        result->resize( std::distance(itr, result->end()) );
      }
    }
  }*/
  return true;
}

selected_names_t storage::tree(
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
  return _key_cache->tree(prefix, sep, depth, offset, limit, node, by, power);
}


selected_names_t storage::select(
  const std::string& prefix,
  const std::string& suffix,
  size_t offset,
  size_t limit,
  sort_by by,
  bool power
) const
{
  return _key_cache->select(prefix, suffix, offset, limit, by, power);
}

}}
