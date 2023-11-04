#include "CommandProcessor.h"

void testCommandProcessor(bool _isConsoleSource)
{
    CommandProcessor commandProcessor;
    Command newCommand;

    if (_isConsoleSource)
    {
        std::cout << "Testing command processor from console input. Enter an invalid/unknown command, then state to exit." << std::endl;

        while (true)
        {
            std::cout << "Enter a new command: ";
            auto valid = commandProcessor.getCommand(&newCommand);

            if (!valid)
            {
                std::cout << "Unknwon command." << std::endl;
                break;
            }

            std::string testState;
            std::cout << "Enter the current state: ";
            std::cin >> testState;

            valid = commandProcessor.validate(newCommand, testState);

            if (!valid)
            {
                std::cout << "Invalid command." << std::endl;
                break;
            }

            std::cout << "Valid command: {" << newCommand << "}" << std::endl;
        }
    }
    else
    {
        std::cout << "Testing command processor from file input..." << std::endl;
    }

    std::cout << "Done testing command processor.\n"
              << std::endl;
}