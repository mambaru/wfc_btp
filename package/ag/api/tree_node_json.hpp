#pragma once

#include <btp/types.hpp>
#include <wfc/json.hpp>

namespace wamba { namespace btp {

struct tree_node_json
{
  JSON_NAME2(n_empty, "")
  JSON_NAME(leaf)
  JSON_NAME(branch)
  JSON_NAME(all)

  typedef wfc::json::enumerator<
    tree_node, 
    wfc::json::member_list<
      wfc::json::enum_value<n_empty,  tree_node, tree_node::ANY>,
      wfc::json::enum_value<n_leaf,   tree_node, tree_node::LEAF>,
      wfc::json::enum_value<n_branch, tree_node, tree_node::BRANCH>,
      wfc::json::enum_value<n_all,    tree_node, tree_node::ANY>
    >
  > type;

  typedef type::target target;
  typedef type::member_list member_list;
  typedef type::serializer serializer;
};

}}
