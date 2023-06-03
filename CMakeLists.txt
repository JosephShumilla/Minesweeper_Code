cmake_minimum_required(VERSION 3.24)
project(MinesweeperFixed)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinesweeperFixed main.cpp Tile.cpp Tile.h Board.cpp Board.h TextureManager.cpp TextureManager.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
include_directories(c:/SFML/include)
target_link_libraries(MinesweeperFixed sfml-system sfml-window sfml-graphics sfml-audio)