cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "MT_tests")

file(GLOB_RECURSE TARGET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${TARGET_NAME} ${TARGET_SRC})

target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/generators/include,
)

target_link_libraries(${TARGET_NAME} PUBLIC
    generators
    gtest
    gtest_main
    metrics
)

target_compile_options(${TARGET_NAME} PRIVATE -O3 -mavx -mavx2 -mfma -mavx512f -mavx512vl -mavx512ifma)

gtest_discover_tests(${TARGET_NAME})
