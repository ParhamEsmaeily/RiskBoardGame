#include "GameEngine.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include <string>
#include "Command.h"
#include "Player.h"

using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;

/**
 * GameEngine Class Constructor
 */
GameEngine::GameEngine() { 
  map = new Map();

  initGame(); 
  }

/** Copy Constructor */
GameEngine::GameEngine(const GameEngine &other) {
  this->currState = other.currState;
}

GameEngine::~GameEngine()
{
  delete map;
  map = nullptr;
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

//void GameEngine::startupPhase(CommandProcessor* cmdProcessor)
void GameEngine::startupPhase() {
    bool startupPhaselogic = false;
    bool mapLoaded = false;
    bool mapValidated = false;
    bool playersAdded = false;
    int playernum;
    Command currentCommand;
    string commandType;
    std::cout<<"entered\n";
    


    while (!startupPhaselogic) {
      std::cout<<"entered while loop\n";
      std::cout << "\nEnter next command: "<<std::endl;
      std::cin>>commandType;


       string commandAction =commandType;


       std::cout<<"entered command type: \n"<<commandAction<<"\n";

       std::cout<<"maploaded outside"<<mapLoaded<<"\n";

        if (commandAction == "loadmap" && !mapLoaded) {
            // Logic to load the map
            std::cout << "\n<<Loading the map...\n " <<std::endl;
            // delete previous map if any

            // load the new map
            string map_path = "../maps/world.map";
            //std::shared_ptr<Map> map = MapLoader::loadMap(map_path);
            const auto map_shared_ptr = MapLoader::loadMap(map_path);
            this->map = map_shared_ptr.get();

            std::cout<<"entered6\n";
            mapLoaded = true;
            std::cout<<mapLoaded<<"\n";
            Map::validate(map);


            // if (this->map = nullptr) {//if null pointer
            //     string effect = "Could not load the map ";
            //     command->saveEffect(effect);
            //     //go_to_next_state = false;
            //     std::cout<<"entered4\n";
            // } else {
            //     string effect = "Loaded map";
            //     command->saveEffect(effect);
            //     std::cout<<"entered5\n";
            // }

        } 

        else if (commandAction == "validatemap"  && mapLoaded) {

          

          //&& this->map->getValidity() == MapValidity::VALID
            // Logic to validate the map
            // string effect = "Validated Map";
            //     command->saveEffect(effect);

            std::cout<<"mapvalidate  before\n";
            mapValidated = true;
            std::cout<<"mapvalidate  true\n";


        } else if (commandAction  == "addplayer" && mapValidated && !playersAdded) {

          std::cout<<"choose number of players"<<std::endl;
          std::cin>>playernum;

            // Logic to add players
            if (playernum <=6 && playernum >=2) {
              for(int i = 1; i<=playernum; i++){

                //set player name and ID
                std::string playerName = "player" + std::to_string(i);
                //Player p = Player(i, playerName);
                players.push_back(new Player (i, playerName));

                std::cout<< "adding playerID: "<<i<<" & name: player"<<i<<std::endl;
              }
              // for(auto&& player: players){
              //   std::cout<<*player<<std::endl;
              // }
              // string effect = "Validated Map";
              //   command->saveEffect(effect);

                std::cout<<"Hello";
                playersAdded = true;
                
            }else{
              std::cout<< "wrong number of players. choose between 2 and 6"<<std::endl;
            }
        } else if (commandAction == "gamestart" && playersAdded) {
            // Logic to start the game
            // a) Distribute territories
            std::cout<<"\nEntering game start\n";
            const auto territories = Map::getAllTerritories(*map);
            std::cout<<"\nafter getAllTER\n";

            auto size = static_cast<double>(territories.size());
            cout << size << " total territories" << endl;
            for (int i = 0; i < size; i++)
           {   
            std::cout<<"\nEntering forLOOP ````game start\n";
            auto *territory = &*territories[i];
            int playerIndex = i % playernum; // Round-robin distribution
            // Assuming Player has a method to add a territory
             players[playerIndex]->addTerritory(territory);
              
           }
           std::cout<<"trying to return player 1 ter\n";
           players[1]->getTerritories();
            std::cout<<"\ndid I return player 1 ter\n";
            // b) Randomize order of play
            std::cout << "\nRandomizing player order\n";
            std::random_device rd;  // Obtain a random number from hardware
            std::mt19937 g(rd());   // Seed the generator
            std::shuffle(players.begin(), players.end(), g);  // Shuffle the players vector

            //PRINT
            std::cout << "Order of play:" << std::endl;
            for (const auto& player : players) {
            // Assuming each player has a method to get their name or ID, such as getName() or getID()
            std::cout << player->getName() << std::endl;  // Replace getName() with the appropriate method
}
            // c) Assign initial armies
            // d) Deal initial cards
            // e) Switch to play phase
            startupPhaselogic = true;
            break; // Assuming the loop should end when the game starts

        } else {
            // Handle invalid commands or commands in the wrong order
            std::cout<<"Invalid command or command received in the wrong state.";
            startupPhaselogic = true;
        }
    }



}
