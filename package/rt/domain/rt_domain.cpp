//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2017
//
// Copyright: See COPYING file that comes with this distribution
//

#include "rt_domain.hpp"
#include <unistd.h>

namespace wamba{ namespace btp{

void rt_domain::initialize()
{
  for (std::string stat_target: this->options().targets)
    _targets.push_back( this->get_target<istatistics>( stat_target ) );
}

void rt_domain::start()
{
  auto ag = std::make_shared<wrtstat::aggregator_hashmap>(this->options());
  // 4.7 не поддерживает std::atomic_exchange(&_aggregators, ag);
  _aggregators = ag;

  if (auto stat = this->get_statistics() )
  {
    _multi_add_stat = stat->create_time_meter("multi_add:rate");
    _add_stat = stat->create_time_meter("add:rate");
    _add_batch_cnt = stat->create_value_meter("add:batch");
    _values_stat = stat->create_value_meter("values:batch");
  }
}


void rt_domain::ag_handler_(const std::string& name1, aggregated_ptr ag)
{
  ::wfc::read_lock<mutex_type> lk(_mutex);
  if ( _targets.empty() )
    return;

  auto req = std::make_unique<wfc::statistics::request::push>();
  static_cast<wrtstat::aggregated_data&>(*req) = std::move(*ag);
  req->name = std::move(name1);

  for( size_t i = 1; i < _targets.size(); ++i) if ( auto t = _targets[i].lock() )
  {
    t->push( std::make_unique<wfc::statistics::request::push>(*req), nullptr );
  }
  if ( auto t = _targets[0].lock() )
  {
    t->push( std::move(req), nullptr );
  }

}

void rt_domain::push( wfc::statistics::request::push::ptr req, wfc::statistics::response::push::handler cb)
{
  if ( this->bad_request(req, cb) )
    return;
  using namespace std::placeholders;
  const std::string& name1 = req->name;
  bool status = _aggregators->push(name1, *req, std::bind(&rt_domain::ag_handler_, this, std::cref(name1), _1 ));

  if ( auto res = this->create_response(cb) )
  {
    res->status = status;
    cb( std::move(res) );
  }
}

void rt_domain::del( wfc::statistics::request::del::ptr req, wfc::statistics::response::del::handler cb)
{
  if ( this->bad_request(req, cb) )
    return;
  for( size_t i = 1; i < _targets.size(); ++i) if ( auto t = _targets[i].lock() )
  {
    t->del( std::make_unique<wfc::statistics::request::del>(*req), nullptr );
  }
  if ( auto t = _targets[0].lock() )
  {
    t->del( std::move(req), nullptr );
  }
}


void rt_domain::add( rtdepr::request::add::ptr req, rtdepr::response::add::handler cb)
{
  if ( this->bad_request(req, cb) )
    return;

  wrtstat::aggregated_data ag;
  ag.ts = req->ts;

  if ( req->count < 0 )
    ag.count = req->cl.size();
  else
    ag.count = static_cast<size_t>(req->count);

  ag.data = std::move(req->cl);

  using namespace std::placeholders;
  std::string& name1 = req->name;
  bool status = _aggregators->push(req->name, ag, std::bind(&rt_domain::ag_handler_, this, std::ref(name1), _1 ));

  if ( auto res = this->create_response(cb) )
  {
    res->result = status;
    cb( std::move(res) );
  }
}

void rt_domain::multi_add( rtdepr::request::multi_add::ptr req, rtdepr::response::multi_add::handler cb)
{
  if ( this->bad_request(req, cb) )
    return;

  auto stat = this->get_statistics();

  wfc::time_point  multi_add_mtr;
  wfc::time_point  add_stat_mtr;
  wfc::value_point add_batch_mtr;
  wfc::value_point values_mtr;

  if ( stat!=nullptr )
  {
    multi_add_mtr = _multi_add_stat.create(1);
    add_batch_mtr = _add_batch_cnt.create(req->data.size(), req->data.size() );
  }

  size_t add_stat_count = 0;
  size_t add_values_stat = 0;

  wrtstat::aggregated_data ag;
  for ( auto& a : req->data )
  {
    ++add_stat_count;
    add_values_stat+=a.cl.size();
    /*if ( stat!=nullptr )
    {
      add_stat_mtr = stat->create_meter(_add_stat, 1);
      values_mtr = stat->create_meter(_values_stat, a.cl.size(), a.cl.size() );
    }*/
    ag.ts = a.ts;

    if ( a.count < 0 )
      ag.count = a.cl.size();
    else
      ag.count = static_cast<size_t>(a.count);

    ag.data = std::move(a.cl);

    using namespace std::placeholders;
    const std::string& name1 = a.name;

    _aggregators->push(name1, ag, std::bind(&rt_domain::ag_handler_, this, std::cref(name1), _1 ));
  }

  if ( stat!=nullptr )
  {
    add_stat_mtr = _add_stat.create(add_stat_count);
    values_mtr = _values_stat.create(add_values_stat, add_values_stat );
  }


  if ( auto res = this->create_response(cb) )
  {
    res->result = true;
    cb( std::move(res) );
  }
}


}}
