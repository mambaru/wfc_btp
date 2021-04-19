cmake_minimum_required(VERSION 3.1)

project(wfc_btp)

include(cmake/ci.cmake)
include(cmake/rocksdb.cmake)

wci_remove_options(-Wsign-conversion)
wci_remove_options(-Wzero-as-null-pointer-constant)
wci_remove_options(-Wswitch-default)
wci_remove_options(-Wconversion)
wci_remove_options(-Wsuggest-override)

wci_getlib(NAME wfc ${WCI_SUPERMODULE})
enable_stat()
get_rocksdb()

add_subdirectory(libbtp)
add_subdirectory(package)

if ( BUILD_TESTING )
  enable_testing()
  add_subdirectory(tests)
endif()
