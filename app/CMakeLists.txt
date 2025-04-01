cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "APP")

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC
    metrics
    generators
)

target_compile_options("APP" PRIVATE -O3 -mavx2 -mfma -mavx512f -mavx512vl -mavx512ifma)
