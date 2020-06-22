add_library(
  wfc_btp
    btp_package.cpp
    rt/rt_module.cpp
    rt/domain/rt_domain.cpp
    rt/domain/rt_domain_multiton.cpp
    rt/service/rt_service_multiton.cpp
    ag/ag_module.cpp
    ag/domain/ag_domain.cpp
    ag/domain/ag_domain_multiton.cpp
    ag/service/ag_service_multiton.cpp
    ag/gateway/ag_gateway_multiton.cpp
    ag/deprecated/btp_deprecated_gateway_multiton.cpp
)

wci_targets(wfc_btp)

set_source_files_properties(
  ${wfc_btp_SOURCE_DIR}/package/rt/domain/rt_domain.cpp
  ${wfc_btp_SOURCE_DIR}/package/ag/domain/ag_domain.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-sign-conversion"
)

set_source_files_properties(
  ${wfc_btp_SOURCE_DIR}/package/ag/domain/ag_domain.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-unused-macros"
)

target_include_directories(wfc_btp PUBLIC ${wfc_btp_SOURCE_DIR})
target_include_directories(wfc_btp PRIVATE ${wfc_btp_SOURCE_DIR}/package)
target_include_directories(wfc_btp PRIVATE ${CMAKE_SOURCE_DIR}/external/rocksdb)
target_include_directories(wfc_btp PRIVATE ${CMAKE_SOURCE_DIR}/external/rocksdb/include)

find_library(LIBRT rt)
target_link_libraries(wfc_btp PUBLIC wfc rocksdb btpstorage wfc)
add_dependencies(wfc_btp rocksdb btpstorage wfc)

build_info(wfc_btp btp)

install(TARGETS wfc_btp DESTINATION ${CMAKE_INSTALL_PREFIX})


