cmake_minimum_required(VERSION 3.10)  # Minimum version of CMake required

project(Cards)  # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 14)

# Add source files to the project
set(SOURCES 
    ./CardsDriver.cpp
    ./Cards.cpp
)

# Add header files to the project
set(HEADERS
    ./Cards.hpp
)

# Create an executable from the source files
add_executable(executable ${SOURCES} ${HEADERS})

# Specify include directories (if any)
target_include_directories(executable PRIVATE include)
