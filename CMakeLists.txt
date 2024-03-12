cmake_minimum_required(VERSION 3.27)
project(Lab1_NoMS)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab1_NoMS main.cpp
        Studio/Studio.h
        Film/Film.h
        Handlers/handlers.h)
