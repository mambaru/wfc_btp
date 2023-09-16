#include "merge_operator.hpp"
#include "logger.hpp"
#include <algorithm>
#include <wrtstat/aggregator/api/aggregated_info.hpp>

namespace wamba{ namespace btp{

using namespace rocksdb;

namespace{

bool Deserialize(const Slice& ival, wrtstat::aggregated_info* oval)
{
  if ( ival.size() != sizeof(wrtstat::aggregated_info) )
    return false;
  *oval = *reinterpret_cast<const wrtstat::aggregated_info*>( static_cast<const void*>(ival.data()) );
  return true;
}

std::string Serialize(const wrtstat::aggregated_info& val)
{
  const char* beg = reinterpret_cast<const char*>(&val);
  const char* end = beg + sizeof(wrtstat::aggregated_info);
  return std::string(beg, end);
}

}

merge_operator::merge_operator()
{
}


bool merge_operator::Merge(
        const Slice& /*key*/,
        const Slice* existing_value,
        const Slice& value,
        std::string* new_value,
        Logger* logger) const
{

  wrtstat::aggregated_info res_ag={};
  if ( existing_value != nullptr )
  {
    if (!Deserialize(*existing_value, &res_ag)) {
      // if existing_value is corrupted, treat it as 0
      Log(logger, "existing value corruption");
      res_ag = wrtstat::aggregated_info();
    }
  }

  wrtstat::aggregated_info oper_ag={};

  if (!Deserialize(value, &oper_ag)) {
    // if operand is corrupted, treat it as 0
    Log(logger, "operand value corruption");
    oper_ag = wrtstat::aggregated_info();
  }
  res_ag += oper_ag;
  *new_value = Serialize(res_ag);
  return true;        // always return true for this, since we treat all errors as "zero".
}

const char* merge_operator::Name() const
{
  return "BtpMergeOperator";
}

}}
