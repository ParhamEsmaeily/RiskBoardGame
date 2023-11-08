#include <algorithm>

#include "CommandProcessor.h"

CommandProcessor::CommandProcessor()
{
    commands = std::vector<Command>();
}

CommandProcessor::CommandProcessor(const CommandProcessor &_other)
{
    // deep copies the command list
    commands = std::vector<Command>();

    for (const auto &command : _other.commands)
    {
        commands.emplace_back(command);
    }
}

CommandProcessor::~CommandProcessor()
{
    // redundant destructor
}

CommandProcessor &CommandProcessor::operator=(const CommandProcessor &_other)
{
    commands = _other.commands;

    return *this;
}

bool CommandProcessor::getCommand(Command *_newCommand)
{
    // reads a command from the console and saves it in the command list, returns false if the command is unknown
    if (!readCommand(_newCommand))
        return false;

    // saves the commmand internally
    saveCommand(_newCommand);

    return true;
}

bool CommandProcessor::validate(Command &_command, const std::string &_currentState)
{
    // if the next state is unknown, then the command is invalid
    if (_command.nextState->phase->compare("unknown") == 0)
    {
        _command.saveEffect("Invalid command.");
        return false;
    }

    // if the list of valid states is empty, then the command is invalid
    if (_command.validStates.empty())
    {
        _command.saveEffect("No valid states for this command.");
        return false;
    }

    // if the current state is in the list of valid states (i.e. the returned iterator is not the end of the vector), then the command is valid
    return std::find(_command.validStates.begin(), _command.validStates.end(), _currentState) != _command.validStates.end();
}

bool CommandProcessor::readCommand(Command *_newCommand)
{
    bool result = true;

    std::string commandString;
    std::string commandArgString;
    std::vector<std::string> validStatesString;
    std::string nextStateString;

    // reads the initial command from the console
    std::cin >> commandString;

    // checks if the command is valid and sets the next state and valid states accordingly
    if (commandString.rfind("loadmap", 0) != std::string::npos)
    {
        // if the command requires an argument, reads the argument from the console
        std::cin >> commandArgString;

        commandString += " " + commandArgString;
        nextStateString = "maploaded";
        validStatesString = {"start", "maploaded"};
    }
    else if (commandString.rfind("validatemap", 0) != std::string::npos)
    {
        nextStateString = "mapvalidated";
        validStatesString = {"maploaded"};
    }
    else if (commandString.rfind("addplayer", 0) != std::string::npos)
    {
        std::cin >> commandArgString;

        commandString += " " + commandArgString;
        nextStateString = "playersadded";
        validStatesString = {"mapvalidated", "playersadded"};
    }
    else if (commandString.rfind("gamestart", 0) != std::string::npos)
    {
        nextStateString = "assignreinforcement";
        validStatesString = {"playersadded"};
    }
    else if (commandString.rfind("replay", 0) != std::string::npos)
    {
        nextStateString = "start";
        validStatesString = {"win"};
    }
    else if (commandString.rfind("quit", 0) != std::string::npos)
    {
        nextStateString = "exit";
        validStatesString = {"win"};
    }
    else
    {
        // if the command is unknown, sets the next state to unknown and the valid states to an empty vector (for later validation)
        nextStateString = "unknown";
        validStatesString = {};
        result = false;
    }

    // sets the command's properties
    _newCommand->action = std::make_shared<std::string>(commandString);
    _newCommand->validStates = validStatesString;
    _newCommand->nextState = std::make_shared<State>(nextStateString);

    return result;
}

void CommandProcessor::saveCommand(Command *_newCommand)
{
    // creates a copy of the command and stores it in the command list
    commands.emplace_back(*_newCommand);
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader *_fileLineReader) : CommandProcessor()
{
    fileLineReader = _fileLineReader;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &_other) : CommandProcessor(_other)
{
    // see FileLineReader.h for more information and warnings about its assignment operator
    fileLineReader = _other.fileLineReader;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    // redundant destructor
    CommandProcessor::~CommandProcessor();
}

FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &_other)
{
    CommandProcessor::operator=(_other);

    // see FileLineReader.h for more information and warnings about its assignment operator
    fileLineReader = _other.fileLineReader;

    return *this;
}

bool FileCommandProcessorAdapter::getCommand(Command *_newCommand)
{
    std::string commandString;
    std::string commandArgString;
    std::vector<std::string> validStatesString;
    std::string nextStateString;

    // checks if there are more lines to read from the file
    if (!fileLineReader->hasLine())
        return false;

    // reads the initial command from the console
    if (!fileLineReader->getLine(&commandString))
        return false;

    bool result = true;

    // checks if the command is valid and sets the next state and valid states accordingly
    if (commandString.rfind("loadmap", 0) != std::string::npos)
    {
        nextStateString = "maploaded";
        validStatesString = {"start", "maploaded"};
    }
    else if (commandString.rfind("validatemap", 0) != std::string::npos)
    {
        nextStateString = "mapvalidated";
        validStatesString = {"maploaded"};
    }
    else if (commandString.rfind("addplayer", 0) != std::string::npos)
    {
        nextStateString = "playersadded";
        validStatesString = {"mapvalidated", "playersadded"};
    }
    else if (commandString.rfind("gamestart", 0) != std::string::npos)
    {
        nextStateString = "assignreinforcement";
        validStatesString = {"playersadded"};
    }
    else if (commandString.rfind("replay", 0) != std::string::npos)
    {
        nextStateString = "start";
        validStatesString = {"win"};
    }
    else if (commandString.rfind("quit", 0) != std::string::npos)
    {
        nextStateString = "exit";
        validStatesString = {"win"};
    }
    else
    {
        // if the command is unknown, sets the next state to unknown and the valid states to an empty vector (for later validation)
        nextStateString = "unknown";
        validStatesString = {};
        result = false;
    }

    // sets the command's properties
    _newCommand->action = std::make_shared<std::string>(commandString);
    _newCommand->validStates = validStatesString;
    _newCommand->nextState = std::make_shared<State>(nextStateString);

    return result;
}

bool FileCommandProcessorAdapter::canGetCommand()
{
    return fileLineReader->hasLine();
}
