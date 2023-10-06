#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Orders.h"
using namespace std;

/**Prameterized constructor*/
OrdersList::OrdersList(vector<shared_ptr<Order>> list)
{
    this->list = list;
}

/**Copy constructor*/
OrdersList::OrdersList(const OrdersList &other)
{
    this->list = other.list;
}

/**[] operator override*/
std::shared_ptr<Order> &OrdersList::operator[](const int i)
{
    if (i >= this->size() || i < 0)
    {
        std::cout << "Index out of range (index=" << i << ").\nProgram will exit." << endl;
        exit(1);
    }
    return this->list[i];
}

/**<< Operator override, I would like for this operator to be able to call the same operator,
 * but for its contained class. This will do for now.
 */
ostream &operator<<(ostream &os, const OrdersList &olist)
{
    os << "Orders list: \n";
    for (int i = 0; i < olist.size(); i++)
    {
        os << (i + 1) << ". " << olist.list[i]->name << ": " << olist.list[i]->description << endl;
    }
    return os;
};

/**Moves Order to new index in list*/
bool OrdersList::move(int index, int destination)
{
    if (index == destination)
        return true;
    if (index < 0 || destination < 0 || index >= this->size() || destination >= this->size())
    {
        std::cout << "Invalid parameters were passed(start index:" << index << " destination: " << destination << " size: " << this->size() << ")." << endl;
        return false;
    }
    shared_ptr<Order> temp = this->list.at(index);
    this->list.erase(this->list.begin() + index);
    this->list.insert(this->list.begin() + destination, temp);
    return true;
}

/**Removes Order from list*/
bool OrdersList::remove(int index)
{
    if (this->list.size() >= index)
    {
        this->list.erase(this->list.begin() + index);
        return true;
    }
    cout << "Invalid index.";
    return false;
}

/**Adds order to back of the list. I would like to pass param o as a const shared_ptr,
 * since I'm not changing the actual pointer, but it seems like I cannot. Is this because
 * vector::push_back() is not a const function?
 */
void OrdersList::add(const Order &o)
{
    this->list.emplace_back(make_shared<Order>(o));
}

/**Copy constructor*/
Order::Order(Order const &other)
{
    this->name = other.name;
    this->description = other.description;
}

/**<< operator override*/
ostream &operator<<(ostream &os, Order &order)
{
    os << "Order: " << order.name << "\n"
       << order.description << endl;
    return os;
}

bool Order::validate()
{
    std::cout << this->name << ": No real implementation yet! Validated.";
    return true;
}

void Order::execute()
{
    if (this->validate())
        std::cout << "No real implementation yet! Executed." << endl;
    else
        std::cout << "Cannot validate." << endl;
}

Deploy::Deploy()
{
    this->name = "Deploy";
    this->description = "Placeholder description for deploy order.";
}

Advance::Advance()
{
    this->name = "Advance";
    this->description = "Placeholder description for advance order.";
}
Bomb::Bomb()
{
    this->name = "Bomb";
    this->description = "Placeholder description for bomb order.";
}
Blockade::Blockade()
{
    this->name = "Blockade";
    this->description = "Placeholder description for blockade order.";
}
Airlift::Airlift()
{
    this->name = "Airlift";
    this->description = "Placeholder description for airlift order.";
}
Negotiate::Negotiate()
{
    this->name = "Negotiate";
    this->description = "Placeholder description for negotiate order.";
}