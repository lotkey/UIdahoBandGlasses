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
else
  message(SEND_ERROR "libftdi library not found.")
endif(FTDI_LIBRARY)

if(FTDI_INCLUDE_DIR)
  message(STATUS "libftdi headers found in: ${FTDI_INCLUDE_DIR}")
else
  message(SEND_ERROR "libftdi headers not found.")
endif(FTDI_INCLUDE_DIR)

add_executable(transmitter
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

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

set_target_properties(transmitter PROPERTIES 
    CXX_STANDARD 17
)

target_include_directories(transmitter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/
    ${FTDI_INCLUDE_DIR}
)

target_link_libraries(transmitter 
    ${FTDI_LIBRARY}
    pthread
)