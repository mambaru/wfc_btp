#pragma once 

#include <string>

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#ifndef CONFIG_PATH
#define CONFIG_PATH .
#endif


namespace wamba{ namespace btp{

struct key_storage_options
{
  // Путь к базе данных для всех префиксов
  std::string db_path = "./key_storage";
  // Путь для WAL, если не указан то по умолчанью из rocksdb
  // если путь указано в ini, то wal_path + "prefix" + ini.wal_dir
  std::string wal_path = "";
  // Файл опций в формате ini
  std::string ini_path = STRINGIZE_VALUE_OF(CONFIG_PATH)"/key_storage.ini";
  // Время жизни в секундах 
  time_t TTL = 0;
  
  bool create_if_missing = true;
  bool auto_repair = true;
};

}}

