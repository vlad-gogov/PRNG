cmake_minimum_required(VERSION 3.16)

if(ENABLE_TESTS)
    add_subdirectory(googletest)
endif()

add_subdirectory(boost)

