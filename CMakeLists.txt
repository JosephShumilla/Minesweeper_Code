cmake_minimum_required(VERSION 3.27)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)

add_executable(Minesweeper main.cpp
        Board.cpp
        Board.h
        Tile.cpp
        Tile.h
        TextureManager.cpp
        TextureManager.h
        Leaderboard.cpp
        Leaderboard.h)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
include_directories(C:/SFML/include)
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio)