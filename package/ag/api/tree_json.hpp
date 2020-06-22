#pragma once
#include <ag/api/tree.hpp>
#include <ag/api/sort_by_json.hpp>
#include <ag/api/tree_node_json.hpp>
#include <wfc/json.hpp>

namespace wamba { namespace btp{

namespace request {

  struct tree_json
  {
    JSON_NAME(prefix)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(depth)
    JSON_NAME(sep)
    JSON_NAME(ntype)
    JSON_NAME(sortby)
    JSON_NAME(by_power)
    
    typedef wfc::json::object<
      tree,
      wfc::json::member_list<
        wfc::json::member<n_prefix, tree, std::string, &tree::prefix>,
        wfc::json::member<n_sep,    tree, std::string, &tree::sep>,
        wfc::json::member<n_offset, tree, size_t, &tree::offset>,
        wfc::json::member<n_limit,  tree, size_t, &tree::limit>,
        wfc::json::member<n_depth,  tree, size_t, &tree::depth>,
        wfc::json::member<n_ntype,  tree, tree_node, &tree::node, tree_node_json>,
        wfc::json::member<n_sortby, tree, sort_by, &tree::by, sort_by_json>,
        wfc::json::member<n_by_power, tree, bool, &tree::by_power>
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
}

namespace response
{
  struct tree_json
  {
    typedef wfc::json::object_array<
      name_value_t,
      wfc::json::member_list<
        wfc::json::member_array< name_value_t, std::string, &name_value_t::first >,
        wfc::json::member_array< name_value_t, value_type,  &name_value_t::second >
      >
    > name_value_json;
    
    JSON_NAME(names)
    typedef wfc::json::object<
      tree,
      wfc::json::member_list<
        wfc::json::member<n_names, tree,  selected_names_t, &tree::names, wfc::json::vector_of<name_value_json, 64> >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
}
  
namespace request {

  struct get_name_tree_json
  {
    JSON_NAME(prefix)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(depth)
    JSON_NAME(sep)
    JSON_NAME(ntype)
    JSON_NAME(sortby)
    JSON_NAME(by_power)
    
    typedef wfc::json::object<
      get_name_tree,
      wfc::json::member_list<
        wfc::json::member<n_prefix,   get_name_tree, std::string, &get_name_tree::prefix>,
        wfc::json::member<n_sep,      get_name_tree, std::string, &get_name_tree::sep>,
        wfc::json::member<n_offset,   get_name_tree, size_t, &get_name_tree::offset>,
        wfc::json::member<n_limit,    get_name_tree, size_t, &get_name_tree::limit>,
        wfc::json::member<n_depth,    get_name_tree, size_t, &get_name_tree::depth>,
        wfc::json::member<n_ntype,    get_name_tree, tree_node, &get_name_tree::node, tree_node_json>,
        wfc::json::member<n_sortby,   get_name_tree, sort_by, &get_name_tree::by, sort_by_json>,
        wfc::json::member<n_by_power, get_name_tree, bool, &get_name_tree::by_power>
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
}

namespace response
{
  struct get_name_tree_json
  {
    JSON_NAME(branches)
    typedef wfc::json::object<
      get_name_tree,
      wfc::json::member_list<
        wfc::json::member<n_branches, get_name_tree,  std::vector<std::string>, &get_name_tree::branches, wfc::json::vector_of_strings<> >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
    typedef type::member_list member_list;
  };
}

}}
