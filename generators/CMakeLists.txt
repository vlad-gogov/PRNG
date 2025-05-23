cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME generators)

file(GLOB_RECURSE TARGET_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/generators/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/hash_functions/*.hpp
)

file(GLOB_RECURSE TARGET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${TARGET_NAME} STATIC ${TARGET_SRC} ${TARGET_HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC
    include/generators
    include
)

target_compile_options(generators PRIVATE -O3 -mavx -mavx2 -mfma -mavx512f -mavx512vl -mavx512ifma)
