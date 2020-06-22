#pragma once

#include <wfc/statistics/istatistics.hpp>
#include <wfc/jsonrpc.hpp>
#include <wfc/statistics/api/push_json.hpp>
#include <ag/api/get_names_json.hpp>
#include <ag/api/merge_json.hpp>
#include <ag/api/select_json.hpp>
#include <ag/api/tree_json.hpp>
#include <ag/api/get_json.hpp>
#include <ag/api/compact_json.hpp>
#include <ag/iag_depr.hpp>


namespace wamba{ namespace btp{ namespace ag{  namespace gateway{ 

JSONRPC_TAG(merge)
JSONRPC_TAG(push)
JSONRPC_TAG(get_names)
JSONRPC_TAG(get)
JSONRPC_TAG(select)
JSONRPC_TAG(compact)
JSONRPC_TAG(tree)

struct method_list: public ::wfc::jsonrpc::method_list
<
  wfc::jsonrpc::interface_<iag>,
  wfc::jsonrpc::call_method< _push_, wfc::statistics::request::push_json, wfc::statistics::response::push_json>,
  wfc::jsonrpc::call_method< _get_names_, request::get_names_json, response::get_names_json>,
  wfc::jsonrpc::call_method< _get_, request::get_json, response::get_json>,
  wfc::jsonrpc::call_method< _merge_, request::merge_json, response::merge_json>,
  wfc::jsonrpc::call_method< _select_, request::select_json, response::select_json>,
  wfc::jsonrpc::call_method< _compact_, request::compact_json, response::compact_json>,
  wfc::jsonrpc::call_method< _tree_, request::tree_json, response::tree_json>
>
{};

template<typename Base>
class interface_
  : public Base
{
public:
  
  virtual void push(wfc::statistics::request::push::ptr req, wfc::statistics::response::push::handler cb ) override
  {
    this->template call< _push_ >( std::move(req), std::move(cb), nullptr);
  }
  
  virtual void del( wfc::statistics::request::del::ptr, wfc::statistics::response::del::handler cb) override
  {
    if ( cb != nullptr )
      cb( nullptr );
  }

  virtual void get( request::get::ptr req, response::get::handler cb ) override
  {
    this->template call<_get_>( std::move(req), std::move(cb), nullptr);
  }

  virtual void merge( request::merge::ptr req, response::merge::handler cb ) override
  {
    this->template call<_merge_>( std::move(req), std::move(cb), nullptr);
  }

  virtual void tree( request::tree::ptr req, response::tree::handler cb ) override
  {
    this->template call<_tree_>( std::move(req), std::move(cb), nullptr);
  }

  virtual void compact( request::compact::ptr req, response::compact::handler cb ) override
  {
    this->template call<_compact_>( std::move(req), std::move(cb), nullptr);
  }

  virtual void get_name_tree( request::get_name_tree::ptr , response::get_name_tree::handler cb ) override
  {
    if ( cb != nullptr )
      cb( nullptr );
  }

  virtual void select( request::select::ptr req, response::select::handler cb ) override
  {
    this->template call<_select_>( std::move(req), std::move(cb), nullptr);
  }
  
  virtual void get_names( request::get_names::ptr req, response::get_names::handler cb ) override
  {
    this->template call< _get_names_ >( std::move(req), std::move(cb), nullptr);
  }

  virtual void multi_get( request::multi_get::ptr , response::multi_get::handler cb ) override
  {
    if ( cb != nullptr )
      cb( nullptr );
  }

};

}}}}
