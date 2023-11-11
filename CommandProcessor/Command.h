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


class Command;
class State;



class Command : private ILoggable, private Subject {
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

class State {
public:
  shared_ptr<string> phase;
  vector<shared_ptr<Command>> commands;
  explicit State(string phase);

  State(State const &other);
  State &operator=(const State &other);
  friend ostream &operator<<(ostream &os, const State &state);
};