cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME metrics)

file(GLOB_RECURSE TARGET_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/metrics/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/statistical_test/*.hpp
)

file(GLOB_RECURSE TARGET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

find_package(OpenMP REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

if(NOT OpenMP_CXX_FOUND)
	message(SEND_ERROR "Failed to find OpenMP.")
	return()
else()
	include_directories(${OpenMP_INCLUDE_DIRS})
    message("OpenMP FOUND dir is ${OpenMP_INCLUDE_DIRS}.")
endif()

add_library(${TARGET_NAME} STATIC ${TARGET_SRC} ${TARGET_HEADERS})

target_include_directories(${TARGET_NAME}
PUBLIC
    include
    include/statistical_test
PRIVATE
    include/metrics
)

target_link_libraries(${TARGET_NAME} 
PUBLIC
    Boost::math
    OpenMP::OpenMP_CXX
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

add_compile_options("-O3 -march=native -ffast-math -fopenmp")
