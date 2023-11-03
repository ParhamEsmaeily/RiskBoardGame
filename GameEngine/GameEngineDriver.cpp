#include <iostream>
#include <string>
#include "GameEngine.h"

using std::cin;
using std::cout;
using std::endl;

void testGameStates()
{
    auto *gameEngine = new GameEngine();

    while (gameEngine->getPhase() != "end")
    {
        cout << "\n"
             << *gameEngine << endl;
        cout << "Enter a command from the list below (# or name):" << endl;
        cout << gameEngine->getCurrCommandsList() << endl;

        std::string input;
        cin >> input;

        gameEngine->executeCommand(input);
    }
}

#ifdef INCLUDE_MAIN

int main()
{
    testGameStates();

    return 0;
}

#endif