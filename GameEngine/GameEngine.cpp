#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "GameEngine.h"
#include "Player.h"

using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;

/**
 * Command Class Constructor
 */
Command::Command(string action, shared_ptr<State> nextState) {
  this->action = make_shared<string>(action);
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
}
void Command::saveEffect(const std::string &effect) {
  this->effect = make_shared<string>(effect);
  Notify(this);
};

std::string Command::stringToLog() const {
  return "Command stringToLog: " + *effect;
}

/** Ostream << operator */
ostream &operator<<(ostream &os, const Command &command) {
  os << "Action: " << *command.action
     << ", Next State: " << *command.nextState->phase;
  return os;
};

/**
 * State Class Constructor
 */
State::State(string phase) { this->phase = make_shared<string>(phase); }

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
    os << "Command " << i << ": " << *state.commands[i] << std::endl;
  }
  return os;
};

/**
 * GameEngine Class Constructor
 */
GameEngine::GameEngine() { initGame(); }

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
  os << "Current State: " << *gameEngine.currState->phase;
  return os;
};

string GameEngine::getPhase() { return *currState->phase; };

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
  shared_ptr<Command> loadMapCommand =
      make_shared<Command>("loadmap", mapLoaded);
  shared_ptr<Command> validateMapCommand =
      make_shared<Command>("validate", mapValidated);
  shared_ptr<Command> addPlayersCommand =
      make_shared<Command>("addplayers", playersAdded);

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
  shared_ptr<State> assignReinforcements =
      make_shared<State>("assign reinforcements");
  shared_ptr<State> issueOrders = make_shared<State>("issue orders");
  shared_ptr<State> executeOrders = make_shared<State>("execute orders");
  shared_ptr<State> win = make_shared<State>("win");
  shared_ptr<State> endGame = make_shared<State>("end");

  // Commands
  shared_ptr<Command> assignCountriesCommand =
      make_shared<Command>("assigncountries", assignReinforcements);
  shared_ptr<Command> issueOrdersCommand =
      make_shared<Command>("issueorder", issueOrders);
  shared_ptr<Command> endIssueOrdersCommand =
      make_shared<Command>("endissueorders", executeOrders);
  shared_ptr<Command> executeOrdersCommand =
      make_shared<Command>("execorder", executeOrders);
  shared_ptr<Command> endExecuteOrdersCommand =
      make_shared<Command>("endexecorders", assignReinforcements);
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
  for (const auto &command : currState->commands) {
    if (!command->action->empty()) {
      commandList += std::to_string(index) + "." + *command->action + " ";
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
    if (input == *currState->commands[i]->action ||
        (input == std::to_string(i + 1) &&
         !currState->commands[i]->action->empty())) {
      currState = currState->commands[i]->nextState;
      commandExecuted = true;
      break; // Exit loop
    }
  }

  if (!commandExecuted) {
    std::cout << "Invalid command. Try again." << std::endl;
  }

  return *currState->phase;
}

std::string GameEngine::stringToLog() const {
  // Returns the current state of the game engine.
  return "GameEngine stringToLog: Phase " + *currState->phase;
}

/*
 * Part 3: Main Game Loop
 */

void GameEngine::reinforcementPhase(vector<Player *> players, const Map &gameMap) {
    cout << "Reinforcement Phase Starting" << endl;

    const auto continents = Map::getAllContinents(gameMap);

    // for each player, give reinforcements based on territories owned
    for (auto && player : players) {
        int continent_bonus = 0;

        // 1 army for every 3 territories owned rounded down
        int territory_reinforcement_count = static_cast<int>(player->getTerritories().size() / 3);

        // map of number of territories owned per continent: <continent name, number of territories owned>
        map<string, int> num_terr_per_continent;

        // count number of territories owned per continent for this player
        for (auto &&territory : player->getTerritories())
        {
            string territoryContinent = territory->getContinent()->getName();
            bool missing_from_map = num_terr_per_continent.find(territoryContinent) == num_terr_per_continent.end();

            if (missing_from_map) {
                num_terr_per_continent[territoryContinent] = 1;
            } else {
                num_terr_per_continent[territoryContinent]++;
            }
        }

        // check if player owns all territories in a continent by matching the number of territories owned per continent
        // with the total number of territories in that continent
        for (auto &&continent : continents) {
            if (num_terr_per_continent.find(continent->getName()) != num_terr_per_continent.end()) {
                if (num_terr_per_continent[continent->getName()] == continent->getTerritoryCount()) {
                    std::cout << "Player " << player->getName() << " owns all territories in " << continent->getName()
                              << std::endl;
                    continent_bonus += continent->getBonus();
                }
            }
        }

        int reinforcements = territory_reinforcement_count + continent_bonus;

        // minimum of 3 reinforcements
        if(reinforcements < 3) { reinforcements = 3; }

        cout << player->getName() << " gets " << reinforcements << " reinforcements: "
        << territory_reinforcement_count << " from territories and "
        << continent_bonus << " from continent bonuses." << endl;

        // give player reinforcements
        for (int i = 0; i < reinforcements; i++) {
            player->getHand()->insert(Card(card_type::reinforcement));
        }
    }
    cout << "Reinforcement Phase End" << endl;
}

void GameEngine::issueOrdersPhase(vector<Player *> players, const Map &gameMap) {
    cout << "Issue Orders Phase Starting" << endl;

    // for each player, issue orders
    for (auto && player : players) {
        cout << "Player " << player->getName() << " issuing orders" << endl;
        player->issueOrder(gameMap);
    }

    cout << "Issue Orders Phase End" << endl;
}

void GameEngine::executeOrdersPhase(vector<Player *> players) {
    cout << "Execute Orders Phase Starting" << endl;

    // while all players still have orders left
    size_t players_with_deploys_left = players.size();
    while (true) {
        bool all_players_have_no_orders = true;

        // for each player, check if they have orders left
        for (auto && player : players) {
            if (!player->getPlayerOrderList()->list.empty()) {
                all_players_have_no_orders = false;
                break;
            }
        }
        // if all players have no orders left, break out of loop
        if (all_players_have_no_orders) {
            break;
        }

        // for each player, execute orders
        for (auto && player : players) {
            // execute deploy orders first, then when all players have no deploy orders left, execute the rest
            if (!player->getPlayerOrderList()->list.empty() &&
                (player->getPlayerOrderList()->list[0]->name == "Deploy" || players_with_deploys_left <= 0)) {
                cout << "Player " << player->getName() << " executing orders" << endl;

                player->getPlayerOrderList()->list[0]->execute();
                player->getPlayerOrderList()->remove(0);
            }
            else if (players_with_deploys_left > 0){
                players_with_deploys_left--;
            }
        }
    }

    cout << "Execute Orders Phase End" << endl;
}

void GameEngine::mainGameLoop(vector<Player *> players, const Map &gameMap) {
    // Check if gamestart was called
    if(getPhase() != "assign reinforcements")
    {
        cout << "Error: Game start has not been called" << endl;
        return;
    }

    while(getPhase() != "end") {
        // check if a player has no territories (delete function because players don't start with 0 territories)
        for (auto && player : players) {
            if(player->getTerritories().empty()) {
                cout << player->getName() << " has no territories left. Player will be removed." << endl;
                // remove player from list of players
                players.erase(std::remove(players.begin(), players.end(), player), players.end());
                break;
            }
        }

        reinforcementPhase(players, gameMap);
        executeCommand("issueorder");

        issueOrdersPhase(players, gameMap);
        executeCommand("endissueorders");

        executeOrdersPhase(players);

        // check if a player has no territories
        for (auto && player : players) {
            if(player->getTerritories().empty()) {
                cout << player->getName() << " has no territories left. Player will be removed." << endl;
                // remove player from list of players
                players.erase(std::remove(players.begin(), players.end(), player), players.end());
                break;
            }
        }

        // check if there is only one player left
        if(players.size() == 1) {
            cout << players[0]->getName() << " has won the game!" << endl;
            // must be in execute orders state to win
            executeCommand("win");
            return;
        }

        executeCommand("endexecorders");

        // Stops after 1 loop (to delete)
        break;
    }
}

