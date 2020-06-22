#include "aux.hpp"
#include <boost/filesystem.hpp>

namespace wamba{ namespace btp{

bool mkpath(const std::string& path, std::string* err)
{
  boost::system::error_code ec;
  if ( boost::filesystem::is_directory(path, ec) )
    return true;

  ec.clear();
  if ( boost::filesystem::create_directories(path, ec) )
    return true;
  *err = ec.message() + ": " + path;
  return false;
}

}}
