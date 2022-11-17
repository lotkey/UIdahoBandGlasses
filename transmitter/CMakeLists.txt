# Authors:
#  @haydenccarroll Hayden Carroll
#  @lotkey Chris McVickar
cmake_minimum_required(VERSION 3.12)
project(transmitter)
set(CMAKE_BUILD_TYPE Debug)

find_path(FTDI_INCLUDE_DIR 
  names ftdi.h
  paths /usr/include
  /usr/local/include
  /opt/local/include
  PATH_SUFFIXES libftdi libftdi1
)

find_library(FTDI_LIBRARY
  names ftdi ftdi1
  paths /usr/lib
  /usr/local/lib
  /opt/local/lib
)

if(FTDI_LIBRARY)
  message(STATUS "libftdi library found: ${FTDI_LIBRARY}")
else()
  message(SEND_ERROR "libftdi library not found.")
endif(FTDI_LIBRARY)

if(FTDI_INCLUDE_DIR)
  message(STATUS "libftdi headers found in: ${FTDI_INCLUDE_DIR}")
else()
  message(SEND_ERROR "libftdi headers not found.")
endif(FTDI_INCLUDE_DIR)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/
  ${FTDI_INCLUDE_DIR}
)

add_library(transmitter_core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/BMP.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/FTDI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/FTDIDevice.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Instruction.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Mask.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Writer.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/../common/Color.cpp
)

target_link_libraries(transmitter_core
  ${FTDI_LIBRARY}
  pthread
)

add_executable(transmitter ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(test01_blink_red ${CMAKE_CURRENT_SOURCE_DIR}/tests/01_blink_red.cpp)
add_executable(test02_blink_rgb ${CMAKE_CURRENT_SOURCE_DIR}/tests/02_blink_rgb.cpp)
add_executable(test03_blink_red2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/03_blink_red2.cpp)
add_executable(test04_green_until_newline ${CMAKE_CURRENT_SOURCE_DIR}/tests/04_green_until_newline.cpp)
add_executable(test05_alternate_on_newline ${CMAKE_CURRENT_SOURCE_DIR}/tests/05_alternate_on_newline.cpp)

set_target_properties(transmitter_core PROPERTIES CXX_STANDARD 17)
set_target_properties(transmitter PROPERTIES CXX_STANDARD 17)
set_target_properties(test01_blink_red PROPERTIES CXX_STANDARD 17)
set_target_properties(test02_blink_rgb PROPERTIES CXX_STANDARD 17)
set_target_properties(test03_blink_red2 PROPERTIES CXX_STANDARD 17)
set_target_properties(test04_green_until_newline PROPERTIES CXX_STANDARD 17)
set_target_properties(test05_alternate_on_newline PROPERTIES CXX_STANDARD 17)

target_link_libraries(transmitter PUBLIC transmitter_core)
target_link_libraries(test01_blink_red PUBLIC transmitter_core)
target_link_libraries(test02_blink_rgb PUBLIC transmitter_core)
target_link_libraries(test03_blink_red2 PUBLIC transmitter_core)
target_link_libraries(test04_green_until_newline PUBLIC transmitter_core)
target_link_libraries(test05_alternate_on_newline PUBLIC transmitter_core)