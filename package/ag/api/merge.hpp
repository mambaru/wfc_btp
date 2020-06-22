#pragma once
#include <btp/types.hpp>
#include <functional>
#include <string>
#include <memory>


namespace wamba{ namespace btp{

namespace request
{
  struct merge
  {
    std::string name;
    aggregated_list counters;
    typedef std::unique_ptr<merge> ptr;
  };
}

namespace response
{
  struct merge
  {
    bool status = true;
    typedef std::unique_ptr<merge> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
