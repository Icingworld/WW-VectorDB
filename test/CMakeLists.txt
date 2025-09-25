file(GLOB TEST_SRCS "*.cpp")

add_executable(vectordb_test ${TEST_SRCS})

target_link_libraries(vectordb_test PRIVATE
    vectordb
    GTest::gtest
    GTest::gtest_main
)
