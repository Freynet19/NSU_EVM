cmake_minimum_required(VERSION 3.29)

project(lab6)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/")

include(Findlibusb-1.0)

find_package(libusb-1.0 REQUIRED)

add_executable(lab6 mouseDriver.cpp)
target_include_directories(lab6 PUBLIC ${LIBUSB_1_INCLUDE_DIRS})
target_link_libraries(lab6 ${LIBUSB_1_LIBRARIES})
