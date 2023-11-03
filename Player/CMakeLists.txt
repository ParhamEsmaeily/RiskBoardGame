cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Declare folder-specific options
option(BUILD_PLAYER_MAIN "Builds the Player driver with its main method" TRUE)

# Add source files to the project
set(SOURCES
    ./PlayerDriver.cpp
    ./Player.cpp
)

# Add header files to the project
set(HEADERS
    ./Player.h
)

message(STATUS "  Configuring Player library...")

# Create a library from the source files
add_library(PlayerLib ${SOURCES} ${HEADERS})
target_include_directories(PlayerLib PUBLIC .)

# Link any relevant libraries
target_link_libraries(PlayerLib PUBLIC MapLib OrdersLib CardsLib)

if(BUILD_PLAYER_MAIN)
    message(STATUS "  Configuring Player driver...")

    # Create an executable from the driver file
    add_executable(PlayerDriver PlayerDriver.cpp)
    target_link_libraries(PlayerDriver PRIVATE PlayerLib)

    add_compile_definitions(INCLUDE_MAIN)
endif()