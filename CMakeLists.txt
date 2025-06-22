cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/saint/Downloads/vcpkg/scripts/buildsystems/vcpkg.cmake" 
    CACHE STRING "Vcpkg toolchain file")

project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)

add_executable(TowerDefense
    src/main.cpp
    src/options.cpp
    src/tower.cpp
    src/enemy.cpp
    src/play.cpp
    src/wave.cpp
    src/projectile.cpp
    src/sounds.cpp
    src/images.cpp
    src/messages.cpp
)

target_include_directories(TowerDefense PRIVATE include)
target_link_libraries(TowerDefense PRIVATE raylib)
