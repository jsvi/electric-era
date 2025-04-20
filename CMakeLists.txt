cmake_minimum_required(VERSION 3.0)

project(electra2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    ChargingNetwork.cpp
    Charger.cpp
    Station.cpp
    AvailabilityEvent.cpp
    StationAvailabilityReport.cpp
    StationAvailabilityReportFactory.cpp
    StationAvailabilityEntry.cpp
)

add_executable(electra2
    main.cpp
    ${SOURCES}
)

install(TARGETS electra2 RUNTIME DESTINATION bin)

#only compile test files if debug build
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)


target_include_directories( electra2 PUBLIC "build/_deps/googletest-src/googletest/include")


enable_testing()

add_executable(
    station_test
    station_test.cpp
    ${SOURCES}
)
target_link_libraries(
    station_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(station_test)

endif() #Debug
