cmake_minimum_required(VERSION 3.20)
project(WW-VectorDB VERSION 0.1.0 LANGUAGES CXX)

# 设置 C++ 编译标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加源文件
set(CORE_SOURCES
    src/core/segment.cpp
    src/core/collection.cpp
)

set(UTILS_SOURCES
    src/utils/logger/common/level.cpp
    src/utils/logger/formatter/default_formatter.cpp
    src/utils/logger/logger/base.cpp
    src/utils/logger/logger/sync_logger.cpp
    src/utils/logger/logger.cpp
    src/utils/logger/sink/base.cpp
    src/utils/logger/sink/console_sink.cpp
    src/utils/uuid.cpp
    src/utils/memory_pool/common/platform.cpp
    src/utils/memory_pool/common/size.cpp
    src/utils/memory_pool/list/free_list.cpp
    src/utils/memory_pool/list/span_list.cpp
    src/utils/memory_pool/cache/page_cache.cpp
    src/utils/memory_pool/cache/central_cache.cpp
    src/utils/memory_pool/cache/thread_cache.cpp
)

# 创建模块子库
add_library(vectordb_core STATIC ${CORE_SOURCES})
add_library(vectordb_utils STATIC ${UTILS_SOURCES})

find_package(Threads REQUIRED)

# 创建主库
add_library(vectordb INTERFACE)

# 链接子库到主库
target_link_libraries(vectordb INTERFACE
    vectordb_core
    vectordb_utils
)

# 创建别名
add_library(WW::vectordb ALIAS vectordb)

enable_testing()

add_executable(sync_console_logger_test
    tests/utils/logger/sync_console_logger_test.cpp
)

target_link_libraries(sync_console_logger_test PRIVATE
    vectordb_core
    vectordb_utils
    Threads::Threads
)

add_test(NAME sync_console_logger_test COMMAND sync_console_logger_test)

add_executable(uuid_test
    tests/utils/uuid/uuid_test.cpp
)

target_link_libraries(uuid_test PRIVATE
    vectordb_utils
    Threads::Threads
)

add_test(NAME uuid_test COMMAND uuid_test)

add_executable(distance_test
    tests/utils/math/distance_test.cpp
)

target_link_libraries(distance_test PRIVATE
    vectordb_utils
    Threads::Threads
)

add_test(NAME distance_test COMMAND distance_test)

add_executable(thread_cache_test
    tests/utils/memory_pool/thread_cache_test.cpp
)

target_link_libraries(thread_cache_test PRIVATE
    vectordb_utils
    Threads::Threads
)

add_test(NAME thread_cache_test COMMAND thread_cache_test)
