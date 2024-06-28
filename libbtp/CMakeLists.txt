cmake_minimum_required(VERSION 3.10)

project(btpstorage VERSION "0.9.8")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

include(cmake/ci)

wci_remove_options(-Wsign-conversion)
wci_remove_options(-Wzero-as-null-pointer-constant)
wci_remove_options(-Wswitch-default)
wci_remove_options(-Wconversion)
wci_remove_options(-Wsuggest-override)

add_subdirectory(btp)

if ( BUILD_TESTING )
  enable_testing()
  add_subdirectory(tests)
endif()

include(cmake/install)
