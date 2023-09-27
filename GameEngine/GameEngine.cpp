#include <iostream>
#include <string>
#include <memory>
#include "GameEngine.h"

using std::cin;
using std::make_shared;
using std::shared_ptr;
using std::ostream;
using std::string;

/**
 * Command Class Constructor
 */
Command::Command(string action, shared_ptr<State> nextState) {
    this->action = action;
    this->nextState = nextState;
}

/** Copy Constructor */
Command::Command(const Command &other) {
    this->action = other.action;
    this->nextState = other.nextState;
};

/** Assignment Constructor */
Command &Command::operator=(const Command &other) {
    this->action = other.action;
    this->nextState = other.nextState;
    return *this;
};

/** Ostream << operator */
ostream &operator<<(ostream &os, const Command &command) {
    os << "Action: " << command.action << " Next State: " << command.nextState->phase;
    return os;
};


/**
 * State Class Constructor
 */
State::State(string phase) {
    this->phase = phase;
}

/** Copy Constructor */
State::State(const State &other) {
    this->phase = other.phase;
    for (int i = 0; i < 3; i++) {
        this->commands[i] = other.commands[i];
    }
}

/** Assignment Constructor */
State &State::operator=(const State &other) {
    this->phase = other.phase;
    for (int i = 0; i < 3; i++) {
        this->commands[i] = other.commands[i];
    }
    return *this;
}

/** Ostream << operator */
ostream &operator<<(ostream &os, const State &state) {
    os << "Phase: " << state.phase << std::endl;
    for (int i = 0; i < 3; i++) {
        os << "Command " << i << ": " << state.commands[i] << std::endl;
    }
    return os;
};


/**
 * GameEngine Class Constructor
 */
GameEngine::GameEngine() {
    initGame();
}

/** Copy Constructor */
GameEngine::GameEngine(const GameEngine &other) {
    this->currState = other.currState;
}

/** Assignment Constructor */
GameEngine &GameEngine::operator=(const GameEngine &other) {
    this->currState = other.currState;
    return *this;
}

/** Ostream << operator */
ostream &operator<<(ostream &os, const GameEngine &gameEngine) {
    os << "Current State: " << gameEngine.currState->phase;
    return os;
};

string GameEngine::getPhase() {
    return currState->phase;
};

/**
 * Initializes the game by creating the states and commands
 * and assigning them to each other.
 */
void GameEngine::initGame() {
    /*
     *  Create 'Startup' section of the game
     */

    // States
    shared_ptr<State> start = make_shared<State>("start");
    shared_ptr<State> mapLoaded = make_shared<State>("map loaded");
    shared_ptr<State> mapValidated = make_shared<State>("map validated");
    shared_ptr<State> playersAdded = make_shared<State>("players added");

    // Commands
    shared_ptr<Command> loadMapCommand = make_shared<Command>("loadmap", mapLoaded);
    shared_ptr<Command> validateMapCommand = make_shared<Command>("validate", mapValidated);
    shared_ptr<Command> addPlayersCommand = make_shared<Command>("addplayers", playersAdded);

    // Assign commands to states
    start->commands.push_back(loadMapCommand);
    mapLoaded->commands.push_back(loadMapCommand);
    mapLoaded->commands.push_back(validateMapCommand);
    mapValidated->commands.push_back(addPlayersCommand);
    playersAdded->commands.push_back(addPlayersCommand);

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
    shared_ptr<Command> assignCountriesCommand = make_shared<Command>("assigncountries", assignReinforcements);
    shared_ptr<Command> issueOrdersCommand = make_shared<Command>("issueorder", issueOrders);
    shared_ptr<Command> endIssueOrdersCommand = make_shared<Command>("endissueorders", executeOrders);
    shared_ptr<Command> executeOrdersCommand = make_shared<Command>("execorder", executeOrders);
    shared_ptr<Command> endExecuteOrdersCommand = make_shared<Command>("endexecorders", assignReinforcements);
    shared_ptr<Command> winCommand = make_shared<Command>("win", win);
    shared_ptr<Command> endCommand = make_shared<Command>("end", endGame);
    shared_ptr<Command> playAgainCommand = make_shared<Command>("play", start);

    // Assign commands to states
    playersAdded->commands.push_back(assignCountriesCommand);
    assignReinforcements->commands.push_back(issueOrdersCommand);
    issueOrders->commands.push_back(issueOrdersCommand);
    issueOrders->commands.push_back(endIssueOrdersCommand);
    executeOrders->commands.push_back(executeOrdersCommand);
    executeOrders->commands.push_back(endExecuteOrdersCommand);
    executeOrders->commands.push_back(winCommand);
    win->commands.push_back(endCommand);
    win->commands.push_back(playAgainCommand);

    // Set current state to start
    currState = start;
}

/**
 * Returns a string of the current available commands
 * in the currState for the console to display
 */
string GameEngine::getCurrCommandsList() {
    string commandList = "  ";
    int index = 1;

    // Iterate through the commands in the currState
    for (const auto &command: currState->commands) {
        if (!command->action.empty()) {
            commandList += std::to_string(index) + "." + command->action + " ";
        }
        index++;
    }

    return commandList;
}

/*
 * Executes the command and returns the phase of the next state
 */
string GameEngine::executeCommand(string input) {
    bool commandExecuted = false;

    for (int i = 0; i < currState->commands.size(); i++) {
        if (input == currState->commands[i]->action ||
            (input == std::to_string(i + 1) && !currState->commands[i]->action.empty())) {
            currState = currState->commands[i]->nextState;
            commandExecuted = true;
            break; // Exit loop
        }
    }

    if (!commandExecuted) {
        std::cout << "Invalid command. Try again." << std::endl;
    }

    return currState->phase;
}
