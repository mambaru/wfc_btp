#pragma once

#include <btp/stored_key.hpp>
#include <btp/types.hpp>
#include <btp/key_cache_options.hpp>
#include <string>
#include <mutex>
#include <map>
#include <set>

namespace wamba{ namespace btp{

class key_aggregator;
class key_cache
{
public:

  virtual ~key_cache();

  explicit key_cache(const key_cache_options& opt);

  key_id_t create(const std::string& name, time_type now_ts);

  key_id_t get_id(const std::string& name) const;

  key_id_t add(const std::string& name, aggregated_data&& data, stored_key* up_key, std::vector<aggregated_info>* up_data, bool* is_last);

  key_id_t merge(const std::string& name, aggregated_list&& data, stored_key* up_key, std::vector<aggregated_info>* up_data);

  bool init(const stored_name& info);

  size_t size() const;

  selected_names_t select(
    const std::string& prefix,
    const std::string& suffix,
    size_t offset,
    size_t limit,
    sort_by by,
    bool power
  ) const;


  selected_names_t select(
    const std::string& prefix,
    const std::string& suffix,
    size_t offset,
    size_t limit
  ) const;


  selected_names_t tree(
    const std::string& prefix,
    const std::string& sep,
    size_t depth,
    size_t offset,
    size_t limit,
    tree_node node,
    sort_by by,
    bool power
  ) const;

  size_t gc();

  key_id_t del(const std::string& name);
private:

  typedef std::shared_ptr<key_aggregator> key_ptr;
  typedef std::mutex mutex_type;

  key_ptr get_or_create(const std::string& name, time_type now_ts);
  void update_time_(const std::string& name, time_type now = 0);
  mutex_type& get_mutex_(const key_id_t& k) const;
private:
  mutable std::vector<mutex_type> _mutex_list;
  key_cache_options _options;
  const time_type _resulution;
  key_id_t _key_counter;

  typedef std::map<std::string, key_ptr> key_map_t;
  key_map_t _key_map;
  std::multiset< std::pair<time_t, std::string> > _time_key_set;
  std::map<std::string, time_t> _key_time_map;
  mutable mutex_type _mutex;


};

}}

