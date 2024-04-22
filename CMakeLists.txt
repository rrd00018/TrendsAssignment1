cmake_minimum_required(VERSION 3.26)
project(IA)

set(CMAKE_CXX_STANDARD 17)

add_executable(IA main.cpp
        argumentSolver.cpp
        argumentSolver.h
        argumentBuilder.cpp
        argumentBuilder.h
        Rule.cpp
        Rule.h
        Formula.cpp
        Formula.h
        Argument.cpp
        Argument.h)
