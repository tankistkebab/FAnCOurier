cmake_minimum_required(VERSION 3.20)
project(FAnCOurier LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FANCOURIER_BUILD_TESTS "Build the unit tests" ON)

# Core simulation logic, shared by the app and the tests
add_library(fancourier_core STATIC
        Logic/SimManager.cpp
        Logic/Utils.cpp
        Logic/Agent.cpp
        Logic/CustomAgents.cpp
        Logic/HiveMind.cpp
        Logic/Map_Building/IMapGenerator.cpp
        Logic/Map_Building/Map.cpp
        Logic/Map_Building/FileMapLoader.cpp
        Logic/Map_Building/ProceduralMapGenerator.cpp)
target_include_directories(fancourier_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(FAnCOurier main.cpp)
target_link_libraries(FAnCOurier PRIVATE fancourier_core)

if (FANCOURIER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
