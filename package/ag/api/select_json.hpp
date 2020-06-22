#pragma once
#include <ag/api/sort_by_json.hpp>
#include <ag/api/select.hpp>
#include <wfc/json.hpp>


namespace wamba { namespace btp{

namespace request {
  
  struct select_json
  {
    JSON_NAME(prefix)
    JSON_NAME(suffix)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(sortby)
    JSON_NAME(power)
    
    typedef wfc::json::object<
      select,
      wfc::json::member_list<
        wfc::json::member<n_prefix, select, std::string, &select::prefix>,
        wfc::json::member<n_suffix, select, std::string, &select::suffix>,
        wfc::json::member<n_offset, select, size_t,      &select::offset>,
        wfc::json::member<n_limit,  select, size_t,      &select::limit>,
        wfc::json::member<n_sortby, select, sort_by,     &select::by, sort_by_json>,
        wfc::json::member<n_power,  select, bool,        &select::power>
      >
    > type;
    
    typedef type::target target;
    typedef type::member_list member_list;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct select_json
  {
    JSON_NAME(names)
    
    typedef wfc::json::object_array<
      name_value_t,
      wfc::json::member_list<
        wfc::json::member_array< name_value_t, std::string, &name_value_t::first >,
        wfc::json::member_array< name_value_t, value_type, &name_value_t::second >
      >
    > name_value_json;
    
    typedef wfc::json::object<
      select,
      wfc::json::member_list<
        wfc::json::member<n_names, select, selected_names_t, &select::names, wfc::json::vector_of<name_value_json, 64> >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
}

}}
