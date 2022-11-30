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

  typedef wjson::enumerator<
    tree_node, 
    wjson::member_list<
      wjson::enum_value<n_empty,  tree_node, tree_node::ANY>,
      wjson::enum_value<n_leaf,   tree_node, tree_node::LEAF>,
      wjson::enum_value<n_branch, tree_node, tree_node::BRANCH>,
      wjson::enum_value<n_all,    tree_node, tree_node::ANY>
    >
  > meta;

  typedef meta::target target;
  typedef meta::member_list member_list;
  typedef meta::serializer serializer;
};

}}
