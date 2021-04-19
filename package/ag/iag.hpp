#pragma once

#include <wfc/statistics/istatistics.hpp>
#include <ag/api/get.hpp>
#include <ag/api/merge.hpp>
#include <ag/api/compact.hpp>
#include <ag/api/tree.hpp>
#include <ag/api/select.hpp>
#include <ag/api/get_names.hpp>
#include <ag/api/multi_get.hpp>

namespace wamba{ namespace btp{

  /*
struct iag
  : public virtual wfc::iinterface
{
public:
  virtual ~iag() = default;
  virtual void get( request::get::ptr req, response::get::handler cb ) = 0;
  virtual void merge( request::merge::ptr req, response::merge::handler cb ) = 0;
  virtual void tree( request::tree::ptr req, response::tree::handler cb ) = 0;
  virtual void select( request::select::ptr req, response::select::handler cb ) = 0;
  virtual void get_names( request::get_names::ptr req, response::get_names::handler cb ) = 0;
  virtual void get_name_tree( request::get_name_tree::ptr req, response::get_name_tree::handler cb ) = 0;
  virtual void multi_get( request::multi_get::ptr req, response::multi_get::handler cb ) = 0;
  virtual void compact( request::compact::ptr req, response::compact::handler cb ) = 0;
};*/

struct ibtp
  : public wfc::istatistics
{
public:
  virtual ~ibtp() = default;

  virtual void get( request::get::ptr req, response::get::handler cb ) = 0;
  virtual void merge( request::merge::ptr req, response::merge::handler cb ) = 0;
  virtual void tree( request::tree::ptr req, response::tree::handler cb ) = 0;
  virtual void select( request::select::ptr req, response::select::handler cb ) = 0;
  virtual void get_names( request::get_names::ptr req, response::get_names::handler cb ) = 0;
  virtual void get_name_tree( request::get_name_tree::ptr req, response::get_name_tree::handler cb ) = 0;
  virtual void multi_get( request::multi_get::ptr req, response::multi_get::handler cb ) = 0;
  virtual void compact( request::compact::ptr req, response::compact::handler cb ) = 0;
};

}}
