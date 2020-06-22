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
    
    typedef wfc::json::object<
      get_names,
      wfc::json::member_list<
        wfc::json::member<n_prefix, get_names, std::string, &get_names::prefix>,
        wfc::json::member<n_suffix, get_names, std::string, &get_names::suffix>,
        wfc::json::member<n_offset, get_names, size_t,      &get_names::offset>,
        wfc::json::member<n_limit,  get_names, size_t,      &get_names::limit>,
        wfc::json::member<n_sortby, get_names, sort_by,     &get_names::by, sort_by_json>,
        wfc::json::member<n_power,  get_names, bool,        &get_names::power>
      >
    > type;
    
    typedef type::target target;
    typedef type::member_list member_list;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct get_names_json
  {
    JSON_NAME(name)
    JSON_NAME(value)

    typedef wfc::json::object<
      get_names::name_ts,
      wfc::json::member_list<
        wfc::json::member<n_name, get_names::name_ts, std::string, &get_names::name_ts::first>,
        wfc::json::member<n_value,   get_names::name_ts, value_type,   &get_names::name_ts::second>
      >
    > name_ts_json;

    typedef wfc::json::array< std::vector<name_ts_json> > array_name_ts_json;

    
    /*JSON_NAME(scale)*/
    JSON_NAME(names_ts)
    
    typedef wfc::json::object<
      get_names,
      wfc::json::member_list<
        /*wfc::json::member<n_scale, get_names, time_type, &get_names::scale>,*/
        wfc::json::member<n_names_ts, get_names, std::vector<get_names::name_ts>, &get_names::names_ts, array_name_ts_json >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
}

}}
