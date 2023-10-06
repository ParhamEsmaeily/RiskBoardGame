cmake_minimum_required(VERSION 3.10) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Add source files to the project
set(SOURCES
    driver.cpp
    ./Player/Player.cpp
    ./Player/PlayerDriver.cpp
    ./Map/Map.cpp
    ./Map/MapDriver.cpp
    ./GameEngine/GameEngine.cpp
    ./GameEngine/GameEngineDriver.cpp
    ./Cards/Cards.cpp
    ./Cards/CardsDriver.cpp
    ./Orders/Orders.cpp
    ./Orders/OrdersDriver.cpp
)

# Add header files to the project
set(HEADERS
    ./Player/Player.h
    ./Map/Map.h
    ./GameEngine/GameEngine.h
    ./Cards/Cards.h
    ./Orders/Orders.h
)

# Create an executable from the source files
add_executable(MainDriver ${SOURCES} ${HEADERS})