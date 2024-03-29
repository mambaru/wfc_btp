#pragma once
#include <ag/api/merge.hpp>
#include <wfc/json.hpp>
#include <wrtstat/aggregator/api/json/aggregated_info_json_compact.hpp>

namespace wamba { namespace btp {

namespace request {
  
  struct merge_json
  {
    JSON_NAME(name)
    JSON_NAME(counters)

    typedef wjson::object<
      merge,
      wjson::member_list<
        wjson::member<n_name, merge, std::string, &merge::name>,
        wjson::member<n_counters, merge, aggregated_list, &merge::counters, 
          wjson::vector_of<wrtstat::aggregated_info_json_compact, 3000> >      >
    > meta;
    
    typedef meta::target target;
    typedef meta::serializer serializer;
  };
}

namespace response
{
  struct merge_json
  {
    JSON_NAME(status)
    
    typedef wjson::object<
      merge,
      wjson::member_list<
        wjson::member< n_status,  merge, bool, &merge::status >
      >
    > meta;
    typedef meta::target target;
    typedef meta::serializer serializer;
  };
}

}}
