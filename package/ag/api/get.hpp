#pragma once
#include <btp/types.hpp>
#include <functional>
#include <string>
#include <memory>


namespace wamba{ namespace btp{

namespace request
{
  struct get
  {
    std::string name;
    bool power = false;
    size_t offset = 0;
    size_t limit = 0;
    time_type ts = 0;
    typedef std::unique_ptr<get> ptr;
  };
}

namespace response
{
  struct get 
  {
    aggregated_list counters;
    typedef std::unique_ptr<get> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
