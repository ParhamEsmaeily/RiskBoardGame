cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Declare folder-specific options
option(BUILD_ORDERS_MAIN "Builds the Orders driver with its main method" TRUE)

# Add source files to the project
set(SOURCES
    ./OrdersDriver.cpp
    ./Orders.cpp
)

# Add header files to the project
set(HEADERS
    ./Orders.h
)

message(STATUS "  Configuring Orders library...")

# Create a library from the source files
add_library(OrdersLib ${SOURCES} ${HEADERS})
target_include_directories(OrdersLib PUBLIC .)

if(BUILD_ORDERS_MAIN)
    message(STATUS "  Configuring Orders driver...")

    # Create an executable from the driver file
    add_executable(OrdersDriver OrdersDriver.cpp)
    target_link_libraries(OrdersDriver PRIVATE OrdersLib)

    add_compile_definitions(INCLUDE_MAIN)
endif()