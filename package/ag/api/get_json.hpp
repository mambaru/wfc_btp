#pragma once
#include <ag/api/get.hpp>
#include <wfc/json.hpp>
#include <wfc/statistics/api/aggregated_json.hpp>

namespace wamba { namespace btp {

namespace request {
  
  struct get_json
  {
    JSON_NAME(ts)
    JSON_NAME(power)
    JSON_NAME(name)
    JSON_NAME(offset)
    JSON_NAME(limit)
    
    typedef wfc::json::object<
      get,
      wfc::json::member_list<
        wfc::json::member<n_ts, get, time_type, &get::ts>,
        wfc::json::member<n_power, get, bool, &get::power>,
        wfc::json::member<n_name, get, std::string, &get::name>,
        wfc::json::member<n_offset, get, size_t, &get::offset>,
        wfc::json::member<n_limit, get, size_t, &get::limit>
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct get_json
  {
    JSON_NAME(counters)
    
    typedef wfc::json::object<
      get,
      wfc::json::member_list<
        wfc::json::member<n_counters, get, aggregated_list, &get::counters, 
          wfc::json::vector_of<wfc::statistics::aggregated_info_json_array, 3000> >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}
