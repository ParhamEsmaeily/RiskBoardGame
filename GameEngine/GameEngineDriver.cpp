#include <iostream>
#include <string>
#include "GameEngine.h"

using namespace std;

void testGameStates()
{
    GameEngine *gameEngine = new GameEngine();

    while (gameEngine->getPhase() != "end")
    {
        string commandList = gameEngine->getCurrCommandsList();

        cout << "\nCurrent Phase: " << gameEngine->getPhase() << endl;
        cout << "Enter a command from the list below (number or name):" << endl;
        cout << commandList << endl;

        string input;
        cin >> input;

        gameEngine->executeCommand(input);
    }
}

int main()
{
    testGameStates();
    return 0;
}