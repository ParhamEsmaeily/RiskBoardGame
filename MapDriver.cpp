#include <iostream>

#include "Map.h"

void testLoadMaps()
{
    const auto testMap = MapLoader::loadMap("maps/test.map");
    const auto worldMap = MapLoader::loadMap("maps/world.map");

    Map::validate(*worldMap);

    std::cout << *worldMap << std::endl;

    std::cout << "\nALL CONTINENTS TEST" << std::endl;

    const auto continents = Map::getAllContinents(*worldMap);
    for (auto &&continent : continents)
    {
        std::cout << *continent << std::endl;
    }

    std::cout << "\nALL TERRITORIES TEST" << std::endl;

    const auto territories = Map::getAllTerritories(*worldMap);
    for (auto &&territory : territories)
    {
        std::cout << *territory << std::endl;
    }

    const auto testTerritory = territories.at(territories.size() / 3);

    std::cout << "\n\nTEST TERRITORY: " << *testTerritory << std::endl;

    std::cout << "\nADJACENT TERRITORIES TEST" << std::endl;

    const auto adjacentTerritories = Map::getAdjacentTerritories(*worldMap, *testTerritory);
    for (auto &&territory : adjacentTerritories)
    {
        std::cout << *territory << std::endl;
    }

    std::cout << "\nSAME CONTINENT TERRITORIES TEST" << std::endl;

    const auto sameContinentTerritories = Map::getAllTerritoriesInContinent(*worldMap, *testTerritory->getContinent());
    for (auto &&territory : sameContinentTerritories)
    {
        std::cout << *territory << std::endl;
    }
}

int main()
{
    testLoadMaps();

    return 0;
}