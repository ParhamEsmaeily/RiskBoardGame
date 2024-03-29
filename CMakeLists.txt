cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source file to the root project
set(SOURCES
    src/Cards/Cards.cpp
    src/Cards/CardsDriver.cpp
    src/CommandProcessor/Command.cpp
    src/CommandProcessor/CommandProcessor.cpp
    src/CommandProcessor/CommandProcessorDriver.cpp
    src/CommandProcessor/FileLineReader.cpp
    src/GameEngine/GameEngine.cpp
    src/GameEngine/GameEngineDriver.cpp
    src/Map/Map.cpp
    src/Map/MapDriver.cpp
    src/Orders/Orders.cpp
    src/Orders/OrdersDriver.cpp
    src/Player/Player.cpp
    src/Player/PlayerDriver.cpp
    src/Player/PlayerStrategiesDriver.cpp
    src/Player/PlayerStrategies.cpp
    src/LoggingObserver/LoggingObserver.cpp
    src/LoggingObserver/LoggingObserverDriver.cpp
    src/MainDriver.cpp
    src/Tournament/TournamentDriver.cpp
)

# Add include directories to the root project
set(INCLUDE_DIRS
    src/include
)

# Create an executable from the source files
add_executable(MainDriver ${SOURCES})
target_include_directories(MainDriver PRIVATE ${INCLUDE_DIRS})