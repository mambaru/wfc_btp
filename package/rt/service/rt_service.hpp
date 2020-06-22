#pragma once

#include <rt/api/depr/add_json.hpp>
#include <rt/api/depr/multi_add_json.hpp>

#include <wfc/statistics/api/push_json.hpp>
#include <wfc/statistics/api/del_json.hpp>
#include <rt/irt_depr.hpp>
#include <wfc/jsonrpc.hpp>

namespace wamba{ namespace btp{ namespace service{

JSONRPC_TAG(push)
JSONRPC_TAG(del)

JSONRPC_TAG(add)
JSONRPC_TAG(multi_add)

struct method_list: wfc::jsonrpc::method_list
<
  wfc::jsonrpc::target< irt_depr >,
  wfc::jsonrpc::invoke_method< 
    _push_, 
    wfc::statistics::request::push_json, 
    wfc::statistics::response::push_json, 
    wfc::istatistics, 
    &wfc::istatistics::push
  >,
  wfc::jsonrpc::invoke_method< 
    _del_, 
    wfc::statistics::request::del_json, 
    wfc::statistics::response::del_json, 
    wfc::istatistics, 
    &wfc::istatistics::del
  >,
  //depr : 
  wfc::jsonrpc::invoke_method< 
    _add_, 
    ::wamba::btp::rtdepr::request::add_json, 
    ::wamba::btp::rtdepr::response::add_json, 
    irt_depr, 
    &irt_depr::add
  >,
  wfc::jsonrpc::invoke_method< 
    _multi_add_, 
    ::wamba::btp::rtdepr::request::multi_add_json, 
    ::wamba::btp::rtdepr::response::multi_add_json, 
    irt_depr, 
    &irt_depr::multi_add
  >

>
{};

}}}
