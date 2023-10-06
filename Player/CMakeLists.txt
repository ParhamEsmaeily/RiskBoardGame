cmake_minimum_required(VERSION 3.10) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Add source files to the project
set(SOURCES
    ./PlayerDriver.cpp
    ./Player.cpp
    ../Map/Map.cpp
    ../GameEngine/GameEngine.cpp
    ../Cards/Cards.cpp
    ../Orders/Orders.cpp
)

# Add header files to the project
set(HEADERS
    ./Player.h
    ../Map/Map.h
    ../GameEngine/GameEngine.h
    ../Cards/Cards.h
    ../Orders/Orders.h
)

# Create an executable from the source files
add_executable(PlayerDriver ${SOURCES} ${HEADERS})