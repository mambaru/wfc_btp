#include "data_storage.hpp"
#include "shard_rocksdb.hpp"

namespace wamba{ namespace btp{

data_storage::~data_storage()
{
}

bool data_storage::open(const data_storage_options& opt, std::string* err )
{
  _db = std::make_shared<shard_rocksdb>();
  return _db->open(opt, err);
}

bool data_storage::compact(std::string* err)
{
  return _db->compact(err);
}

bool data_storage::close( std::string* err )
{
  bool res = _db->close(err);
  _db = nullptr;
  return res;
}

bool data_storage::set(key_id_t id, const aggregated_info& data, std::string* err)
{
  return _db->set(id, data, err);
}

bool data_storage::inc(key_id_t id, const aggregated_info& data, std::string* err)
{
  return _db->inc(id, data, err);
}

bool data_storage::get(key_id_t id, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit)
{
  return _db->get(id, result, err, ts, offset, limit);
}

bool data_storage::del(key_id_t id, std::string* err)
{
  return _db->del(id, err);
}

}}
