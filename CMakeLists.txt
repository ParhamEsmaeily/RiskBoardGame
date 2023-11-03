cmake_minimum_required(VERSION 3.19) # Minimum version of CMake required

project(RiskBoardGame) # Name of your project

# Set C++ standard (you can change this to any standard you prefer)
set(CMAKE_CXX_STANDARD 17)

# Add source file to the root project
set(SOURCES
    driver.cpp
)

# Add header files to the root project
set(HEADERS
)

set(BUILD_CARDS_MAIN CACHE BOOL FALSE FORCE)
set(BUILD_GAME_ENGINE_MAIN CACHE BOOL FALSE FORCE)
set(BUILD_MAP_MAIN CACHE BOOL FALSE FORCE)
set(BUILD_ORDERS_MAIN CACHE BOOL FALSE FORCE)
set(BUILD_PLAYER_MAIN CACHE BOOL FALSE FORCE)

# Add individual drivers
add_subdirectory(Cards)
add_subdirectory(GameEngine)
add_subdirectory(Map)
add_subdirectory(Orders)
add_subdirectory(Player)

# Create an executable from the source files
add_executable(MainDriver ${SOURCES} ${HEADERS})
target_link_libraries(MainDriver PRIVATE CardsLib GameEngineLib MapLib OrdersLib PlayerLib)