cmake_minimum_required(VERSION 3.10)
project(Jogo CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-2.3.2")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(Jogo ${SOURCES})

target_include_directories(Jogo PRIVATE
    src
    "${SFML_ROOT}/include"
)

target_link_directories(Jogo PRIVATE "${SFML_ROOT}/lib")

target_link_libraries(Jogo PRIVATE
    debug sfml-graphics-d
    debug sfml-window-d
    debug sfml-system-d
    optimized sfml-graphics
    optimized sfml-window
    optimized sfml-system
)
