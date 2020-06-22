#pragma once

#include <wfc/statistics/api/push_json.hpp>
#include <wfc/statistics/api/del_json.hpp>
#include <ag/iag.hpp>
#include <ag/api/get_json.hpp>
#include <ag/api/merge_json.hpp>
#include <ag/api/multi_get_json.hpp>
#include <ag/api/tree_json.hpp>
#include <ag/api/select_json.hpp>
#include <ag/api/compact_json.hpp>
#include <ag/api/get_names_json.hpp>
#include <wfc/jsonrpc.hpp>

namespace wamba{ namespace btp{ namespace ag{ namespace service{ 

JSONRPC_TAG(push)
JSONRPC_TAG(del)
JSONRPC_TAG(get)
JSONRPC_TAG(merge)
JSONRPC_TAG(multi_get)
JSONRPC_TAG(tree)
JSONRPC_TAG(get_name_tree)
JSONRPC_TAG(select)
JSONRPC_TAG(get_names)
JSONRPC_TAG(compact)

struct method_list: wfc::jsonrpc::method_list
<
  wfc::jsonrpc::target< iag >,
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
  wfc::jsonrpc::invoke_method<  _get_,    request::get_json,    response::get_json,    iag, &iag::get >,
  wfc::jsonrpc::invoke_method<  _merge_,    request::merge_json,    response::merge_json,    iag, &iag::merge >,
  wfc::jsonrpc::invoke_method<  _multi_get_,    request::multi_get_json,    response::multi_get_json,    iag, &iag::multi_get >,
  wfc::jsonrpc::invoke_method<  _tree_,   request::tree_json,   response::tree_json,   iag, &iag::tree >,
  wfc::jsonrpc::invoke_method<  _get_name_tree_,   request::get_name_tree_json,   response::get_name_tree_json,   iag, &iag::get_name_tree >,
  wfc::jsonrpc::invoke_method<  _select_, request::select_json, response::select_json, iag, &iag::select >,
  wfc::jsonrpc::invoke_method<  _get_names_, request::get_names_json, response::get_names_json, iag, &iag::get_names >,
  wfc::jsonrpc::invoke_method<  _compact_, request::compact_json, response::compact_json, iag, &iag::compact >
>
{};

}}}}
