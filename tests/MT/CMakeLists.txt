cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "MT_tests")

file(GLOB_RECURSE TARGET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)

# Check AVX2
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2)
    set(CMAKE_REQUIRED_FLAGS "-mavx2")
    check_cxx_source_compiles("
        #include <immintrin.h>
        int main() { __m256i a = _mm256_setzero_si256(); return 0; }
    " AVX2_COMPILES)
    unset(CMAKE_REQUIRED_FLAGS)
    if(AVX2_COMPILES)
        add_compile_options(-mavx -mavx2 -mfma)
    endif()
endif()

# Check AVX512
check_cxx_compiler_flag("-mavx512f" COMPILER_SUPPORTS_AVX512)
if(COMPILER_SUPPORTS_AVX512)
    set(CMAKE_REQUIRED_FLAGS "-mavx512f")
    check_cxx_source_compiles("
        #include <immintrin.h>
        int main() { __m512i a = _mm512_setzero_si512(); return 0; }
    " AVX512_COMPILES)
    unset(CMAKE_REQUIRED_FLAGS)
    if(AVX512_COMPILES)
        add_compile_options(-mavx512f -mavx512vl -mavx512ifma)
    endif()
endif()

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

# target_compile_options(${TARGET_NAME} PRIVATE -O3)

gtest_discover_tests(${TARGET_NAME})
