#pragma once

#include <rocksdb/merge_operator.h>
#include <rocksdb/env.h>

namespace wamba{ namespace btp{

class merge_operator
  : public ::rocksdb::MergeOperator
{
public:
  typedef ::rocksdb::Slice slice_type;

  merge_operator();

  virtual const char* Name() const override;

  virtual bool FullMergeV2(const MergeOperationInput& merge_in,
                           MergeOperationOutput* merge_out) const override;
};

}}
