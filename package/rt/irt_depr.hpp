#pragma once

#include <rt/api/depr/add.hpp>
#include <rt/api/depr/multi_add.hpp>
#include <rt/irt.hpp>

namespace wamba{ namespace btp{

struct irt_depr
  : public irt
{
  virtual ~irt_depr() {}
  virtual void add( rtdepr::request::add::ptr req, rtdepr::response::add::handler cb) = 0;
  virtual void multi_add( rtdepr::request::multi_add::ptr req, rtdepr::response::multi_add::handler cb) = 0;
};

}}
