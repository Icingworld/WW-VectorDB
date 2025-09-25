file(GLOB_RECURSE SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/db/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
)

add_library(vectordb STATIC ${SRCS})

target_include_directories(vectordb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/db
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    /usr/local/rocksdb/include
)

find_package(RocksDB REQUIRED)

target_link_libraries(vectordb PUBLIC
    /usr/local/rocksdb/lib/librocksdb.a
)
