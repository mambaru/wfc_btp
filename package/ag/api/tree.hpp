#pragma once

#include <btp/types.hpp>
#include <functional>
#include <string>
#include <memory>

namespace wamba { namespace btp {

namespace request
{
  struct tree
  {
    std::string prefix;
    std::string sep = "~~";
    size_t limit = 100;
    size_t offset = 0;
    size_t depth = 1;
    tree_node node = tree_node::ANY;
    sort_by by = sort_by::NOSORT;
    bool by_power = false;
    typedef std::unique_ptr<tree> ptr;
  };
}

namespace response
{
  struct tree
  {
    selected_names_t names;
    typedef std::unique_ptr<tree> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

  
// устарело
namespace request
{
  struct get_name_tree
  {
    std::string prefix;
    std::string sep = "~~";
    size_t limit = 100;
    size_t offset = 0;
    size_t depth = 1;
    tree_node node = tree_node::ANY;
    sort_by by = sort_by::NOSORT;
    bool by_power = false;
    typedef std::unique_ptr<get_name_tree> ptr;
  };
}

namespace response
{
  struct get_name_tree
  {
    std::vector<std::string> branches;
    typedef std::unique_ptr<get_name_tree> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
