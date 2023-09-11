#include "merge_operator.hpp"
#include "logger.hpp"
#include <algorithm>
#include <wrtstat/aggregator/api/aggregated_info.hpp>

namespace wamba{ namespace btp{

  /*
namespace{

void add_to_queue(message_queue& mqueue, std::vector<message>* message_list, const rocksdb::Slice& value )
{
  typedef wjson::vector_of<message_json> message_list_json;
  const char *beg = value.data();
  const char *end = beg + value.size();
  wjson::json_error je;
  message_list->clear();
  message_list_json::serializer()(*message_list, beg, end, &je);
  if ( je )
  {
    PUBSUB_LOG_ERROR("PUBSUB merge_operator::FullMergeV2 json_error: "
        << wjson::strerror::message_trace(je, beg, end)  )
    return;
  }
  for ( message& m : *message_list)
    mqueue.push( std::move(m) );
  message_list->clear();
}

}
*/

merge_operator::merge_operator()
{
}

bool merge_operator::FullMergeV2( const MergeOperationInput& merge_in, MergeOperationOutput* merge_out) const
try
{
  const auto& operands = merge_in.operand_list;
  if ( operands.empty() )
    return true;

  wrtstat::aggregated_info res_ag;
  if ( merge_in.existing_value != nullptr )
  {
    const auto& value = merge_in.existing_value;
    const wrtstat::aggregated_info* ptr_ag = reinterpret_cast<const wrtstat::aggregated_info*>( static_cast<const void*>(value->data()) );
    res_ag += *ptr_ag;
  }

  for ( const auto& op : operands  )
  {
    const wrtstat::aggregated_info* ptr_ag = reinterpret_cast<const wrtstat::aggregated_info*>( static_cast<const void*>( op.data() ) );
    res_ag += *ptr_ag;
  }

  wrtstat::aggregated_info* out_ag = reinterpret_cast<wrtstat::aggregated_info*>( static_cast<void*>(merge_out->new_value.data()) );
  *out_ag = res_ag;

  /*
  typedef std::vector<message> message_list_t;
  typedef wjson::vector_of<message_json> message_list_json;
  const auto& operands = merge_in.operand_list;

  if ( operands.empty() )
    return true;

  message_queue mqueue;
  message_list_t message_list;
  message_list.reserve(16);
  if ( merge_in.existing_value != nullptr )
  {
    add_to_queue(mqueue, &message_list, *merge_in.existing_value);
  }

  for ( const auto& op : operands  )
  {
    add_to_queue(mqueue, &message_list, op);
  }

  message_list.clear();

  mqueue.remove_death( time(nullptr) );
  mqueue.copy_to(std::back_inserter(message_list));
  merge_out->new_value.clear();
  message_list_json::serializer()(message_list, std::back_inserter(merge_out->new_value));

  PUBSUB_LOG_TRACE("Merge result: " << merge_out->new_value)
  */
  return true;
}
catch(const std::exception& e)
{
  wrtstat::aggregated_info ag_tmp;

  if ( merge_in.existing_value )
    merge_out->new_value = reinterpret_cast<const char*>(&ag_tmp);

  BTPLOG_ERROR("BTP merge_operator::FullMergeV2 exception: "<< e.what() )
  return true;
}
catch(...)
{
  wrtstat::aggregated_info ag_tmp;

  if ( merge_in.existing_value )
    merge_out->new_value = reinterpret_cast<const char*>(&ag_tmp);

  BTPLOG_ERROR("BTP merge_operator::FullMergeV2 unhandled exception")
  return true;
}

const char* merge_operator::Name() const
{
  return "BtpMergeOperator";
}

}}
