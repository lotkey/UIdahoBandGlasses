cmake_minimum_required(VERSION 3.12)
project(transmitter)

add_executable(transmitter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Symbol.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Color.cpp
)

set_target_properties(transmitter PROPERTIES 
    CXX_STANDARD 17
)

target_include_directories(transmitter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)