#pragma once
#include <ag/api/multi_get.hpp>
#include <wfc/json.hpp>



namespace wamba{ namespace btp{

namespace request {
  
  struct multi_get_json
  {
    JSON_NAME(ts)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(power)
    JSON_NAME(names)


    typedef wfc::json::object<
      multi_get,
      wfc::json::member_list<
        wfc::json::member<n_ts, multi_get, time_type, &multi_get::ts>,
        wfc::json::member<n_offset, multi_get, size_t, &multi_get::offset>,
        wfc::json::member<n_limit, multi_get, size_t, &multi_get::limit>,
        wfc::json::member<n_power, multi_get, bool, &multi_get::power>,
        wfc::json::member<n_names, multi_get, std::vector<std::string>, &multi_get::names, wfc::json::vector_of_strings<> >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct multi_get_json
  {
    JSON_NAME(name)
    JSON_NAME(counters)

    typedef wfc::json::object<
      data_get,
      wfc::json::member_list<
        wfc::json::member<n_name, data_get, std::string, &data_get::first>,
        wfc::json::member<n_counters, data_get, aggregated_list, &data_get::second, 
          wfc::json::vector_of<wfc::statistics::aggregated_info_json_array, 3000> >
      >
    > data_get_json;

    typedef wfc::json::vector_of<data_get_json, 16> data_get_list_json;

    //JSON_NAME(scale)
    JSON_NAME(data)
    

    typedef wfc::json::object<
      multi_get,
      wfc::json::member_list<
        //wfc::json::member<n_scale, multi_get, time_type, &multi_get::scale>,
        wfc::json::member<n_data,
          multi_get, std::vector<data_get>, &multi_get::data, data_get_list_json>
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}
