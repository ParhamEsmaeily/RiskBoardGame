#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class State; // Forward declaration

class Command
{
public:
    string action;
    shared_ptr<State> nextState;
    Command()= default;
    Command(string action, shared_ptr<State> nextState);

    Command(Command const &other);
    Command &operator=(const Command &other);
    friend ostream &operator<<(ostream &os, const Command &command);
};

class State
{
public:
    string phase;
    vector<shared_ptr<Command>> commands;
    explicit State(string phase);

    State(State const &other);
    State &operator=(const State &other);
    friend ostream &operator<<(ostream &os, const State &state);
};

class GameEngine
{
    shared_ptr<State> currState;
    void initGame();

public:
    string getCurrCommandsList();
    string executeCommand(string input);
    string getPhase();

    GameEngine();
    GameEngine(GameEngine const &other);
    GameEngine &operator=(const GameEngine &other);
    friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);
};
