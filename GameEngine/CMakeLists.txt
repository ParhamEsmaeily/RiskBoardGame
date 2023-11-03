cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Declare folder-specific options
option(BUILD_GAME_ENGINE_MAIN "Builds the Game Engine driver with its main method" TRUE)

# Add source files to the project
set(SOURCES
    ./GameEngineDriver.cpp
    ./GameEngine.cpp
)

# Add header files to the project
set(HEADERS
    ./GameEngine.h
)

message(STATUS "  Configuring Game Engine library...")

# Create a library from the source files
add_library(GameEngineLib ${SOURCES} ${HEADERS})
target_include_directories(GameEngineLib PUBLIC .)

if(BUILD_GAME_ENGINE_MAIN)
    message(STATUS "  Configuring Game Engine driver...")

    # Create an executable from the driver file
    add_executable(GameEngineDriver GameEngineDriver.cpp)
    target_link_libraries(GameEngineDriver PRIVATE GameEngineLib)

    add_compile_definitions(INCLUDE_MAIN)
endif()