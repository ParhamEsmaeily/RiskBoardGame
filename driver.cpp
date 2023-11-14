#include <iostream>

#include "LoggingObserver.h"
#include "CommandProcessor.h"
#include "Cards.h"
#include "Orders.h"
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Command.h"

int main(int argc, char const *argv[])
{
    // Using obs::path, path to the gamelog file.
    std::ofstream ofs;
    ofs.open(obs::path); // Clears the gamelog file if text was still inside.
    ofs << "Gamelog." << std::endl;

    int choice = -1;
    std::vector<std::string> argsVector = std::vector<std::string>(
        argv + 1, argv + argc); // create a vector of the command line arguments

    const auto testRunIterator =
        std::find(argsVector.begin(), argsVector.end(), "-test");

    // if the '-test' flag is not provided, run the game
    if (testRunIterator == argsVector.end())
    {
        // run actual game

        return 0;
    }

    // otherwise, if the '-test' flag is provided, run the tests and exit
    while (true)
    {
        std::cout << "Choose your poison \n1: Test Maps\n2: Test Players\n3: Test Orders\n4: Test Cards\n5: Test Game Engine\n6: Test Command Processor\n7: Test Startup Phase\n8: Test Main Game Loop \n9: Test Logging Observer\nElse: exit\n";
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
            // if the '-console' flag is provided, run the command processor using the console as source
            const auto consoleFlagIterator = std::find(argsVector.begin(), argsVector.end(), "-console");
            if (consoleFlagIterator != argsVector.end())
            {
                testCommandProcessor();
                break;
            }

            // if the '-file' flag is provided, run the command processor using the file as source
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
        case 7:
            testStartupPhase();
            break;
        case 8:
            testMainGameLoop();
            break;
        case 9:
            test_LoggingObserver();
            break;
        default:
            std::cout << "Byyyye ;)" << std::endl;
            return 0;
        }
    }

    return 0;
}