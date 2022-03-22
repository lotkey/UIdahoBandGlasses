cmake_minimum_required(VERSION 3.12)
project(receiver)

add_executable(receiver
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/../common/Color.cpp
)

set_target_properties(receiver PROPERTIES 
    CXX_STANDARD 17
)

target_include_directories(receiver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/../commomn/
)