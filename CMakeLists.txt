cmake_minimum_required(VERSION 3.10)
project(ProjetoLegado CXX)

# Força o padrão C++ 2003
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Inclui a pasta de cabeçalhos (faz o #include "meu_app.h" funcionar)
include_directories(include)

# 2. Lista os arquivos fonte
set(SOURCES
        src/Jogo_Principal/main.cpp
        src/Jogo_Principal/main.h
)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# 3. Cria o executável
add_executable(Jogo ${SOURCES})
target_include_directories(Jogo PRIVATE src)