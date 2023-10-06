#include <iostream>

#include "Cards/Cards.h"
#include "Orders/Orders.h"
#include "GameEngine/GameEngine.h"
#include "Map/Map.h"
#include "Player/Player.h"

int main()
{
    int choice = -1;

    while (true)
    {
        std::cout << "Choose your poison (1: Test Maps, 2: Test Players, 3: Test Orders, 4: Test Cards & 5: Test Game Engine, else: exit): ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            testLoadMaps();
            break;
        case 2:
            testPlayers();
            break;
        case 3:
            testOrdersList();
            break;
        case 4:
            test_cards();
            break;
        case 5:
            testGameStates();
            break;
        default:
            std::cout << "Byyyye ;)" << std::endl;
            return 0;
        }
    }

    return 0;
}