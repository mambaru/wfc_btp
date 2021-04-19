#pragma once

#include <wfc/statistics/istatistics.hpp>
#include <wfc/jsonrpc.hpp>

#include <wrtstat/api/push_json.hpp>
#include <wrtstat/api/multi_push_json.hpp>
#include <wrtstat/api/del_json.hpp>


#include <ag/api/get_names_json.hpp>
#include <ag/api/merge_json.hpp>
#include <ag/api/select_json.hpp>
#include <ag/api/tree_json.hpp>
#include <ag/api/get_json.hpp>
#include <ag/api/multi_get_json.hpp>
#include <ag/api/compact_json.hpp>
#include <ag/iag.hpp>


namespace wamba{ namespace btp{ namespace ag{  namespace gateway{ 

using namespace ::wfc::statistics;
using namespace ::wrtstat;
  
JSONRPC_TAG(push)
JSONRPC_TAG(multi_push)
JSONRPC_TAG(del)
  
JSONRPC_TAG(merge)
JSONRPC_TAG(get_names)
JSONRPC_TAG(get_name_tree)
JSONRPC_TAG(get)
JSONRPC_TAG(select)
JSONRPC_TAG(compact)
JSONRPC_TAG(tree)
JSONRPC_TAG(multi_get)
/*
 *  virtual void get( request::get::ptr req, response::get::handler cb ) = 0;
 *  virtual void merge( request::merge::ptr req, response::merge::handler cb ) = 0;
 *  virtual void tree( request::tree::ptr req, response::tree::handler cb ) = 0;
 *  virtual void select( request::select::ptr req, response::select::handler cb ) = 0;
 *  virtual void get_names( request::get_names::ptr req, response::get_names::handler cb ) = 0;
 *  virtual void get_name_tree( request::get_name_tree::ptr req, response::get_name_tree::handler cb ) = 0;
 *  virtual void multi_get( request::multi_get::ptr req, response::multi_get::handler cb ) = 0;
 *  virtual void compact( request::compact::ptr req, response::compact::handler cb ) = 0;
 * 
 */
struct method_list: public ::wfc::jsonrpc::method_list
<
  wfc::jsonrpc::interface_<ibtp>,
  
  wfc::jsonrpc::call_method< _push_, ::wrtstat::request::push_json, ::wrtstat::response::push_json>,
  wfc::jsonrpc::call_method< _multi_push_, ::wrtstat::request::multi_push_json, ::wrtstat::response::multi_push_json>,
  wfc::jsonrpc::call_method< _del_, ::wrtstat::request::del_json, ::wrtstat::response::del_json>,
  
  wfc::jsonrpc::call_method< _get_, request::get_json, response::get_json>,
  wfc::jsonrpc::call_method< _merge_, request::merge_json, response::merge_json>,
  wfc::jsonrpc::call_method< _tree_, request::tree_json, response::tree_json>,
  wfc::jsonrpc::call_method< _select_, request::select_json, response::select_json>,
  wfc::jsonrpc::call_method< _get_names_, request::get_names_json, response::get_names_json>,
  wfc::jsonrpc::call_method< _get_name_tree_, request::get_name_tree_json, response::get_name_tree_json>,
  wfc::jsonrpc::call_method< _multi_get_, request::multi_get_json, response::multi_get_json>,
  wfc::jsonrpc::call_method< _compact_, request::compact_json, response::compact_json>
>
{};

template<typename Base>
class interface_
  : public Base
{
public:
  virtual void push( ::wrtstat::request::push::ptr req, ::wrtstat::response::push::handler cb ) override
  {
    this->template call<_push_>( std::move(req), std::move(cb), nullptr);
  }
  
  virtual void multi_push( ::wrtstat::request::multi_push::ptr req, ::wrtstat::response::multi_push::handler cb ) override
  {
    this->template call<_multi_push_>( std::move(req), std::move(cb), nullptr);
  }
  
  virtual void del( ::wrtstat::request::del::ptr req, ::wrtstat::response::del::handler cb ) override
  {
    this->template call<_del_>( std::move(req), std::move(cb), nullptr);
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
