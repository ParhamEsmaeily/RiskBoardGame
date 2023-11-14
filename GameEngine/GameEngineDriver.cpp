#include "GameEngine.h"
#include <iostream>
#include <string>

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

    delete gameEngine;
    gameEngine = nullptr;
}

// void GameEngine::testStartupPhase(){
//   this->startupPhase();

// }
void testStartupPhase()
{
    auto *gameEngine = new GameEngine();
    gameEngine->startupPhase();
    delete gameEngine;
    gameEngine = nullptr;
}

void testMainGameLoop()
{
    GameEngine gameEngine;
    vector<Player *> list_players;

    // fixes incompatibility with cin and getline
    cin.ignore(256, '\n');

    // Test: load map
    const shared_ptr<Map> gameMap = MapLoader::loadMap("../maps/world.map");
    const auto territories = Map::getAllTerritories(*gameMap);

    // Test: add list_players and add territories to them
    auto *p1 = new Player(1, "p1");
    auto *p2 = new Player(2, "p2");
    auto *p3 = new Player(3, "p3");
    auto *p4 = new Player(4, "p4");
    auto *p5 = new Player(5, "p5");
    list_players.push_back(p1);
    list_players.push_back(p2);
    list_players.push_back(p3);
    list_players.push_back(p4);
    list_players.push_back(p5);

    // Test: split territories between p1, p2, p3, and p4 (10%, 20%, 10%, 60%)
    auto size = static_cast<double>(territories.size());
    cout << size << " total territories" << endl;
    for (int i = 0; i < size; i++)
    {
        auto *territory = &*territories[i];
        if (i < size * 0.1)
        {
            p1->addTerritory(territory);
        }
        else if (i < size * 0.3)
        {
            p2->addTerritory(territory);
        }
        else if (i < size * 0.4)
        {
            p3->addTerritory(territory);
        }
        else
        {
            p4->addTerritory(territory);
        }
    }

    // Test: print number of territories for each player
    cout << "p1: " << p1->getTerritories().size() << " territories owned."
         << endl;
    cout << "p2: " << p2->getTerritories().size() << " territories owned."
         << endl;
    cout << "p3: " << p3->getTerritories().size() << " territories owned."
         << endl;
    cout << "p4: " << p4->getTerritories().size() << " territories owned."
         << endl;
    cout << "p5: " << p5->getTerritories().size() << " territories owned."
         << endl;

    // Test: move current state to reinforcement phase
    gameEngine.executeCommand("loadmap");
    gameEngine.executeCommand("validate");
    gameEngine.executeCommand("addplayers");
    gameEngine.executeCommand("assigncountries");

    gameEngine.mainGameLoop(list_players, *gameMap);

    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete p5;
    p1 = p2 = p3 = p4 = p5 = nullptr;

    cout << "---------------------------" << endl;
    cout << "Test Game 2 (Win condition)" << endl;
    GameEngine gameEngine2;
    vector<Player *> list_players2;

    // test: win condition
    auto *p6 = new Player(6, "p6");
    auto *p7 = new Player(7, "p7");

    list_players2.push_back(p6);
    list_players2.push_back(p7);

    // Test: give all territories to p6
    for (int i = 0; i < size; i++)
    {
        p6->addTerritory(territories[i].get());
    }

    // Test: move current state to reinforcement phase
    gameEngine2.executeCommand("loadmap");
    gameEngine2.executeCommand("validate");
    gameEngine2.executeCommand("addplayers");
    gameEngine2.executeCommand("assigncountries");

    gameEngine2.mainGameLoop(list_players2, *gameMap);

    delete p6;
    delete p7;
    p6 = p7 = nullptr;
}