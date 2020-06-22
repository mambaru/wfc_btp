#pragma once 

#include <string>
#include <mutex>
#include <map>
#include <btp/types.hpp>
#include <btp/data_storage_options.hpp>

namespace wamba{ namespace btp{

class shard_rocksdb;
class data_storage
{
public:
  virtual ~data_storage();
  bool open(const data_storage_options& opt, std::string* err );
  bool close( std::string* err );
  bool compact(std::string* err);
  bool set(key_id_t id, const aggregated_info& data, std::string* err);
  bool get(key_id_t id, aggregated_list* result, std::string* err, time_type ts=0, size_t offset=0, size_t limit = 0);
private:
  std::shared_ptr<shard_rocksdb> _db;
};

}}

