cmake_minimum_required(VERSION 3.29)
project(WebcamApp)

find_package(OpenCV REQUIRED)

add_executable(WebcamApp lab5.cpp)
target_link_libraries(WebcamApp ${OpenCV_LIBS})
