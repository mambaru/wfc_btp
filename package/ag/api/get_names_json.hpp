#pragma once
#include <ag/api/sort_by_json.hpp>
#include <ag/api/get_names.hpp>
#include <wfc/json.hpp>


namespace wamba { namespace btp{

namespace request {
  
  struct get_names_json
  {
    JSON_NAME(prefix)
    JSON_NAME(suffix)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(sortby)
    JSON_NAME(power)
    
    typedef wjson::object<
      get_names,
      wjson::member_list<
        wjson::member<n_prefix, get_names, std::string, &get_names::prefix>,
        wjson::member<n_suffix, get_names, std::string, &get_names::suffix>,
        wjson::member<n_offset, get_names, size_t,      &get_names::offset>,
        wjson::member<n_limit,  get_names, size_t,      &get_names::limit>,
        wjson::member<n_sortby, get_names, sort_by,     &get_names::by, sort_by_json>,
        wjson::member<n_power,  get_names, bool,        &get_names::power>
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::member_list member_list;
    typedef type1::serializer serializer;
  };
}

namespace response
{
  struct get_names_json
  {
    JSON_NAME(name)
    JSON_NAME(value)

    typedef wjson::object<
      get_names::name_ts,
      wjson::member_list<
        wjson::member<n_name, get_names::name_ts, std::string, &get_names::name_ts::first>,
        wjson::member<n_value,   get_names::name_ts, value_type,   &get_names::name_ts::second>
      >
    > name_ts_json;

    typedef wjson::array< std::vector<name_ts_json> > array_name_ts_json;

    
    /*JSON_NAME(scale)*/
    JSON_NAME(names_ts)
    
    typedef wjson::object<
      get_names,
      wjson::member_list<
        /*wjson::member<n_scale, get_names, time_type, &get_names::scale>,*/
        wjson::member<n_names_ts, get_names, std::vector<get_names::name_ts>, &get_names::names_ts, array_name_ts_json >
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
    typedef type1::member_list member_list;
  };
}

}}
