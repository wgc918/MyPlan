cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(qt_test LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Set both Qt Kit and MinGW compiler paths
set(CMAKE_PREFIX_PATH "D:/QT/6.9.3/mingw_64;D:/QT/Tools/mingw1310_64") # Qt Kit Dir and MinGW Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED) # Qt COMPONENTS

# Source files
set(SOURCES
    src/main.cpp
    src/qt_test.cpp
    src/qt_test.h
    src/task.cpp
    src/task.h
    src/taskmanager.cpp
    src/taskmanager.h
)

# UI files
set(UI_FILES
    src/qt_test.ui
)

# Workaround for GCC 8.x compatibility with Qt 6.9.3
add_compile_options(-fno-strict-aliasing)

# Specify UTF-8 encoding for MSVC (optional)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${UI_FILES}
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui) # Qt6 Shared Library

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "D:/QT/6.9.3/mingw_64/bin/windeployqt.exe" 
    ARGS --dir "${CMAKE_BINARY_DIR}" "$<TARGET_FILE:${PROJECT_NAME}>"
    COMMENT "Deploying Qt libraries..."
)