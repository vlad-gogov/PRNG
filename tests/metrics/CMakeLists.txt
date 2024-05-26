cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "Metrics_Test")


file(GLOB_RECURSE TARGET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${TARGET_NAME} ${TARGET_SRC})

include_directories(${ARMADILLO_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/metircs/include
)

target_link_libraries(${TARGET_NAME} PUBLIC
    metrics
    gtest
    gtest_main
)

gtest_discover_tests(${TARGET_NAME})
