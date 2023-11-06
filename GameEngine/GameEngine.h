#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Player.h"

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();

class State; // Forward declaration

class Command
{
public:
    shared_ptr<string> action;
    shared_ptr<State> nextState;
    Command() = default;
    Command(string action, shared_ptr<State> nextState);

    Command(Command const &other);
    Command &operator=(const Command &other);
    friend ostream &operator<<(ostream &os, const Command &command);
};

class State
{
public:
    shared_ptr<string> phase;
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
    static void reinforcementPhase(vector<Player *> players, shared_ptr<Map> map);
    void issueOrdersPhase();
    void executeOrdersPhase();

public:
    string getCurrCommandsList();
    string executeCommand(string input);
    string getPhase();
    void mainGameLoop(); // move to private after testing

    GameEngine();
    GameEngine(GameEngine const &other);
    GameEngine &operator=(const GameEngine &other);
    friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);
};
