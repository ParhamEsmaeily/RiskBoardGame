#pragma once
#include "Command.h"
#include "FileLineReader.h"
#include <algorithm>
#include <fstream>
#include <memory>

void testCommandProcessor(const std::string &_fileName = "");

/*
 * Class that processes commands from the console.
 */
class CommandProcessor {
private:
  std::vector<Command> commands;

public:
  /**
   * @brief The CommandProcessor class is responsible for executing commands and
   * keeping track of the executed commands.
   */
  CommandProcessor();
  /**
   * @brief The CommandProcessor class is responsible for executing commands and
   * keeping track of the executed commands.
   */
  CommandProcessor(const CommandProcessor &_other);
  ~CommandProcessor();

  CommandProcessor &operator=(const CommandProcessor &_other);

  /**
   * @brief Retrieves the next command from the command processor (console).
   *
   * @param _newCommand Pointer to a Command object where the retrieved command
   * will be stored.
   * @return true if a command was successfully retrieved, false otherwise.
   */
  virtual bool getCommand(Command *_newCommand);
  /**
   * @brief Validates a command based on the current state of the game.
   *
   * @param _command The command to be validated.
   * @param _currentState The current state of the game.
   * @return true if the command is valid, false otherwise.
   */
  bool validate(Command &_command, const std::string &_currentState);

private:
  bool readCommand(Command *_newCommand);
  void saveCommand(Command *_newCommand);
};

/*
 * Adapter class that processes commands from a file.
 */
class FileCommandProcessorAdapter : public CommandProcessor {
private:
  FileLineReader *fileLineReader;

public:
  /**
   * @brief Constructs a FileCommandProcessorAdapter object.
   *
   * @param _fileLineReader A pointer to a FileLineReader object.
   */
  FileCommandProcessorAdapter(FileLineReader *_fileLineReader);
  /**
   * @brief Adapter class that allows the CommandProcessor to read commands from
   * a file.
   *
   * It is an adapter for the CommandProcessor class.
   */
  FileCommandProcessorAdapter(const FileCommandProcessorAdapter &_other);
  /**
   * @attention The FileLineReader object is not owned by the
   * FileCommandProcessorAdapter object, so it is not deleted in the destructor.
   * Make sure to delete it manually.
   */
  ~FileCommandProcessorAdapter();

  FileCommandProcessorAdapter &
  operator=(const FileCommandProcessorAdapter &_other);

  /**
   * @brief Checks if there is a command available to be retrieved.
   *
   * @return true if there is a command available, false otherwise.
   */
  bool canGetCommand();
  /**
   * @brief Retrieves the next command from the command processor (file).
   *
   * @param _newCommand Pointer to a Command object where the retrieved command
   * will be stored.
   * @return true if a command was successfully retrieved, false otherwise.
   */
  bool getCommand(Command *_newCommand) override;
};