cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(front src/front/main.cpp)

target_link_libraries(front PRIVATE Qt6::Widgets)