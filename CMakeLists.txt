cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(CppND-Capstone-MeteorCanon-Game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(PRIVATE ${SDL2_image_INCLUDE_DIRS} ${SDL2_tff_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} src lib)

add_executable( MeteorCanon 
                src/spritecomponent.cpp 
                src/keyboardcontrolcomponent.cpp 
                src/tilecomponent.cpp 
                src/map.cpp 
                src/main.cpp 
                src/game.cpp
                src/fontmanager.cpp
                src/texturemanager.cpp 
                src/assetmanager.cpp 
                src/entity.cpp 
                src/component.cpp 
                src/transformcomponent.cpp 
                src/entitymanager.cpp
                src/collidercomponent.cpp 
                src/collision.cpp
                src/textlabelcomponent.cpp
                src/bulletemittercomponent.cpp)

target_link_libraries(MeteorCanon SDL2::Core SDL2::Image SDL2::TTF)
