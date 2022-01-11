cmake_minimum_required(VERSION 3.1)

project(wfc_btp)

include(cmake/ci.cmake)
include(cmake/rocksdb.cmake)

ogenc_warning(-Wsign-conversion "" OFF)
ogenc_warning(-Wzero-as-null-pointer-constant "" OFF)
ogenc_warning(-Wswitch-default "" OFF)
ogenc_warning(-Wconversion "" OFF)
ogenc_warning(-Wsuggest-override "" OFF)
ogenc_warning(-Wunused-const-variable "" OFF)
ogenc_warning(-Wrange-loop-construct "" OFF)

wci_submodule(NAME wfc SUPERMODULE)
enable_stat()
get_rocksdb()

add_subdirectory(libbtp)
add_subdirectory(package)

if ( BUILD_TESTING )
  enable_testing()
  add_subdirectory(tests)
endif()
