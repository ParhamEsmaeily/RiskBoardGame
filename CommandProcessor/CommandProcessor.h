#pragma once

#include <memory>

#include "GameEngine.h"

void testCommandProcessor(bool _isConsoleSource);

/*
 * Class that processes commands from the console.
 */
class CommandProcessor
{
private:
    std::vector<Command> commands;

public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor &_other);
    ~CommandProcessor();

    void operator=(const CommandProcessor &_other);

    bool getCommand(Command *_newCommand);
    bool validate(Command &_command, const std::string &_currentState);

private:
    bool readCommand(Command *_newCommand);
    void saveCommand(Command *_newCommand);
};