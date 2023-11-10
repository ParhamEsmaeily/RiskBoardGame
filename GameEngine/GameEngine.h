#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Player.h"
#include "LoggingObserver.h"

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();
void testMainGameLoop();

class State; // Forward declaration

class Command : private ILoggable, private Subject
{
public:
  shared_ptr<string> action;
  shared_ptr<State> nextState;
  vector<string> validStates;
  shared_ptr<string> effect;

  Command() = default;
  Command(string action, shared_ptr<State> nextState);

  Command(Command const &other);
  Command &operator=(const Command &other);
  friend ostream &operator<<(ostream &os, const Command &command);

  void saveEffect(const std::string &effect);
  std::string stringToLog() const override;
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

class GameEngine : private ILoggable, private Subject
{
  shared_ptr<State> currState;
  void initGame();
  void reinforcementPhase(vector<Player *> players, const Map &map);
  void issueOrdersPhase(vector<Player *> players, const Map &map);
  void executeOrdersPhase(vector<Player *> players);

public:
  string getCurrCommandsList();
  string executeCommand(string input);
  string getPhase();

  GameEngine();
  GameEngine(GameEngine const &other);
  GameEngine &operator=(const GameEngine &other);
  friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);

  void mainGameLoop(vector<Player *> players, const Map &map);

  std::string stringToLog() const override;
};