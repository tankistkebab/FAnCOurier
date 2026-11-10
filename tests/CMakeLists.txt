include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(fancourier_tests
        test_helpers.h
        MapTests.cpp
        PathfinderTests.cpp
        AgentTests.cpp
        HiveMindTests.cpp)
target_link_libraries(fancourier_tests PRIVATE fancourier_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(fancourier_tests)
