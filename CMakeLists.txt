cmake_minimum_required(VERSION 4.0)
project(FAnCOurier)

set(CMAKE_CXX_STANDARD 20)

add_executable(FAnCOurier main.cpp
        SimManager.cpp
        Headers/SimManager.h
        Utils.cpp
        Headers/Utils.h
        "Logic/Map Building/IMapGenerator.cpp"
        "Headers/Map Building/IMapGenerator.h"
        "Logic/Map Building/Map.cpp"
        "Headers/Map Building/Map.h"
        "Logic/Map Building/FileMapLoader.cpp"
        "Headers/Map Building/FileMapLoader.h"
        "Logic/Map Building/ProceduralMapGenerator.cpp"
        "Headers/Map Building/ProceduralMapGenerator.h")
