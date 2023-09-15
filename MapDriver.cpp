#include <iostream>

#include "Map.h"

void testLoadMaps()
{
    const auto testMap = MapLoader::loadMap("maps/test.map");
    const auto worldMap = MapLoader::loadMap("maps/world.map");

    worldMap->validate();

    std::cout << *worldMap << std::endl;
}

int main()
{
    testLoadMaps();

    return 0;
}