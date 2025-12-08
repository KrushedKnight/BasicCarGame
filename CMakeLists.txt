cmake_minimum_required(VERSION 3.10)
project(SimpleTrafficGame)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/eigen-3.4.0)

link_directories(${SDL2_LIBRARY_DIRS})
link_directories(${SDL2_TTF_LIBRARY_DIRS})

add_executable(SimpleTrafficGame main.cpp
        Car.cpp
        Car.h
        constants.h
        RigidBody.cpp
        RigidBody.h
        Wheel.cpp
        Wheel.h
        GUI.cpp
        GUI.h
        constants.cpp)
target_link_libraries(SimpleTrafficGame ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Add tests subdirectory
add_subdirectory(tests)
