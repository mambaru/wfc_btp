#pragma once

#include <rt/api/depr/types.hpp>
#include <rt/api/depr/add.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace wamba { namespace btp { namespace rtdepr {

namespace request
{
  struct multi_add
  {
    std::vector<add> data;
    typedef std::unique_ptr<multi_add> ptr;
  };
}

namespace response
{
  struct multi_add
  {
    // false - превышен лимит счетчиков (хотя бы для одного нового, существующие отрабатывают всегда)
    bool result = false;
    typedef std::unique_ptr<multi_add> ptr;
    typedef std::function<void(ptr)> handler;

  };
}

}}}
