#pragma once
#include <ag/api/merge.hpp>
#include <wfc/json.hpp>
#include <wfc/statistics/api/aggregated_json.hpp>

namespace wamba { namespace btp {

namespace request {
  
  struct merge_json
  {
    JSON_NAME(name)
    JSON_NAME(counters)

    typedef wfc::json::object<
      merge,
      wfc::json::member_list<
        wfc::json::member<n_name, merge, std::string, &merge::name>,
        wfc::json::member<n_counters, merge, aggregated_list, &merge::counters, 
          wfc::json::vector_of<wfc::statistics::aggregated_info_json_array, 3000> >      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct merge_json
  {
    JSON_NAME(status)
    
    typedef wfc::json::object<
      merge,
      wfc::json::member_list<
        wfc::json::member< n_status,  merge, bool, &merge::status >
      >
    > type;
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}
