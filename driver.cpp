#include <iostream>

#include "CommandProcessor/CommandProcessor.h"
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
        std::cout << "Choose your poison \n1: Test Maps\n2: Test Players\n3: Test Orders\n4: Test Cards\n5: Test Game Engine\n6: Test Command Processor\nElse: exit ";
        std::cin >> choice;
        std::cout << std::endl;

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
        case 6:
            testCommandProcessor(true);
            break;
        default:
            std::cout << "Byyyye ;)" << std::endl;
            return 0;
        }
    }

    return 0;
}