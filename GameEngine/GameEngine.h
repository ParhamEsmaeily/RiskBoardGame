#pragma once

#include <iostream>
#include <string>
#include <memory>

using namespace std;

class State; // Forward declaration

class Command
{
public:
    string action;
    shared_ptr<State> nextState;
    Command(){};
    Command(string action, shared_ptr<State> nextState);
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
    shared_ptr<State> currState;
    void initGame();

public:
    GameEngine();
    string getCurrCommandsList();
    string executeCommand(string input);
    string getPhase();
};
