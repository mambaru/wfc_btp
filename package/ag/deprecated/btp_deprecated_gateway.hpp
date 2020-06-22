#pragma once

#include <wfc/statistics/istatistics.hpp>
#include <wfc/jsonrpc.hpp>
#include <ag/deprecated/add_deprecated_json.hpp>

#include <ag/iag_depr.hpp>
#include "add_deprecated_json.hpp"


namespace wamba{ namespace btp{  namespace gateway{

using namespace ::wfc::statistics;

JSONRPC_TAG(add)

struct btp_deprecated_method_list: public ::wfc::jsonrpc::method_list
<
  ::wfc::jsonrpc::interface_<iag_depr>,
  ::wfc::jsonrpc::call_method< _add_, request::add_deprecated_json, response::add_deprecated_json>
>
{};

template<typename Base>
class btp_deprecated_interface
  : public Base
{
public:

  virtual void push(wfc::statistics::request::push::ptr req, wfc::statistics::response::push::handler cb ) override
  {
    if ( req==nullptr )
    {
      if ( cb!=nullptr )
        cb(nullptr);
      return;
    }

    auto depr_push = std::make_unique<request::add_deprecated>();
    depr_push->ts = req->ts;
    depr_push->name = std::move( req->name);
    depr_push->cl = std::move( req->data );
    depr_push->ag.avg     = req->avg;
    depr_push->ag.count   = static_cast<int64_t>(req->count);
    depr_push->ag.perc50  = static_cast<int64_t>(req->perc50);
    depr_push->ag.perc80  = static_cast<int64_t>(req->perc80);
    depr_push->ag.perc95  = static_cast<int64_t>(req->perc95);
    depr_push->ag.perc99  = static_cast<int64_t>(req->perc99);
    depr_push->ag.perc100 = static_cast<int64_t>(req->perc100);

    response::add_deprecated::handler depr_cb = nullptr;
    if ( cb!=nullptr )
    {
      depr_cb = [cb](response::add_deprecated::ptr res)
      {
        auto orig_res = std::make_unique<wfc::statistics::response::push>();
        if (res!=nullptr)
          orig_res->status = res->result;
        if (cb!=nullptr)
          cb( std::move(orig_res) );
      };
    }

    this->template call< _add_ >(
      std::move(depr_push),
      std::move(depr_cb),
      nullptr
    );
  }

  virtual void add_deprecated( request::add_deprecated::ptr req, response::add_deprecated::handler cb) override
  {
    COMMON_LOG_MESSAGE("btp_deprecated_interface::add_deprecated cb=" << (cb!=nullptr) << " req=" << (req!=nullptr) )
    this->template call< _add_ >(
      std::move(req),
      std::move(cb),
      nullptr
    );
  }

  virtual void del( wfc::statistics::request::del::ptr, wfc::statistics::response::del::handler cb) override
  {
    if ( cb != nullptr )
      cb( nullptr );
  }

};

}}}
