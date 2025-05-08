cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME "APP")

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC
    metrics
    generators
)

option(BUILD_WITH_ASAN "Enable AddressSanitizer" OFF)
option(BUILD_WITH_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

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
        # add_compile_options(-mavx -mavx2 -mfma)
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
        # add_compile_options(-mavx512f -mavx512vl -mavx512ifma)
    endif()
endif()

target_compile_options(${TARGET_NAME} PRIVATE -O3 -funroll-loops -ftree-vectorize)

if(BUILD_WITH_ASAN)
    message(STATUS "AddressSanitizer ENABLED")
    add_compile_options(${TARGET_NAME} PRIVATE
        -O1
        -fsanitize=address
        -fno-omit-frame-pointer
    )
    target_link_options(${TARGET_NAME} PRIVATE
        -fsanitize=address
    )
endif()

if(BUILD_WITH_UBSAN)
    message(STATUS "UndefinedBehaviorSanitizer ENABLED")
    add_compile_options(${TARGET_NAME} PRIVATE
        -O1
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )
    target_link_options(${TARGET_NAME} PRIVATE
        -fsanitize=undefined
    )
endif()

