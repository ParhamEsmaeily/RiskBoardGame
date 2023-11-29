#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Cards.h"
#include "Map.h"
#include "Command.h"
#include "LoggingObserver.h"
#include "Player.h"

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();
void testMainGameLoop();
void testStartupPhase();
void testTournament();

// class State; // Forward declaration
class CommandProcessor;
class Map;

class GameEngine : private ILoggable, private Subject
{
  shared_ptr<State> currState;
  void initGame();
  void reinforcementPhase(vector<Player *> players, const Map &map);
  void issueOrdersPhase(vector<Player *> players, const Map &map);
  void executeOrdersPhase(vector<Player *> players);
  void startTournament(vector<std::string> mapList, vector<std::string> playerList, int numGames, int numTurns);

public:
  string getCurrCommandsList();
  string executeCommand(string input);
  string getPhase();
  void phase(std::string) noexcept;

  void startupPhase();
  vector<Player *> players;
  std::string stringToLog() const override;
  std::shared_ptr<Map> map;
  Deck *deck;

  CommandProcessor *commandProcessor;

  GameEngine();
  GameEngine(GameEngine const &other);
  ~GameEngine();

  GameEngine &operator=(const GameEngine &other);
  friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);

  void initiateTournament();
  string mainGameLoop(vector<Player *> players, const Map &gameMap, int numTurns = -1);
};