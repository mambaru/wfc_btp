//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2017
//
// Copyright: See COPYING file that comes with this distribution
//

#pragma once

#include <wfc/domain_object.hpp>
#include <wfc/mutex.hpp>
#include <rt/domain/rt_config.hpp>
#include <rt/irt_depr.hpp>
#include <wrtstat/manager/aggregator_hashmap.hpp>

namespace wamba{ namespace btp{

class rt_domain
  : public ::wfc::domain_object< irt_depr, rt_config, wfc::defstat>
{
public:
  virtual void initialize() override;
  virtual void start() override;
  virtual void push( wfc::statistics::request::push::ptr req, wfc::statistics::response::push::handler cb) override final;
  virtual void del( wfc::statistics::request::del::ptr req, wfc::statistics::response::del::handler cb) override final;
  virtual void add( rtdepr::request::add::ptr req, rtdepr::response::add::handler cb) override final;
  virtual void multi_add( rtdepr::request::multi_add::ptr req, rtdepr::response::multi_add::handler cb) override final;
  
private:
  typedef std::unique_ptr<wrtstat::aggregated_data> aggregated_ptr;
  void ag_handler_(const std::string& name, aggregated_ptr ag);
  
private:
  typedef std::weak_ptr<wfc::istatistics> btp_wptr;
  typedef std::vector<btp_wptr> target_list;
  typedef ::wfc::rwlock<std::mutex> mutex_type;
  mutex_type _mutex;
  target_list _targets;
  std::shared_ptr<wrtstat::aggregator_hashmap> _aggregators;
  
private:
  wfc::time_meter  _multi_add_stat;
  wfc::time_meter  _add_stat;
  wfc::value_meter _add_batch_cnt;
  wfc::value_meter _values_stat;
  
  
  
};

}}
