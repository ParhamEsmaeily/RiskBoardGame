#include "Orders.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

/**Prameterized constructor*/
OrdersList::OrdersList(vector<shared_ptr<Order>> list) { this->list = list; }

/**Copy constructor*/
OrdersList::OrdersList(const OrdersList &other) { this->list = other.list; }

/**[] operator override*/
std::shared_ptr<Order> &OrdersList::operator[](const int i) {
  if (i >= this->size() || i < 0) {
    std::cout << "Index out of range (index=" << i << ").\nProgram will exit."
              << endl;
    exit(1);
  }
  return this->list[i];
}

/**<< Operator override, I would like for this operator to be able to call the
 * same operator, but for its contained class. This will do for now.
 */
ostream &operator<<(ostream &os, const OrdersList &olist) {
  os << "Orders list: \n";
  for (int i = 0; i < olist.size(); i++) {
    os << (i + 1) << ". " << olist.list[i]->name << ": "
       << olist.list[i]->description << endl;
  }
  return os;
};

/**Moves Order to new index in list*/
bool OrdersList::move(int index, int destination) {
  if (index == destination)
    return true;
  if (index < 0 || destination < 0 || index >= this->size() ||
      destination >= this->size()) {
    std::cout << "Invalid parameters were passed(start index:" << index
              << " destination: " << destination << " size: " << this->size()
              << ")." << endl;
    return false;
  }
  shared_ptr<Order> temp = this->list.at(index);
  this->list.erase(this->list.begin() + index);
  this->list.insert(this->list.begin() + destination, temp);
  return true;
}

/**Removes Order from list*/
bool OrdersList::remove(int index) {
  if (this->list.size() >= index) {
    this->list.erase(this->list.begin() + index);
    return true;
  }
  cout << "Invalid index.";
  return false;
}

/**Adds order to back of the list. I would like to pass param o as a const
 * shared_ptr, since I'm not changing the actual pointer, but it seems like I
 * cannot. Is this because vector::push_back() is not a const function?
 */
void OrdersList::add(const Order &o) {
  this->list.emplace_back(make_shared<Order>(o));
  // Calls in the log file that the order has been added to the list.
  Notify(this);
}

std::string OrdersList::stringToLog() const {
  // Called when an order is added to the list.
  std::string log = "Order stringToLog : Added ";
  log.append(list.back()->name);
  return log;
}

/*
    Parameterized constructor.
    Added after A1 because cannot list-initalized protected members in subclass.
*/
Order::Order(std::string name, std::string desc)
    : name(std::move(name)), description(std::move(desc)) {}

/**Copy constructor*/
Order::Order(Order const &other) {
  this->name = other.name;
  this->description = other.description;
}

/**<< operator override*/
ostream &operator<<(ostream &os, Order &order) {
  os << "Order: " << order.name << "\n" << order.description << endl;
  return os;
}

bool Order::validate() {
  std::cout << this->name << ": No real implementation yet! Validated.";
  return true;
}

void Order::execute() {
  if (this->validate())
    std::cout << "No real implementation yet! Executed." << endl;
  else
    std::cout << "Cannot validate." << endl;
}

Advance::Advance(Player* player, Player* target, const Territory* source, const Territory* dest, int units) :
    Order(player, "Advance", "An advance order tells a certain number of army units to move from a source territory to a target adjacent territory.")
{
    this->target_player = target;
    this->source_terr = source;
    this->dest_terr = dest;
    this->units_deployed = units;
}

Advance::Advance(const Advance& other) :
    Order(other)
{
    this->target_player = other.target_player;
    this->source_terr = other.source_terr;
    this->dest_terr = other.dest_terr;
    this->units_deployed = other.units_deployed;
}

Advance::~Advance()
{
    Order::~Order();
}

ostream& operator<<(ostream& os, Advance& order)
{
    Order* base = &order;
    os << base;
    os << order.units_deployed << " units are advancing from " << order.source_terr << "to " << order.dest_terr << endl;
    return os;
}

bool Advance::operator==(const Advance& other)
{
    if (Order::operator==(other) && this->source_terr == other.source_terr && this->dest_terr == other.dest_terr && this->units_deployed == other.units_deployed)
        return true;
    return false;
}

bool Advance::operator!=(const Advance& other)
{
    return !(*this == other);
}

Advance& Advance::operator=(const Advance& other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->source_terr = other.source_terr;
        this->dest_terr = other.dest_terr;
        this->units_deployed = other.units_deployed;
    }
    return *this;
}

bool Advance::validate()
{
    if (this->issuer->owns(this->source_terr) && this->issuer->getTerritoryUnits(this->source_terr) >= this->units_deployed && //check adjacency)
        return true;
        cout << this->name << " order invalid.";
        return false;
}

void Advance::execute()
{
    if (this.validate())
    {
        if (this->issuer->owns(this->dest_terr))
        {
            this->issuer->addTerritory(this->dest_terr);
            this->issuer->setTerritoryUnits(this->dest_terr, this->units_deployed);
        }
        else
        {
            int attackers = this->units_deployed;
            int defenders = this->target_player->getTerritoryUnits(this->source_terr);
            while (attackers > 0 && defenders > 0)
            {
                int seed = rand() % 10;
                if (seed > 3)
                    defenders--;
                if (seed < 7)
                    attackers--;
            }
            int source_units = this->issuer->getTerritoryUnits(this->source_terr) - this->units_deployed;
            if (attackers > 0)
            {
                this->target_player->removeTerritory(this->dest_terr);
                this->issuer->addTerritory(this->dest_terr);
                this->issuer->setTerritoryUnits(this->source_terr, source_units);
                this->issuer->setTerritoryUnits(this->dest_terr, attackers);
            }
            else
            {
                this->issuer->setTerritoryUnits(this->source_terr, source_units);
                this->target_player->setTerritoryUnits(this->dest_terr, defenders);
            }
        }
    }
    Notify(this);
}

std::string Advance::stringToLog() const { return name; }

Airlift::Airlift(Player* player, Card* card, const Territory* source, const Territory* dest, int units) :
    Order(player, "Airlift", "An airlift order tells a certain number of army units taken from a source territory to be moved to a target territory, the source and the target territory being owned by the player issuing the order.The airlift order can only be created by playing the airlift card.")
{
    this->card = card;
    this->source_terr = source;
    this->dest_terr = dest;
    this->units_deployed = units;
}

Airlift::Airlift(const Airlift& other) :
    Order(other)
{
    this->source_terr = other.source_terr;
    this->dest_terr = other.dest_terr;
    this->card = other.card;
    this->units_deployed = other.units_deployed;
}

Airlift::~Airlift()
{
    Order::~Order();
}

ostream& operator<<(ostream& os, Airlift& order)
{
    Order* base = &order;
    os << base;
    os << order.units_deployed << " units airlifted from " << order.source_terr << "to " << order.dest_terr << endl;
    return os;
}

bool Airlift::operator==(const Airlift& other)
{
    Order base = *this;
    Order baseo = other;
    if (base == baseo && this->source_terr == other.source_terr && this->dest_terr == other.dest_terr && this->units_deployed == other.units_deployed && this->card == other.card)
        return true;
    return false;
}

bool Airlift::operator!=(const Airlift& other)
{
    return!(this == &other);
}

Airlift& Airlift::operator=(const Airlift& other)
{
    if (this != &other)
    {
        Order::operator=(other);
        this->source_terr = other.source_terr;
        this->dest_terr = other.dest_terr;
        this->units_deployed = other.units_deployed;
        this->card = other.card;
    }
    return *this;
}

bool Airlift::validate()
{
    if (this->issuer->owns(this->source_terr) && this->issuer->owns(this->source_terr) && this->units_deployed <= this->issuer->getTerritoryUnits(this->source_terr) && this->card->type() == "airlift")
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Airlift::execute()
{
    if (this.validate())
    {
        int source_remainder = this->issuer->getTerritoryUnits(this->source_terr) - this->units_deployed;
        this->issuer->setTerritoryUnits(this->source_terr, source_remainder);
        int dest_sum = this->issuer->getTerritoryUnits(this->dest_terr) + this->units_deployed;
        this->issuer->setTerritoryUnits(this->dest_terr, dest_sum);
        Notify(this);
    }
}

std::string Airlift::stringToLog() const {
    return "Airlift stringToLog: Airlift Executing:";
}

Bomb::Bomb(Player* player, Player* target, Card* card, const Territory* dest) :
    Order(player, "Bomb", "A bomb order targets a territory owned by another player than the one issuing the order. Its result is to remove half of the army units from this territory.The bomb order can only be created by playing the bomb card.")
{
    this->card = card;
    this->dest_terr = dest;
    this->target_player = target;
}

Bomb::Bomb(const Bomb& other) :
    Order(other)
{
    this->dest_terr = other.dest_terr;
    this->card = other.card;
    this->target_player = other.target_player;
}

Bomb::~Bomb()
{
    Order::~Order();
    delete this->card;
    delete this->dest_terr;
}

ostream& operator<<(ostream& os, Bomb& order)
{
    Order* base = &order;
    os << base;
    os << "Player bombs " << order.dest_terr << endl;
    return os;
}

bool Bomb::operator==(const Bomb& other)
{
    Order base = *this;
    Order baseo = other;
    if (base == baseo && this->dest_terr == other.dest_terr && this->card == other.card && this->target_player == other.target_player)
        return true;
    return false;
}

bool Bomb::operator!=(const Bomb& other)
{
    return!(*this == other);
}

Bomb& Bomb::operator=(const Bomb& other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->dest_terr = other.dest_terr;
        this->card = other.card;
    }
    return *this;
}
bool Bomb::validate()
{
    if (this->target_player->owns(this->dest_terr) && this->card->type() == "bomb" && !this->issuer->isAllied(this->target_player))
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Bomb::execute()
{
    if (this.validate())
    {
        this->target_player->setTerritoryUnits(this->dest_terr, this->target_player->getTerritoryUnits(this->dest_terr) / 2);
        Notify(this);
    }
}

std::string Bomb::stringToLog() const {
  return "Bomb stringToLog: Bomb Executing:";
}

Blockade::Blockade(Player* player, Player* neutral, Card* card, const Territory* dest) :
    Order(player, "Blockade", "A blockade order targets a territory that belongs to the player issuing the order. Its effect is to double the number of army units on the territory and to transfer the ownership of the territory to the Neutral player. The blockade order can only be created by playing the blockade card")
{
    this->neutral_player = neutral;
    this->card = card;
    this->dest_terr = dest;
}

Blockade::Blockade(const Blockade& other) :
    Order(other)
{
    this->card = other.card;
    this->dest_terr = other.dest_terr;
}

Blockade::~Blockade()
{
    Order::~Order();
    delete this->card;
    delete this->dest_terr;
}

ostream& operator<<(ostream& os, Blockade& order)
{
    Order* base = &order;
    os << base;
    os << "Blockade created in " << order.dest_terr << endl;
    return os;
}

bool Blockade::operator==(const Blockade& other)
{
    Order base = *this;
    Order baseo = other;
    if (base == baseo && this->dest_terr == other.dest_terr && this->card == other.card)
        return true;
    return false;
}

bool Blockade::operator!=(const Blockade& other)
{
    return!(*this == other);
}

Blockade& Blockade::operator=(const Blockade& other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->dest_terr = other.dest_terr;
        this->card = other.card;
    }
    return *this;
}

bool Blockade::validate()
{
    if (this->neutral_player->isNeutral() && this->card->type() == "blockade" && this->issuer->owns(this->dest_terr))
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Blockade::execute()
{
    if (this.validate())
    {
        int units = this->issuer->getTerritoryUnits(this->dest_terr) * 2;
        this->issuer->removeTerritory(this->dest_terr);
        this->neutral_player->addTerritory(this->dest_terr);
        this->neutral_player->setTerritoryUnits(this->dest_terr, units);
        Notify(this);
    }
}

std::string Blockade::stringToLog() const {
  return "Blockade stringToLog: Order Executing:";
}

Deploy::Deploy(Player* player, const Territory* dest, int units) :
    Order(issuer, "Deploy", "A deploy order tells a certain number of army units taken from the reinforcement pool to deploy to a target territory owned by the player issuing this order.")
{
    this->dest_terr = dest;
    this->units_deployed = units;
}

Deploy::Deploy(const Deploy& other) :
    Order(other)
{
    this->dest_terr = other.dest_terr;
    this->units_deployed = other.units_deployed;
}

Deploy::~Deploy()
{
    Order::~Order();
}

ostream& operator<<(ostream& os, Deploy& order)
{
    Order* base = &order;
    os << base;
    os << order.units_deployed << " units deployed to " << order.dest_terr << endl;
    return os;
}

bool Deploy::operator==(const Deploy& other)
{
    const Order* baseo = &other;
    Order* base = this;
    if (base == baseo && this->units_deployed == other.units_deployed && this->dest_terr == other.dest_terr)
        return true;
    return false;
}

Deploy& Deploy::operator=(const Deploy& other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->dest_terr = other.dest_terr;
        this->units_deployed = other.units_deployed;
    }
    return *this;
}

bool Deploy::validate()
{
    if (this->issuer->owns(this->dest_terr))
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Deploy::execute()
{
    if (this->validate())
    {
        this->issuer->setTerritoryUnits(this->dest_terr, this->units_deployed);
        Notify(this);
    }
}

std::string Deploy::stringToLog() const { return name; }

Negotiate::Negotiate(Player* player, Player* target, Card* card) :
    Order(player, "Negotiate", "A negotiate order targets an enemy player. It results in the target player and the player issuing the order to not be able to successfully attack each others territories for the remainder of the turn.The negotiate order can only be created by playing the diplomacy card.")
{
    this->target_player = target;
    this->card = card;
}

Negotiate::Negotiate(const Negotiate& other) :
    Order(other)
{
    this->target_player = other.target_player;
    this->card = other.card;
}

Negotiate::~Negotiate()
{
    Order::~Order();
    delete this->card;
    delete this->target_player;
}

ostream& operator<<(ostream& os, Negotiate& order)
{
    Order* base = &order;
    os << base;
    os << "Negotiations between " << order.issuer << "and " << order.target_player << " result in a ceasefire for the remainder of the turn." << endl;
    return os;
}

bool Negotiate::operator==(const Negotiate& other)
{
    Order base = *this;
    Order baseo = other;
    if (base == baseo && this->target_player == other.target_player && this->card == other.card)
        return true;
    return false;
}

bool Negotiate::operator!=(const Negotiate& other)
{
    return!(*this == other);
}

Negotiate& Negotiate::operator=(const Negotiate& other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->card = other.card;
        this->target_player = other.target_player;
    }
    return *this;
}

bool Negotiate::validate()
{
    if (this->card->type() == "negotiate")
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Negotiate::execute()
{
    if (this.validate())
    {
        this->issuer->addAlly(this->target_player);
        this->target_player->addAlly(this->issuer);
        Notify(this);
    }
}

std::string Negotiate::stringToLog() const {
  return "Negotiate stringToLog: Order executing.";
}
