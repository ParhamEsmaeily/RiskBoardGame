#include <iostream>
#include <vector>
#include <memory>
#include<string>
#include "Orders.h"

using namespace std;

void testOrdersList()
{
    OrdersList* list = new OrdersList();

    cout<<"Initial orders list:\n";
    list->add(std::make_shared<Deploy>());
    list->add(std::make_shared<Advance>());
    list->add(std::make_shared<Bomb>());
    list->add(std::make_shared<Blockade>());
    list->add(std::make_shared<Airlift>());
    list->add(std::make_shared<Negotiate>());
    
    std::cout<<*list<<"\n";

    cout<<"Executing orders:\n";
    for(int i=0; i<list->size(); i++)
    {
        (*list)[i]->execute();
    }

    cout<<"Moving Order at index 0 to index 4\n";
    list->move(0, 4);
    std::cout<<*list<<"\n";

    cout<<"Removing order at index 4:\n";
    list->remove(4);
    std::cout<<*list<<"\n";

    cout<<"Test done."<<endl;    
}

int main()
{
    testOrdersList();
    return 0;
}

