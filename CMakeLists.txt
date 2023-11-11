cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Add source file to the root project
set(SOURCES
    Cards/Cards.cpp
    Cards/CardsDriver.cpp
    CommandProcessor/Command.cpp
    CommandProcessor/CommandProcessor.cpp
    CommandProcessor/CommandProcessorDriver.cpp
    CommandProcessor/FileLineReader.cpp
    GameEngine/GameEngine.cpp
    GameEngine/GameEngineDriver.cpp
    Map/Map.cpp
    Map/MapDriver.cpp
    Orders/Orders.cpp
    Orders/OrdersDriver.cpp
    Player/Player.cpp
    Player/PlayerDriver.cpp
    LoggingObserver/LoggingObserver.cpp
    LoggingObserver/LoggingObserverDriver.cpp
    driver.cpp
)

# Add include directories to the root project
set(INCLUDE_DIRS
    Cards
    CommandProcessor
    GameEngine
    Map
    Orders
    Player
    LoggingObserver
)

# Create an executable from the source files
add_executable(MainDriver ${SOURCES})
target_include_directories(MainDriver PRIVATE ${INCLUDE_DIRS})