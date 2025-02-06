cmake_minimum_required(VERSION 3.14)
project(lab7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -fno-tree-vectorize")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/mat.cpp
        src/novec.cpp
        src/vec.cpp
        src/blas.cpp
)

find_package(CBLAS REQUIRED)
target_link_libraries(${PROJECT_NAME} cblas)
target_include_directories(${PROJECT_NAME} PRIVATE ./src/include)
