#pragma once
#include "../LoggingObserver/LoggingObserver.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::ostream;
using std::shared_ptr;
using std::string;
using std::vector;

void testGameStates();

class State; // Forward declaration

class Command : private ILoggable, private Subject {
private:
  /*
    Records effect of the command into the command object.
  */
  std::string m_effect;

public:
  shared_ptr<string> action;
  shared_ptr<State> nextState;

  Command() = default;
  Command(string action, shared_ptr<State> nextState);
  Command(Command const &other);
  Command &operator=(const Command &other);

  void saveEffect(std::string);
  std::string stringToLog() const override;

  friend ostream &operator<<(ostream &os, const Command &command);
};

class State {
public:
  shared_ptr<string> phase;
  vector<shared_ptr<Command>> commands;
  explicit State(string phase);

  State(State const &other);
  State &operator=(const State &other);
  friend ostream &operator<<(ostream &os, const State &state);
};

class GameEngine : private ILoggable, private Subject {
  shared_ptr<State> currState;
  void initGame();

public:
  string getCurrCommandsList();
  string executeCommand(string input);
  string getPhase();
  /*
    Returns the phase of the current state.
  */
  std::string stringToLog() const override;

  GameEngine();
  GameEngine(GameEngine const &other);
  GameEngine &operator=(const GameEngine &other);
  friend ostream &operator<<(ostream &os, const GameEngine &gameEngine);
};
