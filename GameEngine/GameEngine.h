#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>

using namespace std;

class State; // Forward declaration

class Command
{
public:
    string action;
    State *nextState;
    Command(){};
    Command(string action, State *nextState);
};

class State
{
public:
    string phase;
    Command commands[3];
    State(string phase);
};

class GameEngine
{
    State *currState;
    void initGame();

public:
    GameEngine();
    string getCurrCommandsList();
    string executeCommand(string input);
    string getPhase();
};

#endif // GAMEENGINE_H
