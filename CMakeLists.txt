cmake_minimum_required(VERSION 3.16)
project(recommendation-algorithm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.0/mingw_64")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/front/*.cpp"
    "src/front/components/*.cpp"
)

set(RESOURCES src/resources.qrc)

qt6_add_resources(QRC_SOURCES ${RESOURCES})

add_executable(front
    ${SOURCES}
    ${QRC_SOURCES}
)

target_link_libraries(front PRIVATE Qt6::Widgets)

set_target_properties(front PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

if (WIN32)
    set(WINDEPLOYQT_EXECUTABLE "C:/Qt/6.10.0/mingw_64/bin/windeployqt.exe")

    add_custom_target(deploy
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-quick-import
                --no-translations
                --compiler-runtime
                --dir "$<TARGET_FILE_DIR:front>"
                "$<TARGET_FILE:front>"
        DEPENDS front
        COMMENT "Running windeployqt to deploy Qt dependencies..."
        VERBATIM
    )
endif()
