#include "Command.h"

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
  return "Command stringToLog: Saved effect " + *effect;
}

/** Ostream << operator */
ostream &operator<<(ostream &os, const Command &command) {
  os << "Action: " << *command.action;
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
