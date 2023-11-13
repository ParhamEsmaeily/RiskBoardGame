#pragma once

#include "../Cards/Cards.h"
#include "../CommandProcessor/Command.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../LoggingObserver/LoggingObserver.h"
#include "../Player/Player.h"

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();
void testMainGameLoop();
void testStartupPhase();

// class State; // Forward declaration
class CommandProcessor;
class Map;

class GameEngine : private ILoggable, private Subject {
  shared_ptr<State> currState;
  void initGame();
  void reinforcementPhase(vector<Player *> players, const Map &map);
  void issueOrdersPhase(vector<Player *> players, const Map &map);
  void executeOrdersPhase(vector<Player *> players);

public:
  string getCurrCommandsList();
  string executeCommand(string input);
  string getPhase();
  void phase(std::string) noexcept;
  

  void startupPhase();
  Command *command;
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

  void mainGameLoop(vector<Player *> players, const Map &map);
};