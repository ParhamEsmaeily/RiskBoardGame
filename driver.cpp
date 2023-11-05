#include <iostream>

#include "CommandProcessor/CommandProcessor.h"
#include "Cards/Cards.h"
#include "Orders/Orders.h"
#include "GameEngine/GameEngine.h"
#include "Map/Map.h"
#include "Player/Player.h"

int main(int argc, char const *argv[])
{
    int choice = -1;
    std::vector<std::string> argsVector = std::vector<std::string>(argv + 1, argv + argc); // create a vector of the command line arguments

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
        {
            const auto consoleFlagIterator = std::find(argsVector.begin(), argsVector.end(), "-console");
            if (consoleFlagIterator != argsVector.end())
            {
                testCommandProcessor();
                break;
            }

            auto fileFlagIterator = std::find(argsVector.begin(), argsVector.end(), "-file");

            if (fileFlagIterator != argsVector.end())
            {
                // advance the iterator by 1 to get the file name
                std::advance(fileFlagIterator, 1);
                testCommandProcessor(*fileFlagIterator);
                break;
            }

            std::cout << "Invalid command source provided. Please specify the '-console' or '-file <path>' flag.\n"
                      << std::endl;

            break;
        }
        default:
            std::cout << "Byyyye ;)" << std::endl;
            return 0;
        }
    }

    return 0;
}