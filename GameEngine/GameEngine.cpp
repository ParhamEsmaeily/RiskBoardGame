#include <iostream>
#include <string>
#include <memory>
#include "GameEngine.h"

using namespace std;

Command::Command(string action, shared_ptr<State> nextState)
{
    this->action = action;
    this->nextState = nextState;
};

State::State(string phase)
{
    this->phase = phase;
};

GameEngine::GameEngine()
{
    initGame();
}

string GameEngine::getPhase()
{
    return currState->phase;
};

void GameEngine::initGame()
{
    /*
     *  Create 'Startup' section of the game
     */

    // States
    shared_ptr<State> start = make_shared<State>("start");
    shared_ptr<State> mapLoaded = make_shared<State>("map loaded");
    shared_ptr<State> mapValidated = make_shared<State>("map validated");
    shared_ptr<State> playersAdded = make_shared<State>("players added");

    // Commands
    Command loadMapCommand = Command("loadmap", mapLoaded);
    Command validateMapCommand = Command("validate", mapValidated);
    Command addPlayersCommand = Command("addplayers", playersAdded);

    // Assign commands to states
    start->commands[0] = loadMapCommand;
    mapLoaded->commands[0] = loadMapCommand;
    mapLoaded->commands[1] = validateMapCommand;
    mapValidated->commands[0] = addPlayersCommand;
    playersAdded->commands[0] = addPlayersCommand;

    /*
     * Create 'play' section of the game
     */

    // States
    shared_ptr<State> assignReinforcements = make_shared<State>("assign reinforcements");
    shared_ptr<State> issueOrders = make_shared<State>("issue orders");
    shared_ptr<State> executeOrders = make_shared<State>("execute orders");
    shared_ptr<State> win = make_shared<State>("win");
    shared_ptr<State> endGame = make_shared<State>("end");

    // Commands
    Command assignCountriesCommand = Command("assigncountries", assignReinforcements);
    Command issueOrdersCommand = Command("issueorder", issueOrders);
    Command endIssueOrdersCommand = Command("endissueorders", executeOrders);
    Command executeOrdersCommand = Command("execorder", executeOrders);
    Command endExecuteOrdersCommand = Command("endexecorders", assignReinforcements);
    Command winCommand = Command("win", win);
    Command endCommand = Command("end", endGame);
    Command playAgainCommand = Command("play", start);

    // Assign commands to states
    playersAdded->commands[1] = assignCountriesCommand;
    assignReinforcements->commands[0] = issueOrdersCommand;
    issueOrders->commands[0] = issueOrdersCommand;
    issueOrders->commands[1] = endIssueOrdersCommand;
    executeOrders->commands[0] = executeOrdersCommand;
    executeOrders->commands[1] = endExecuteOrdersCommand;
    executeOrders->commands[2] = winCommand;
    win->commands[0] = endCommand;
    win->commands[1] = playAgainCommand;

    // Set current state to start
    currState = start;
}

/*
 * Returns a string of the current available commands
 */
string GameEngine::getCurrCommandsList()
{
    string commandList = "  ";
    int arraySize = sizeof(currState->commands) / sizeof(currState->commands[0]);

    for (int i = 0; i < arraySize; i++)
    {
        if (!currState->commands[i].action.empty())
        {
            commandList += to_string(i + 1) + "." + currState->commands[i].action + " ";
        }
    }

    return commandList;
}

/*
 * Executes the command and returns the phase of the next state
 */
string GameEngine::executeCommand(string input)
{
    bool commandExecuted = false;
    int arraySize = sizeof(currState->commands) / sizeof(currState->commands[0]);

    for (int i = 0; i < arraySize; i++)
    {
        if (input == currState->commands[i].action ||
            (input == to_string(i + 1) && !currState->commands[i].action.empty()))
        {
            currState = currState->commands[i].nextState;
            commandExecuted = true;
            break; // Exit loop
        }
    }

    if (!commandExecuted)
    {
        cout << "Invalid command. Try again." << endl;
    }

    return currState->phase;
}
