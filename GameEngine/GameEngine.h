#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Command.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"

#include "Player.h"
#include "LoggingObserver.h"

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();
void testMainGameLoop();

// class State; // Forward declaration
class CommandProcessor;
class Map;

class GameEngine : private ILoggable, private Subject
{
  shared_ptr<State> currState;
  void initGame();

public:
  string getCurrCommandsList();
  string executeCommand(string input);
  string getPhase();

  void startupPhase();
  Command *command;
  vector<Player *> players;
  std::string stringToLog() const override;
  std::shared_ptr<Map> map;
  Deck* deck;

  CommandProcessor *commandProcessor;
  
  GameEngine();
  GameEngine(GameEngine const &other);
  ~GameEngine();

  GameEngine &operator=(const GameEngine &other);
  friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);

  void mainGameLoop(vector<Player *> players, const Map &map);

  std::string stringToLog() const override;
};