cmake_minimum_required(VERSION 4.0)
project(FAnCOurier)

set(CMAKE_CXX_STANDARD 20)

add_executable(FAnCOurier main.cpp
        SimManager.cpp
        Headers/SimManager.h
        Utils.cpp
        Headers/Utils.h)
