#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Orders.h"

void testOrdersList()
{
    OrdersList *list = new OrdersList();

    cout << "Initial orders list:\n";
    list->add(Deploy());
    list->add(Advance());
    list->add(Bomb());
    list->add(Blockade());
    list->add(Airlift());
    list->add(Negotiate());

    std::cout << *list << "\n";

    cout << "Executing orders:\n";
    for (int i = 0; i < list->size(); i++)
    {
        (*list)[i]->execute();
    }

    cout << "Moving Order at index 0 to index 4\n";
    list->move(0, 4);
    std::cout << *list << "\n";

    cout << "Removing order at index 4:\n";
    list->remove(4);
    std::cout << *list << "\n";

    cout << "Test done." << endl;
}

/* int main()
{
    testOrdersList();
    return 0;
} */
