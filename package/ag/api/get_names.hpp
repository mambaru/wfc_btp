#pragma once

#include <btp/types.hpp>
#include <functional>
#include <string>
#include <memory>

namespace wamba{ namespace btp {

namespace request
{
  struct get_names
  {
    std::string prefix;
    std::string suffix;
    size_t limit = 0;
    size_t offset = 10;
    sort_by by = sort_by::NOSORT;
    bool power = false;
    typedef std::unique_ptr<get_names> ptr;
  };
}

namespace response
{
  struct get_names
  {
    typedef std::pair<std::string, time_type> name_ts;
    
    /*time_type scale;*/
    std::vector<name_ts> names_ts;
    
    typedef std::unique_ptr<get_names> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
