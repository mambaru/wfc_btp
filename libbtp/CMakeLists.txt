cmake_minimum_required(VERSION 3.1)

project(libbtp)

add_subdirectory(btp)

if ( BUILD_TESTING )
  enable_testing()
  add_subdirectory(tests)
endif()
