//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2017
//
// Copyright: See COPYING file that comes with this distribution
//

#include "ag_domain.hpp"
#include <wrtstat/multi_packer/basic_packer.hpp>
#include <btp/storage.hpp>
#include <unistd.h>
#include <memory>
#include <sstream>
#include <algorithm>

namespace wamba{ namespace btp{


#include <wfc/logger.hpp>

#define LOG_NAME "BTP_AG"
#define BTP_AG_LOG(TYPE, X)    WFC_WRITE_LOG(LOG_NAME, TYPE, X)
#define BTP_AG_LOG_ERROR(X)    WFC_LOG_ERROR( LOG_NAME, X )
#define BTP_AG_LOG_WARNING(X)  WFC_LOG_WARNING( LOG_NAME, X )
#define BTP_AG_LOG_MESSAGE(X)  WFC_LOG_MESSAGE( LOG_NAME, X )
#define BTP_AG_LOG_FATAL(X)    WFC_LOG_FATAL( LOG_NAME, X )
#define BTP_AG_LOG_BEGIN(X)    WFC_LOG_BEGIN( LOG_NAME, X )
#define BTP_AG_LOG_END(X)      WFC_LOG_END( LOG_NAME, X )
#define BTP_AG_LOG_DEBUG(X)    WFC_LOG_DEBUG( LOG_NAME, X )
#define BTP_AG_LOG_TRACE(X)    WFC_LOG_TRACE( LOG_NAME, X )
#define BTP_AG_LOG_PROGRESS(X) WFC_LOG_PROGRESS( NAME, X )

namespace {

struct to_power
{
  aggregated_info operator()(aggregated_info& ai) const
  {
    ai.avg *= ai.avg;
    ai.min *= static_cast<value_type>(ai.count);
    ai.perc50 *= static_cast<value_type>(ai.count);
    ai.perc80 *= static_cast<value_type>(ai.count);
    ai.perc95 *= static_cast<value_type>(ai.count);
    ai.perc99 *= static_cast<value_type>(ai.count);
    ai.perc100 *= static_cast<value_type>(ai.count);
    ai.max *= static_cast<value_type>(ai.count);
    return ai;
  }
};

}


ag_domain::~ag_domain()
{
}

void ag_domain::start()
{
  auto opt = this->options();
  _storage = std::make_shared<storage>();
  std::string err;
  opt.trace = [](std::string message)
  {
    BTP_AG_LOG_MESSAGE(message)
  };
  if ( !_storage->open(opt, &err) )
  {
    BTP_AG_LOG_FATAL("Open btp storage error: " << err);
    _storage = nullptr;
    return;
  }

  this->get_workflow()->release_timer(_gc_timer);
  if ( opt.key_cache.gc_interval!=0 )
  {
    this->get_workflow()->create_timer( std::chrono::seconds(opt.key_cache.gc_interval), [this](){
      size_t del1 = this->_storage->gc();
      BTP_AG_LOG_MESSAGE( "Deleted outdated keys from cache: " << del1);
      return true;
    });
  }

}

void ag_domain::merge( request::merge::ptr req, response::merge::handler cb )
{
  if ( this->bad_request(req, cb) )
    return;

  if ( cb == nullptr )
  {
    _storage->merge( std::move(req->name), std::move(req->counters), nullptr);
  }
  else
  {
    auto res = std::make_unique<response::merge>();
    std::string err;
    res->status = _storage->merge( std::move(req->name), std::move(req->counters), &err);
    if ( !res->status )
    {
      BTP_AG_LOG_ERROR("ag_domain::merge: " << err)
    }
    cb(std::move(res));
  }
}

bool ag_domain::push_( wrtstat::request::push&& req)
{
  std::string err;
  aggregated_data& ag = static_cast<aggregated_data&>(req);
  if ( _storage->add( std::move(req.name), std::move(ag), &err) )
    return true;
  BTP_AG_LOG_ERROR("ag_domain::push: " << err)
  return false;
}

void ag_domain::push( wrtstat::request::push::ptr req, wrtstat::response::push::handler cb)
{
  BTP_AG_LOG_DEBUG("BEGIN: ag_domain::push: " << req->name)
  if ( this->bad_request(req, cb) )
    return;

  if ( auto res = this->create_response(cb) )
  {
    res->status = this->push_( std::move(*req) );
    cb( std::move(res) );
  }
  else
  {
    this->push_( std::move(*req) );
  }
  BTP_AG_LOG_DEBUG("END: ag_domain::push")
}

void ag_domain::multi_push( wrtstat::request::multi_push::ptr req, wrtstat::response::multi_push::handler cb)
{
  if ( this->bad_request(req, cb) )
    return;

  std::string err;
  if ( !wrtstat::basic_packer::recompact(req.get(), &err) )
  {
    if ( auto res = this->create_response(cb) )
    {
      res->status = false;
      res->error = err;
      this->send_response( std::move(res), cb);
    }
    DOMAIN_LOG_ERROR("ag_domain::multi_push recompact ERROR: " << err)
    return;
  }

  bool status = true;
  for (wrtstat::request::push& p: req->data)
    status&=this->push_(std::move(p) );

  if ( auto res = this->create_response(cb) )
  {
    res->status = status;
    cb(std::move(res) );
  }
}

void ag_domain::del( wrtstat::request::del::ptr req, wrtstat::response::del::handler cb)
{
  if ( this->bad_request(req, cb) )
    return;
  auto res = this->create_response(cb);
  for (const std::string& name: req->names )
  {
    std::string err;
    if ( !_storage->del(name, &err) )
    {
      BTP_AG_LOG_MESSAGE("ag_domain::del('" << name << "'):" << err)
    }
  }
  this->send_response(std::move(res), cb);
}

void ag_domain::get( request::get::ptr req, response::get::handler cb )
{
  if ( this->notify_ban(req, cb) )
    return;

  auto res = std::make_unique<response::get>();
  std::string err;
  if ( !_storage->get(req->name, &res->counters, &err, req->ts, req->offset, req->limit) )
  {
    BTP_AG_LOG_ERROR("ag_domain::get('" << req->name << "'):" << err)
  }
  if ( req->power )
    std::transform(res->counters.begin(), res->counters.end(), res->counters.begin(), to_power());

  cb( std::move(res) );
}

void ag_domain::multi_get( request::multi_get::ptr req, response::multi_get::handler cb )
{
  if ( this->notify_ban(req, cb) )
    return;
  auto res = this->create_response(cb);
  for (const std::string& name1 : req->names )
  {
    std::string err;
    aggregated_list counters;
    if ( _storage->get(name1, &counters, &err, req->ts, req->offset, req->limit) )
    {
      if ( req->power )
        std::transform(counters.begin(), counters.end(), counters.begin(), to_power());
      res->data.emplace_back(name1, std::move(counters));
    }
    else
    {
      BTP_AG_LOG_ERROR("ag_domain::multi_get [" << name1 << "]: " << err);
    }
  }
  cb( std::move(res) );
}

void ag_domain::get_name_tree( request::get_name_tree::ptr req, response::get_name_tree::handler cb )
{
  if ( this->notify_ban(req, cb) )
    return;
  auto res = this->create_response(cb);
  auto selected = _storage->tree(
    req->prefix,
    req->sep,
    req->depth,
    req->offset,
    req->limit,
    req->node,
    req->by,
    req->by_power
  );

  res->branches.reserve( selected.size() );
  for (auto& pair : selected)
   res->branches.push_back( std::move(pair.first) );

  //res->prefix = std::move( req->prefix );
  cb( std::move(res) );
}

void ag_domain::tree( request::tree::ptr req, response::tree::handler cb )
{
  if ( this->notify_ban(req, cb) )
    return;
  auto res = this->create_response(cb);
  res->names = _storage->tree(
    req->prefix,
    req->sep,
    req->depth,
    req->offset,
    req->limit,
    req->node,
    req->by,
    req->by_power
  );

  cb( std::move(res) );
}

void ag_domain::select( request::select::ptr req, response::select::handler cb )
{
  if ( this->notify_ban(req, cb) )
    return;
  auto res = this->create_response(cb);
  res->names = _storage->select(
    req->prefix,
    req->suffix,
    req->offset,
    req->limit,
    req->by,
    req->power
  );
  cb( std::move(res) );
}

void ag_domain::get_names( request::get_names::ptr req, response::get_names::handler cb )
{
  if ( this->notify_ban(req, cb) )
    return;

  auto res = this->create_response(cb);
  res->names_ts = _storage->select(
    req->prefix,
    req->suffix,
    req->offset,
    req->limit,
    req->by,
    req->power
  );
  cb( std::move(res) );
}

void ag_domain::perform_io(data_ptr d, io_id_t /*io_id*/, output_handler_t handler)
{
  if (handler==nullptr)
    return;
  if (d==nullptr)
    return handler(nullptr);

  std::stringstream ssout;
  std::stringstream ss( std::string( d->begin(), d->end() ), std::ios_base::in );
  std::string method;
  ss >> method;
  if ( !method.empty() )
  {
    if ( method == "tree" )
    {
      std::string prefix;
      ss >> prefix;
      std::string sortby;
      ss >> sortby;

      auto res = _storage->tree( prefix, "~~", 1, 0, 1000000, tree_node::ANY, sort_by::PERC80, false);
      for (const auto& n : res)
        ssout << "  " << n.first << std::endl;
      ssout << "Done " << res.size();
    }
  }
  else
  {
    ssout << "ERROR";
  }
  std::string res = ssout.str();
  handler( std::make_unique<data_type>(res.begin(), res.end() ) );
}

void ag_domain::compact( request::compact::ptr req, response::compact::handler cb )
{
  if ( this->bad_request(req, cb) )
    return;

  if ( cb==nullptr )
  {
    std::weak_ptr<storage> wstg = _storage;
    this->get_workflow()->post([wstg]()
      {
        if ( auto pstg = wstg.lock() )
        {
          BTP_AG_LOG_BEGIN("Compact by request from queue");
          std::string err;
          if ( !pstg->compact(&err) )
          {
            BTP_AG_LOG_ERROR("Compact by request error: " << err);
          }
          BTP_AG_LOG_END("Compact by request from queue");
        }
      },
      [](){
        BTP_AG_LOG_ERROR("'Compact' request drop from workflow")
      }
    );
  }
  else
  {
    BTP_AG_LOG_BEGIN("Compact by request");
    _storage->compact(nullptr);
    BTP_AG_LOG_END("Compact by request");
    cb( std::make_unique<response::compact>() );
  }

}


}}
