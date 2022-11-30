#pragma once
#include <ag/api/get.hpp>
#include <wjson/json.hpp>
#include <wrtstat/aggregator/api/json/aggregated_info_json_compact.hpp>

namespace wamba { namespace btp {

namespace request {
  
  struct get_json
  {
    JSON_NAME(ts)
    JSON_NAME(power)
    JSON_NAME(name)
    JSON_NAME(offset)
    JSON_NAME(limit)
    
    typedef wjson::object<
      get,
      wjson::member_list<
        wjson::member<n_ts, get, time_type, &get::ts>,
        wjson::member<n_power, get, bool, &get::power>,
        wjson::member<n_name, get, std::string, &get::name>,
        wjson::member<n_offset, get, size_t, &get::offset>,
        wjson::member<n_limit, get, size_t, &get::limit>
      >
    > meta;
    
    typedef meta::target target;
    typedef meta::serializer serializer;
  };
}

namespace response
{
  struct get_json
  {
    JSON_NAME(counters)
    
    typedef wjson::object<
      get,
      wjson::member_list<
        wjson::member<n_counters, get, aggregated_list, &get::counters, 
        wjson::vector_of<wrtstat::aggregated_info_json_compact, 3000> >
      >
    > meta;
    
    typedef meta::target target;
    typedef meta::serializer serializer;
  };
}

}}
