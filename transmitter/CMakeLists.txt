cmake_minimum_required(VERSION 3.12)
project(transmitter)
set(CMAKE_BUILD_TYPE Debug)

add_executable(transmitter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BMP.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mask.cpp
    
<<<<<<< HEAD
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Color.cpp
=======
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/Color.cpp
>>>>>>> c37b4a923ef561d9e2a11ee8b17a9c55b84ae5c9
)
# include_directories(../common)
set_target_properties(transmitter PROPERTIES 
    CXX_STANDARD 17
)

target_include_directories(transmitter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/
)