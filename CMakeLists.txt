cmake_minimum_required(VERSION 3.13)        # CMake version check
project(CppND-System-Monitor)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)                  # Enable c++14 standard


# Add main.cpp file of project root directory as source file
set(SOURCE_FILES main.cpp)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
# Add executable target with source files listed in SOURCE_FILES variable
add_executable(CppND-System-Monitor ${SOURCE_FILES})

target_link_libraries(CppND-System-Monitor ${CURSES_LIBRARIES})