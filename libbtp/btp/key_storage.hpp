#pragma once

#include <string>
#include <mutex>
#include <map>
#include <btp/types.hpp>
#include <btp/stored_key.hpp>
#include <btp/key_storage_options.hpp>
#include <btp/key_rocksdb.hpp>
namespace wamba{ namespace btp{

class key_storage
{
public:
  typedef std::function< void( std::vector<stored_name> ) > load_fun_t;
  virtual ~key_storage();
  bool open(const key_storage_options& opt, std::string* err );
  bool close( std::string* err );
  bool compact(std::string* err);
  bool load(size_t step, load_fun_t handler, std::string* err );
  bool set(const std::string& name, const stored_key& data, std::string* err);
  bool get(const std::string& name, stored_key* data, std::string* err);
  bool del(const std::string& name, std::string* err);
private:
  std::shared_ptr<key_rocksdb> _rocksdb;
};

}}

