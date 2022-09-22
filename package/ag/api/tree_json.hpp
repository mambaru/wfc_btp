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
    
    typedef wjson::object<
      tree,
      wjson::member_list<
        wjson::member<n_prefix, tree, std::string, &tree::prefix>,
        wjson::member<n_sep,    tree, std::string, &tree::sep>,
        wjson::member<n_offset, tree, size_t, &tree::offset>,
        wjson::member<n_limit,  tree, size_t, &tree::limit>,
        wjson::member<n_depth,  tree, size_t, &tree::depth>,
        wjson::member<n_ntype,  tree, tree_node, &tree::node, tree_node_json>,
        wjson::member<n_sortby, tree, sort_by, &tree::by, sort_by_json>,
        wjson::member<n_by_power, tree, bool, &tree::by_power>
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
    typedef type1::member_list member_list;
  };
}

namespace response
{
  struct tree_json
  {
    typedef wjson::object_array<
      name_value_t,
      wjson::member_list<
        wjson::member_array< name_value_t, std::string, &name_value_t::first >,
        wjson::member_array< name_value_t, value_type,  &name_value_t::second >
      >
    > name_value_json;
    
    JSON_NAME(names)
    typedef wjson::object<
      tree,
      wjson::member_list<
        wjson::member<n_names, tree,  selected_names_t, &tree::names, wjson::vector_of<name_value_json, 64> >
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
    typedef type1::member_list member_list;
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
    
    typedef wjson::object<
      get_name_tree,
      wjson::member_list<
        wjson::member<n_prefix,   get_name_tree, std::string, &get_name_tree::prefix>,
        wjson::member<n_sep,      get_name_tree, std::string, &get_name_tree::sep>,
        wjson::member<n_offset,   get_name_tree, size_t, &get_name_tree::offset>,
        wjson::member<n_limit,    get_name_tree, size_t, &get_name_tree::limit>,
        wjson::member<n_depth,    get_name_tree, size_t, &get_name_tree::depth>,
        wjson::member<n_ntype,    get_name_tree, tree_node, &get_name_tree::node, tree_node_json>,
        wjson::member<n_sortby,   get_name_tree, sort_by, &get_name_tree::by, sort_by_json>,
        wjson::member<n_by_power, get_name_tree, bool, &get_name_tree::by_power>
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
    typedef type1::member_list member_list;
  };
}

namespace response
{
  struct get_name_tree_json
  {
    JSON_NAME(branches)
    typedef wjson::object<
      get_name_tree,
      wjson::member_list<
        wjson::member<n_branches, get_name_tree,  std::vector<std::string>, &get_name_tree::branches, wjson::vector_of_strings<> >
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
    typedef type1::member_list member_list;
  };
}

}}
