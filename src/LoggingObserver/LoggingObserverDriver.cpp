#include "GameEngine.h"
#include "Orders.h"
#include "CommandProcessor.h"

#include "LoggingObserver.h"

// To be implemented.
void test_LoggingObserver()
{
  int y;
  int x;
  std::cout
      << "\nTesting LoggingObserver. Please check the gamelog.txt file, if "
         "the proper log have been added."
      << std::endl;
  std::cout << "=====================================" << std::endl;

  // 1- Generic Order and OrderList created.
  Order o("name", "desc");
  OrdersList list;
  list.add(&o);
  std::cout << "Order added to the Order list." << std::endl;

  // 2-
  GameEngine engine;
  engine.phase("play");
  std::cout << "Phase of the GameEngine has changed." << std::endl;

  // 3-
  CommandProcessor pr;
  Command command;
  command.saveEffect("generic effect");
  std::cout << "Generic effect has been saved to a command." << std::endl;

  // 4-

  // 5-
  // o.execute();

  std::cout << "=====================================" << std::endl;
}