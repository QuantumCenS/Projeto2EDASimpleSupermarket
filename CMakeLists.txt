cmake_minimum_required(VERSION 4.1)
project(Projeto2EDA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Projeto2EDA
        main.cpp
        supermercado.cpp
        supermercado.h
        FilaArmazem.cpp
        FilaArmazem.h
        menu.cpp
        menu.h)
