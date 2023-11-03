cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Add source files to the project
set(SOURCES
    ./CardsDriver.cpp
    ./Cards.cpp
)

# Add header files to the project
set(HEADERS
    ./Cards.h
)

# Create an executable from the source files
add_executable(CardsDriver ${SOURCES} ${HEADERS})
