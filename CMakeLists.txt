cmake_minimum_required(VERSION 3.27)
project(Lab1_NoMS)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab1_NoMS main.cpp
        Studio/Studio.h
        Film/Film.h
        Helpers/helpers.h
        Studio/IndexTable/Index.cpp
        Studio/IndexTable/Index.h
        Studio/Studio.cpp
        Film/Film.cpp
        Helpers/helpers.cpp)
