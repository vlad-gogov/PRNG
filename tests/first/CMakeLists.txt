cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "first_test")

file(GLOB_RECURSE TARGET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${TARGET_NAME} ${TARGET_SRC})

target_link_libraries(${TARGET_NAME} PUBLIC
    gtest
    gtest_main
)

gtest_discover_tests(${TARGET_NAME})
