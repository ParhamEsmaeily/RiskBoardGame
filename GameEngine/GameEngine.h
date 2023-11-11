#pragma once
#include "../LoggingObserver/LoggingObserver.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Command.h"
#include "Map.h"
#include "Player.h"

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();

// class State; // Forward declaration
class CommandProcessor;
class Map;

class GameEngine : private ILoggable, private Subject
{
  shared_ptr<State> currState;
  void initGame();
  // void startupPhase(CommandProcessor* cmdProcessor);

public:
  string getCurrCommandsList();
  string executeCommand(string input);
  string getPhase();
  void startupPhase();
  Command *command;
  vector<Player *> players;
  std::string stringToLog() const override;
  std::shared_ptr<Map> map;

  CommandProcessor *commandProcessor;
  GameEngine();
  GameEngine(GameEngine const &other);
  ~GameEngine();

  GameEngine &operator=(const GameEngine &other);
  friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);
};
