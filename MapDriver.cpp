#include <iostream>

#include "Map.h"

void testLoadMap(const std::string &mapLocation)
{
    const auto map = MapLoader::loadMap(mapLocation);

    Map::validate(*map);

    std::cout << *map << std::endl;

    std::cout << "\nALL CONTINENTS TEST" << std::endl;

    const auto continents = Map::getAllContinents(*map);
    for (auto &&continent : continents)
    {
        std::cout << *continent << std::endl;
    }

    std::cout << "\nALL TERRITORIES TEST" << std::endl;

    const auto territories = Map::getAllTerritories(*map);
    for (auto &&territory : territories)
    {
        std::cout << *territory << std::endl;
    }

    const auto testTerritory = territories.at(territories.size() / 3);

    std::cout << "\n\nTEST TERRITORY: " << *testTerritory << std::endl;

    std::cout << "\nADJACENT TERRITORIES TEST" << std::endl;

    const auto adjacentTerritories = Map::getAdjacentTerritories(*map, *testTerritory);
    for (auto &&territory : adjacentTerritories)
    {
        std::cout << *territory << std::endl;
    }

    std::cout << "\nSAME CONTINENT TERRITORIES TEST" << std::endl;

    const auto sameContinentTerritories = Map::getAllTerritoriesInContinent(*map, *testTerritory->getContinent());
    for (auto &&territory : sameContinentTerritories)
    {
        std::cout << *territory << std::endl;
    }
}

void testLoadMaps()
{
    const auto maps = {"maps/test.map", "maps/world.map"};

    for (auto &&map : maps)
    {
        testLoadMap(std::string(map));

        std::cout << std::endl;
    }
}

int main()
{
    testLoadMaps();

    return 0;
}