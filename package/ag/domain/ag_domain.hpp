//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2017
//
// Copyright: See COPYING file that comes with this distribution
//

#pragma once

#include <wfc/domain_object.hpp>
#include <wfc/mutex.hpp>
#include <ag/domain/ag_config.hpp>
#include <ag/iag.hpp>


namespace wamba{ namespace btp{

class storage;  
class ag_domain
  : public ::wfc::domain_object< iag, ag_config, wfc::defstat>
{
public:
  virtual ~ag_domain();
  virtual void start() override;
  virtual void push( request::push::ptr req, response::push::handler cb) override;
  virtual void del( request::del::ptr req, response::del::handler cb) override;
  virtual void get( request::get::ptr req, response::get::handler cb ) override;
  virtual void tree( request::tree::ptr req, response::tree::handler cb ) override;
  virtual void get_name_tree( request::get_name_tree::ptr req, response::get_name_tree::handler cb ) override;
  virtual void select( request::select::ptr req, response::select::handler cb ) override;
  virtual void get_names( request::get_names::ptr req, response::get_names::handler cb ) override;
  virtual void multi_get( request::multi_get::ptr req, response::multi_get::handler cb ) override;
  virtual void merge( request::merge::ptr req, response::merge::handler cb ) override;
  virtual void compact( request::compact::ptr req, response::compact::handler cb ) override;
  virtual void perform_io(data_ptr d, io_id_t io_id, output_handler_t handler) override;
private:
  std::shared_ptr<storage> _storage;
  timer_id_t _gc_timer = -1;
};

}}
