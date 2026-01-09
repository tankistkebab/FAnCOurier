cmake_minimum_required(VERSION 4.0)
project(FAnCOurier)

set(CMAKE_CXX_STANDARD 20)

add_executable(FAnCOurier main.cpp
        Logic/SimManager.cpp
        Headers/SimManager.h
        Logic/Utils.cpp
        Headers/Utils.h
        Logic/Map_Building/IMapGenerator.cpp
        "Headers/Map_Building/IMapGenerator.h"
        "Logic/Map_Building/Map.cpp"
        "Headers/Map_Building/Map.h"
        "Logic/Map_Building/FileMapLoader.cpp"
        "Headers/Map_Building/FileMapLoader.h"
        "Logic/Map_Building/ProceduralMapGenerator.cpp"
        "Headers/Map_Building/ProceduralMapGenerator.h"
        Logic/Agent.cpp
        Headers/Agent.h
        Logic/CustomAgents.cpp
        Headers/CustomAgents.h)
