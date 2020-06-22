#pragma once

#include <rt/api/depr/types.hpp>
#include <memory>
#include <functional>

namespace wamba { namespace btp{ namespace rtdepr {

namespace request
{
  struct add
  {
    microtime_t ts = 0;
    long count = -1;
    std::string name;
    counter_list cl;
    typedef std::unique_ptr<add> ptr;
  };
}

namespace response
{
  struct add 
  {
    // false - превышен лимит счетчиков (для нового, существующие отрабатывают всегда)
    bool result = false;
    typedef std::unique_ptr<add> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}}

