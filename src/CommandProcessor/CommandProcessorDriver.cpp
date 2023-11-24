#include "CommandProcessor.h"

void testCommandProcessor(const std::string &_fileName) {
  Command newCommand;

  // if no file name is provided, then the command processor will read commands
  // from the console
  if (_fileName.compare("") == 0) {
    CommandProcessor commandProcessor = CommandProcessor();

    std::cout << "Testing command processor from console input. Enter an "
                 "unknown command to exit."
              << std::endl;

    // reads commands from the console until an invalid/unknown command is
    // entered
    while (true) {
      std::cout << "Enter a new command: ";
      auto result = commandProcessor.getCommand(&newCommand);

      if (!result) {
        std::cout << "Unknown command: {" << newCommand << "}" << std::endl;
        break;
      }

      std::string testState;
      std::cout << "Enter the current state: ";
      std::cin >> testState;

      result = commandProcessor.validate(newCommand, testState);

      if (!result) {
        std::cout << "Invalid command: {" << newCommand << "}" << std::endl;
        continue;
      }

      std::cout << "Valid command: {" << newCommand << "}" << std::endl;
    }
  } else {
    FileLineReader fileLineReader(_fileName);
    FileCommandProcessorAdapter fileCommandProcessorAdapter =
        FileCommandProcessorAdapter(&fileLineReader);

    std::cout << "Testing command processor from file input (" << _fileName
              << ")... Enter the initial game state to start." << std::endl;

    std::string currentState;
    std::cout << "Enter the initial state: ";
    std::cin >> currentState;

    while (true) {
      auto result = fileCommandProcessorAdapter.canGetCommand();

      // no more commands, the command processor will stop reading commands
      if (!result) {
        break;
      }

      result = fileCommandProcessorAdapter.getCommand(&newCommand);

      if (!result) {
        std::cout << "Unknown command: {" << newCommand << "}" << std::endl;
        continue;
      }

      result = fileCommandProcessorAdapter.validate(newCommand, currentState);

      if (!result) {
        std::cout << "Invalid command: {" << newCommand << "}" << std::endl;
        continue;
      }

      currentState = *newCommand.nextState->phase;

      std::cout << "Valid command: {" << newCommand << "}" << std::endl;
    }
  }

  std::cout << "Done testing command processor.\n" << std::endl;
}