cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(Astromash src/sprite.cpp src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/snake.cpp src/canon.cpp src/gameobject.cpp src/bullet.cpp src/asteroid.cpp src/ufo.cpp)
target_link_libraries(Astromash PRIVATE SDL2::SDL2main SDL2::SDL2-static)
