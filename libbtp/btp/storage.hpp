#pragma once


#include <btp/types.hpp>
#include <btp/storage_options.hpp>

namespace wamba{ namespace btp{

class key_cache;
class key_storage;
class data_storage;

/**
 * Хранилище агрегированных статистических данных на базе RocksDB.
 * Данные счетчиков хранятся дискретно заданное время.
 * Счетчик - именованная сущность, связанная с дискретным набором агрегированных данных.
 * Данные выровненны по временным точкам в зависимости от конфигурации
 *    (например 5 секунд, 1 минута, 7 минут, 1 час, 1 день)
 *    временные отметки выравниваються по UTS
 */
class storage
{
public:
  virtual ~storage();

  bool open(const storage_options& opt, std::string* err);
  bool add(const std::string& name, aggregated_data&& data, std::string* err);
  bool get(const std::string& name, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit);
  bool merge(const std::string& name, aggregated_list&& data, std::string* err);
  bool compact(std::string* err);

  selected_names_t tree(
    const std::string& prefix,
    const std::string& sep,
    size_t depth,
    size_t offset,
    size_t limit,
    tree_node node,
    sort_by by,
    bool power
  ) const;

  selected_names_t select(
    const std::string& prefix,
    const std::string& suffix,
    size_t offset,
    size_t limit,
    sort_by by,
    bool power
  ) const;

  size_t gc();

  bool del(const std::string& name, std::string* err);

  bool close();
private:
  std::shared_ptr<key_cache>    _key_cache;
  std::shared_ptr<key_storage>  _key_storage;
  std::shared_ptr<data_storage> _data_storage;
  time_type _ttl = 0;
  time_type _resolution = 0;
  bool _disable_merge = false;

  storage_options::trace_fun_t _trace;
  std::string _keys_logname;
  std::string _data_logname;
};

}}

