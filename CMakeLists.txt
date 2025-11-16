cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB SOURCES 
    "src/*.cpp"
    "src/front/*.cpp"
    "src/front/components/*.cpp"
)

add_executable(front ${SOURCES})

target_link_libraries(front PRIVATE Qt6::Widgets)

qt6_add_resources(RESOURCES src/resources.qrc)
target_sources(front PRIVATE ${RESOURCES})
