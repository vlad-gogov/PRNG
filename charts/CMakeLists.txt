cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "CHARTS")

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC
    metrics
    generators
)

target_compile_options("CHARTS" PRIVATE -O3)
