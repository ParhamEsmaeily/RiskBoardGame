cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Declare folder-specific options
option(BUILD_CARDS_MAIN "Builds the Cards driver with its main method" TRUE)

# Add source files to the project
set(SOURCES
    CardsDriver.cpp
    Cards.cpp
)

# Add header files to the project
set(HEADERS
    Cards.h
)

message(STATUS "  Configuring Cards library...")

# Create a library from the source files
add_library(CardsLib ${SOURCES} ${HEADERS})
target_include_directories(CardsLib PUBLIC .)

if(BUILD_CARDS_MAIN)
    message(STATUS "  Configuring Cards driver...")

    # Create an executable from the driver file
    add_executable(CardsDriver CardsDriver.cpp)
    target_link_libraries(CardsDriver PRIVATE CardsLib)

    add_compile_definitions(INCLUDE_MAIN)
endif()