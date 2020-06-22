#pragma once

#include <ag/deprecated/add_deprecated.hpp>
//#include <ag/api/depr/multi_add.hpp>
#include <ag/iag.hpp>

namespace wamba{ namespace btp{

struct iag_depr
  : public ::wfc::istatistics
{
  virtual ~iag_depr() {}
  
  virtual void add_deprecated( request::add_deprecated::ptr req, response::add_deprecated::handler cb) = 0;
  
};

}}
