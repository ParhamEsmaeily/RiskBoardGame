#include <map>

#include "GameEngine.h"
#include "PlayerStrategies.h"

using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;

/**
 * GameEngine Class Constructor
 */
GameEngine::GameEngine() { initGame(); }

/** Copy Constructor */
GameEngine::GameEngine(const GameEngine &other)
{
  this->currState = other.currState;
}

GameEngine::~GameEngine() {}

GameEngine &GameEngine::operator=(const GameEngine &other)
{
  this->currState = other.currState;
  return *this;
}

/** Ostream << operator */
ostream &operator<<(ostream &os, const GameEngine &gameEngine)
{
  os << "Current State: " << *gameEngine.currState->phase;
  return os;
};

string GameEngine::getPhase() { return *currState->phase; };

/**
 * Initializes the game by creating the states and commands
 * and assigning them to each other.
 */
void GameEngine::initGame()
{
  /*
   *  Create 'Startup' section of the game
   */
  // Assume it isn't a tournament, dealt with after if it is and it is validated
  this->isTournament = false;
  this->tournament_log = "";
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
string GameEngine::getCurrCommandsList()
{
  string commandList = "  ";
  int index = 1;

  // Iterate through the commands in the currState
  for (const auto &command : currState->commands)
  {
    if (!command->action->empty())
    {
      commandList += std::to_string(index) + "." + *command->action + " ";
    }
    index++;
  }

  return commandList;
}

/*
 * Executes the command and returns the phase of the next state
 */
string GameEngine::executeCommand(string input)
{
  bool commandExecuted = false;

  for (int i = 0; i < currState->commands.size(); i++)
  {
    if (input == *currState->commands[i]->action ||
        (input == std::to_string(i + 1) &&
         !currState->commands[i]->action->empty()))
    {
      currState = currState->commands[i]->nextState;
      commandExecuted = true;
      break; // Exit loop
    }
  }

  if (!commandExecuted)
  {
    std::cout << "Invalid command. Try again." << std::endl;
  }

  return *currState->phase;
}

std::string GameEngine::stringToLog() const
{
  if (this->isTournament)
    return this->tournament_log;

  // Returns the current state of the game engine.
  return "GameEngine stringToLog: Phase " + *currState->phase;
}

void GameEngine::phase(std::string phase) noexcept
{
  *currState->phase = std::move(phase);
  Notify(this);
}

bool validateStringCount(const std::string &input, std::vector<std::string> &result, int minCount, int maxCount)
{
  std::istringstream iss(input);
  std::string token;
  if (input.empty())
  {
    std::cerr << "No input provided." << std::endl;
    return false;
  }
  result.clear();
  while (std::getline(iss, token, ','))
  {
    result.push_back(token);
  }
  if (result.size() < minCount || result.size() > maxCount)
  {
    std::cerr << "Invalid number of strings separated by commas." << std::endl;
    return false;
  }

  return true;
}

bool validateNumber(int &result, int min, int max)
{
  std::cin >> result;

  if (std::cin.fail() || result < min || result > max)
  {
    std::cin.clear(); // Clear the fail state
    std::cin.ignore(1000, '\n');
    std::cerr << "Invalid input." << std::endl;
    return false;
  }

  std::cin.ignore(1000, '\n'); // Clear the input buffer
  return true;
}

void GameEngine::initiateTournament()
{
  // Ask the list of maps, list of players, number of games per map, number of turns per game
  std::string maplist, playerlist;
  int numgames, numturns;
  std::vector<std::string> mapListVector;
  std::vector<std::string> playerListVector;
  this->isTournament = true;

  do
  {
    std::cout << "Enter the list of maps (M, 1-5):" << std::endl;
    std::cin >> maplist;
    std::cin.ignore(1000, '\n');
  } while (!validateStringCount(maplist, mapListVector, 1, 5));

  do
  {
    std::cout << "Enter the list of players (P, 2-4):" << std::endl;
    std::cin >> playerlist;
    std::cin.ignore(1000, '\n');
  } while (!validateStringCount(playerlist, playerListVector, 2, 4));

  do
  {
    std::cout << "Enter the number of games per map (G, 1-5):" << std::endl;
  } while (!validateNumber(numgames, 1, 5));

  do
  {
    std::cout << "Enter the number of turns per game (D, 10-50):" << std::endl;
  } while (!validateNumber(numturns, 10, 50));

  // Call the tournament method
  GameEngine::startTournament(mapListVector, playerListVector, numgames, numturns);
}

// void GameEngine::startupPhase(CommandProcessor* cmdProcessor)
void GameEngine::startupPhase()
{
  bool startupPhaselogic = false;
  bool mapLoaded = false;
  bool mapValidated = false;
  bool playersAdded = false;
  int playernum;
  Command *currentCommand = new Command();
  string commandType;

  while (!startupPhaselogic)
  {
    std::cout << "\nEnter next command: " << std::endl;
    std::cin >> commandType;
    std::cin.ignore(1000, '\n');

    string commandAction = commandType;

    if (commandAction == "tournament")
    {
      initiateTournament();
    }
    else if (commandAction == "loadmap" && !mapLoaded)
    {
      string choice;
      // Logic to load the map
      std::cout << "\n<<Loading the map...\n " << std::endl;

      std::cout << "\n<<two map choices:\n world.map & test.map" << std::endl;

      std::cout << "\n<<Choose your choice make sure to have correct syntax\n " << std::endl;

      std::cin >> choice;
      // delete previous map if any

      // load the new map
      string map_path = "maps/" + choice;
      this->map = MapLoader::loadMap(map_path);

      mapLoaded = true;

      std::cout << "\n<<You may \"validatemap\"\n if loaded correctly" << std::endl;
    }

    else if (commandAction == "validatemap" && mapLoaded)
    {
      mapValidated = true;

      Map::validate(map.get());
      std::cout << "\nMap has been Validated, now you may proceed to sue \"addplayer\"\n"
                << std::endl;
    }
    else if (commandAction == "addplayer" && mapValidated && !playersAdded)
    {

      std::cout << "\nchoose number of players" << std::endl;
      std::cin >> playernum;

      // Logic to add players
      if (playernum <= 6 && playernum >= 2)
      {
        for (int i = 1; i <= playernum; i++)
        {

          // set player name and ID
          std::string playerName = "player" + std::to_string(i);
          // Player p = Player(i, playerName);
          players.push_back(new Player(i, playerName));

          std::cout << "adding playerID: " << i << " & name: player" << i
                    << std::endl;
        }

        playersAdded = true;
        std::cout << "\n<<Players have been added to game\n\n You may \"gamestart\"\n " << std::endl;
      }
      else
      {
        std::cout << "wrong number of players. choose between 2 and 6"
                  << std::endl;
      }
    }
    else if (commandAction == "gamestart" && playersAdded)
    {
      // Logic to start the game
      // a) Distribute territories
      const auto territories = Map::getAllTerritories(*map);

      auto size = static_cast<double>(territories.size());
      cout << size << " total territories" << endl;
      for (int i = 0; i < size; i++)
      {
        Territory *territory = &*territories[i];
        int playerIndex = i % playernum; // Round-robin distribution
        players[playerIndex]->addTerritory(territory);
      }
      for (int j = 0; j < playernum; j++)
      {
        std::cout << "Printing player: " << j + 1 << " territories:\n";
        for (Territory *t : players[j]->getTerritories())
        {
          cout << *t << endl;
        }
      }

      // players[1]->getTerritories();
      //  b) Randomize order of play
      std::cout << "\nRandomizing player order\n==========================================================\n";
      std::random_device rd; // Obtain a random number from hardware
      std::mt19937 g(rd());  // Seed the generator
      std::shuffle(players.begin(), players.end(),
                   g); // Shuffle the players vector

      // PRINT
      std::cout << "Order of play:" << std::endl;
      for (const auto &player : players)
      {
        std::cout << player->getName() << std::endl;
      }
      // c) Assign initial armies
      std::cout << "\n==========================================================\nAssigning 50 initial armies to each player...\n";
      for (auto &player : players)
      {
        for (int k = 0; k < 50; k++)
        {
          player->getHand()->insert(Card(CardType::reinforcement));
        }
      }

      // d) Deal initial cards
      std::cout << "==========================================================\nDealing initial cards to each player...\n";
      Deck deck;
      deck.random_insert(100);
      for (auto &player : players)
      {
        std::cout << "\nplayer ";
        std::cout << player->getPlayerId() << std::endl;
        // std::cout << "\nhand";
        // std::cout << player->getHand()<< std::endl;

        // Each player draws two cards
        deck.draw(*player->getHand()); // First card
        deck.draw(*player->getHand()); // Second card

        // std::cout <<  player->getHand()->show_cards().size() << std::endl;

        std::cout << "\nhand card 1 that should be reinforcement: \n";
        std::cout << player->getHand()->show_cards()[1] << std::endl;
        std::cout << "\ndraw card  #1: \n";
        std::cout << player->getHand()->show_cards()[50] << std::endl; // because we add 50 initial reinforcement cards
        std::cout << "\ndraw card  #2: \n";
        std::cout << player->getHand()->show_cards()[51] << std::endl;
      }
      // e) Switch to play phase
      cout << "\ne) switching the game to the play phase: " << endl;
      this->phase("play");

      std::cout << "\nGameState is: ";
      std::cout << this->getPhase() << std::endl;

      currentCommand->saveEffect("\na)distributing all the territories to the players\n"
                                 "b) determining order of play of randomly\n"
                                 "c) giving 50 initial army units to the players\n"
                                 "d) players draw 2 initial cards from the deck using "
                                 "the deck's draw() method\n"
                                 "e) switch the game to the play phase");

      startupPhaselogic = true;
      break; // Assuming the loop should end when the game starts
    }
    else
    {
      // Handle invalid commands or commands in the wrong order
      std::cout << "Invalid command or command received in the wrong state.";
      startupPhaselogic = true;
    }
  }

  delete currentCommand;
  currentCommand = nullptr;
}

/*
 * Part 3: Main Game Loop
 */

void GameEngine::reinforcementPhase(vector<Player *> players, const Map &gameMap)
{
  cout << "Reinforcement Phase Starting" << endl;

  const auto continents = Map::getAllContinents(gameMap);

  // for each player, give reinforcements based on territories owned
  for (auto &&player : players)
  {
    int continent_bonus = 0;

    // 1 army for every 3 territories owned rounded down
    int territory_reinforcement_count = static_cast<int>(player->getTerritories().size() / 3);

    // map of number of territories owned per continent: <continent name, number of territories owned>
    ::map<string, int> num_terr_per_continent;

    // count number of territories owned per continent for this player
    for (auto &&territory : player->getTerritories())
    {
      string territoryContinent = territory->getContinent()->getName();
      bool missing_from_map = num_terr_per_continent.find(territoryContinent) == num_terr_per_continent.end();

      if (missing_from_map)
      {
        num_terr_per_continent[territoryContinent] = 1;
      }
      else
      {
        num_terr_per_continent[territoryContinent]++;
      }
    }

    // check if player owns all territories in a continent by matching the number of territories owned per continent
    // with the total number of territories in that continent
    for (auto &&continent : continents)
    {
      if (num_terr_per_continent.find(continent->getName()) != num_terr_per_continent.end())
      {
        if (num_terr_per_continent[continent->getName()] == continent->getTerritoryCount())
        {
          std::cout << "Player " << player->getName() << " owns all territories in " << continent->getName()
                    << std::endl;
          continent_bonus += continent->getBonus();
        }
      }
    }

    int reinforcements = territory_reinforcement_count + continent_bonus;

    // minimum of 3 reinforcements
    if (reinforcements < 3)
    {
      reinforcements = 3;
    }

    cout << player->getName() << " gets " << reinforcements << " reinforcements: "
         << territory_reinforcement_count << " from territories and "
         << continent_bonus << " from continent bonuses." << endl;

    // give player reinforcements
    for (int i = 0; i < reinforcements; i++)
    {
      player->getHand()->insert(Card(CardType::reinforcement));
    }
    player->getHand()->random_insert(1); // give a random card
  }
  cout << "Reinforcement Phase End" << endl;
}

void GameEngine::issueOrdersPhase(vector<Player *> players, const Map &gameMap)
{
  cout << "Issue Orders Phase Starting" << endl;

  // for each player, issue orders
  for (auto &&player : players)
  {
    player->issueOrder(gameMap, players);
    cout << "Player " << player->getName() << " has issued " << player->getPlayerOrderList()->list.size() << " orders" << endl;
  }

  cout << "Issue Orders Phase End" << endl;
}

void GameEngine::executeOrdersPhase(vector<Player *> players)
{
  cout << "Execute Orders Phase Starting" << endl;

  // while all players still have orders left
  size_t players_with_deploys_left = players.size();
  while (true)
  {
    bool all_players_have_no_orders = true;

    // for each player, check if they have orders left
    for (auto &&player : players)
    {
      if (!player->getPlayerOrderList()->list.empty())
      {
        all_players_have_no_orders = false;
        break;
      }
    }
    // if all players have no orders left, break out of loop
    if (all_players_have_no_orders)
    {
      break;
    }

    // for each player, execute orders
    for (auto &&player : players)
    {
      // executes all the deploy orders first
      for (size_t i = 0; i < player->getPlayerOrderList()->list.size(); i++)
      {
        auto order = player->getPlayerOrderList()->list[i];

        if (order->name == "Deploy")
        {
          cout << "Player " << player->getName() << " executing deploy orders" << endl;

          order->execute();

          player->getPlayerOrderList()->remove(i);
        }
      }

      // decrement number of players with deploy orders left
      if (players_with_deploys_left > 0)
        players_with_deploys_left--;

      // execute deploy orders first, then when all players have no deploy orders left, execute the rest
      if (!player->getPlayerOrderList()->list.empty() && players_with_deploys_left <= 0)
      {
        cout << "Player " << player->getName() << " executing orders" << endl;

        player->getPlayerOrderList()->list[0]->execute();
        player->getPlayerOrderList()->remove(0);
      }
    }
  }

  cout << "Execute Orders Phase End" << endl;
}

std::string GameEngine::mainGameLoop(vector<Player *> players, const Map &gameMap, int numTurns)
{
  int currTurns = 0;
  // Check if gamestart was called
  if (getPhase() != "assign reinforcements")
  {
    cout << "Error: Game start has not been called" << endl;
    return "Wrong State";
  }

  while (getPhase() != "end")
  {
    // check if a player has no territories (delete function because players don't start with 0 territories)
    for (auto &&player : players)
    {
      if (player->getTerritories().empty())
      {
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
    for (auto &&player : players)
    {
      if (player->getTerritories().empty())
      {
        cout << player->getName() << " has no territories left. Player will be removed." << endl;
        // remove player from list of players
        players.erase(std::remove(players.begin(), players.end(), player), players.end());
        break;
      }
    }

    // check if there is only one player left
    if (players.size() == 1)
    {
      cout << players[0]->getName() << " has won the game!" << endl;
      // must be in execute orders state to win
      executeCommand("win");
      return players[0]->getName();
    }
    if (numTurns != -1 && currTurns >= numTurns)
    {
      // cout << "Reached max number of turns" << endl;
      executeCommand("win");
      return "Draw";
    }

    executeCommand("endexecorders");
    currTurns++;
  }
  return "Draw";
}

void GameEngine::startTournament(std::vector<std::string> mapList, std::vector<std::string> playerList, int numGames, int numTurns)
{
  std::vector<shared_ptr<Map>> mapsInTournament;
  std::vector<Player *> playersInTournament;
  std::string mapsLine = "";
  std::string playersLine = "";

  std::cout << "Starting to validate maps and players.." << std::endl;

  // Check if the maps can be loaded and valid
  for (auto mapstr : mapList)
  {
    // Load the map
    string map_path = "maps/" + mapstr + ".map";
    std::cout << "Loading map: " << map_path << std::endl;
    shared_ptr<Map> loadedMap = MapLoader::loadMap(map_path);
    Map::validate(loadedMap.get());

    // check validation
    if (loadedMap->getValidity() == MapValidity::VALID)
    {
      std::cout << "valid map: " << map_path << std::endl;
      mapsInTournament.push_back(loadedMap);
    }
    else
    {
      std::cout << "invalid map: " << map_path << std::endl;
      return;
    }

    // Game log
    mapsLine += loadedMap->getImage();
    if (mapstr != mapList.back())
    {
      mapsLine += ", ";
    }
  }

  std::cout << "Maps loaded and validated" << std::endl;
  int index = 0;
  // Validate and turn the players into objects
  for (auto playerstr : playerList)
  {
    // lowercase the string
    std::transform(playerstr.begin(), playerstr.end(), playerstr.begin(), ::tolower);

    auto *player = new Player(index, playerstr + std::to_string(index+1));

    // Assign the player strategy
    if (playerstr == "aggressive")
    {
      player->setStrategy(new AggressivePlayer());
    }
    else if (playerstr == "benevolent")
    {
      player->setStrategy(new BenevolentPlayer());
    }
    else if (playerstr == "neutral")
    {
      player->setStrategy(new NeutralPlayer());
    }
    else if (playerstr == "cheater")
    {
      player->setStrategy(new CheaterPlayer());
    }
    else
    {
      throw std::invalid_argument("Invalid player type: " + playerstr);
    }

    playersInTournament.push_back(player);
    index++;

    // Game log
    playersLine += player->getName();
    if(index < playerList.size()) {
      playersLine += ", ";
    }
  }
  std::cout << "Players loaded and validated" << std::endl;
  this->isTournament = true;

  // Game log
  std::string log = "Tournament mode:\nM: " + mapsLine + "\nP: " + playersLine + "\nG: " +
                    std::to_string(numGames) + "\nD: " + std::to_string(numTurns) + "\n\n" + formatForTable("Results:");
  for (int i = 1; i <= numGames; i++) {
    log += formatForTable("Game " + std::to_string(i));
  }

  // Start the tournament
  for (auto map : mapsInTournament)
  {
    this->map = map;
    log += "\n" + formatForTable(map->getImage());
    for (int i = 0; i < numGames; i++)
    {
      executeCommand("loadmap");
      executeCommand("validate");
      executeCommand("addplayers");

      // DEBUG: list players
      /*std::cout << "Players in tournament: " << std::endl;
      for (auto player : playersInTournament)
      {
          std::cout << player->getName() << std::endl;
      }*/

      executeCommand("assigncountries");
      // assign countries to players
      const auto territories = Map::getAllTerritories(*map);
      auto size = static_cast<double>(territories.size());
      cout << size << " total territories" << endl;
      for (int x = 0; x < size; x++)
      {
        Territory *territory = &*territories[x];
        int playerIndex = x % playersInTournament.size();
        playersInTournament[playerIndex]->addTerritory(territory);
      }

      // DEBUG: print all players and their territories
      /*for (int j = 0; j < playersInTournament.size(); j++)
      {
          std::cout << "Printing player: " << j + 1 << " territories:\n";
          for (Territory *t : playersInTournament[j]->getTerritories())
          {
              cout << *t << endl;
          }
      }*/

      // DEBUG: print all players and territory count
//        for (auto player : playersInTournament)
//        {
//            std::cout << "Player: " << player->getName() << " has " << player->getTerritories().size() << " territories" << std::endl;
//        }

      // DEBUG: print all territories and their owner
//        for (const auto& t : territories)
//        {
//            std::cout << "Territory: " << t->getName() << " owned by: " << t->getOwner()->getName() << std::endl;
//        }


      std::cout << "Starting game " << i + 1 << std::endl;
      // Start the game, returns name of player or draw if no winner
      std::string result = mainGameLoop(playersInTournament, *map, numTurns);
      log += formatForTable(result);

      // play again
      executeCommand("play");

      for (auto player : playersInTournament)
      {
            player->resetNewGame();
      }
    }
    log += "\n";
    std::cout << log << std::endl;
  }
  this->tournament_log = log;
  Notify(this);
}

std::string formatForTable(std::string input)
{
  // just so table format can be changed easily
  int maxLength = 12;
  int spacing = 5;
  // this formats all the string going in the tournament table to the same length
  std::string space;
  for (int i = 0; i < spacing; i++)
    space += " ";
  if (input.length() > maxLength)
    return input.substr(0, maxLength) + space;
  for (int j = input.length(); j < maxLength; j++)
    space += " ";
  return (input + space);
}