cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Declare folder-specific options
option(BUILD_MAP_MAIN "Builds the Map driver with its main method" TRUE)

# Add source files to the project
set(SOURCES
    ./MapDriver.cpp
    ./Map.cpp
)

# Add header files to the project
set(HEADERS
    ./Map.h
)

message(STATUS "  Configuring Map library...")

# Create a library from the source files
add_library(MapLib ${SOURCES} ${HEADERS})
target_include_directories(MapLib PUBLIC .)

if(BUILD_MAP_MAIN)
    message(STATUS "  Configuring Map driver...")

    # Create an executable from the driver file
    add_executable(MapDriver MapDriver.cpp)
    target_link_libraries(MapDriver PRIVATE MapLib)

    add_compile_definitions(INCLUDE_MAIN)
endif()
