#pragma once 
/*
#include <btp/stored_key.hpp>
#include <wjson/json.hpp>
#include <wjson/name.hpp>

namespace wamba{ namespace btp{

  
struct key_info_json
{
  struct info_pair_json
  {
    JSON_NAME(value)
    JSON_NAME(ts)

    typedef wjson::object<
      key_info::info_pair,
      wjson::member_list<
        wjson::member<n_value, key_info::info_pair, value_type, &key_info::info_pair::value>,
        wjson::member<n_ts, key_info::info_pair, time_type, &key_info::info_pair::ts>
      >
    > type;
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
  
  JSON_NAME(last_update)
  JSON_NAME(count)
  JSON_NAME(lossy)
  JSON_NAME(avg)
  JSON_NAME(min)
  JSON_NAME(perc50)
  JSON_NAME(perc80)
  JSON_NAME(perc95)
  JSON_NAME(perc99)
  JSON_NAME(perc100)
  JSON_NAME(max)
  

  
  typedef wjson::object<
    key_info,
    wjson::member_list<
      wjson::member<n_last_update, key_info, time_type, &key_info::last_update>,
      wjson::member<n_count,  key_info, key_info::info_pair, &key_info::count,   info_pair_json>,
      wjson::member<n_lossy,  key_info, key_info::info_pair, &key_info::lossy,   info_pair_json>,
      wjson::member<n_avg,    key_info, key_info::info_pair, &key_info::avg,     info_pair_json>,
      wjson::member<n_min,    key_info, key_info::info_pair, &key_info::min,     info_pair_json>,
      wjson::member<n_perc50, key_info, key_info::info_pair, &key_info::perc50,  info_pair_json>,
      wjson::member<n_perc80, key_info, key_info::info_pair, &key_info::perc80,  info_pair_json>,
      wjson::member<n_perc95, key_info, key_info::info_pair, &key_info::perc95,  info_pair_json>,
      wjson::member<n_perc99, key_info, key_info::info_pair, &key_info::perc99,  info_pair_json>,
      wjson::member<n_perc100,key_info, key_info::info_pair, &key_info::perc100, info_pair_json>,
      wjson::member<n_max,    key_info, key_info::info_pair, &key_info::max,     info_pair_json>
    >
  > type;
  typedef type::target target;
  typedef type::serializer serializer;
  typedef type::member_list member_list;
  
};


struct key_info_id_json
{
  JSON_NAME(id)

  typedef wjson::object<
    key_info_id,
    wjson::member_list<
      wjson::member<n_id, key_info_id, key_id_t, &key_info_id::id>,
      wjson::base<key_info_json>
    >
  > type;
  typedef type::target target;
  typedef type::serializer serializer;
  typedef type::member_list member_list;
};

}}
*/

