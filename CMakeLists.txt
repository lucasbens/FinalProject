cmake_minimum_required(VERSION 3.13)
project(Projet_c__)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Projet_c__
        date.cpp
        date.hpp
        features_definition_loan.hpp
        loan.cpp
        loan.hpp
        main.cpp)
