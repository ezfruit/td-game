cmake_minimum_required(VERSION 3.15)

project(RampartDefenders)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)

add_executable(RampartDefenders
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

target_include_directories(RampartDefenders PRIVATE include)
target_link_libraries(RampartDefenders PRIVATE raylib)

add_custom_command(TARGET RampartDefenders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:RampartDefenders>/assets
)