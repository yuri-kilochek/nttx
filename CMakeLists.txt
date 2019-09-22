cmake_minimum_required(VERSION 3.11)

project(nttx)

add_executable(test
    test.cpp    
)

set_target_properties(test PROPERTIES
    CXX_STANDARD 20
)

target_include_directories(test PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
