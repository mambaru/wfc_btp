#pragma once

#include <btp/types.hpp>
#include <functional>
#include <string>
#include <memory>

namespace wamba{ namespace btp {

namespace request
{
  struct select
  {
    std::string prefix;
    std::string suffix;
    size_t limit = 0;
    size_t offset = 10;
    sort_by by = sort_by::NOSORT;
    bool power = true;
    
    typedef std::unique_ptr<select> ptr;
  };
}

namespace response
{
  struct select
  {
    selected_names_t names;
    
    typedef std::unique_ptr<select> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
