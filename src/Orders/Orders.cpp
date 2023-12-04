#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "Player.h"
#include "Map.h"
#include "Cards.h"

#include "Orders.h"
#include "PlayerStrategies.h"

using namespace std;

/**Prameterized constructor*/
OrdersList::OrdersList(vector<Order *> list)
{
    for (Order *o : list)
    {
        this->list.push_back(o);
    }
}

/**Copy constructor*/
OrdersList::OrdersList(const OrdersList &other) { this->list = other.list; }

OrdersList::~OrdersList()
{
    for (Order *o : this->list)
    {
        delete o;
    }
}

/**[] operator override*/
Order *OrdersList::operator[](const int i)
{
    if (i >= this->size() || i < 0)
    {
        std::cout << "Index out of range (index=" << i << ").\nProgram will exit."
                  << endl;
        exit(1);
    }
    return this->list[i];
}

/**<< Operator override, I would like for this operator to be able to call the
 * same operator, but for its contained class. This will do for now.
 */
ostream &operator<<(ostream &os, const OrdersList &olist)
{
    os << "Orders list: \n";
    for (int i = 0; i < olist.size(); i++)
    {
        os << (i + 1) << ". " << olist.list[i]->name << ": "
           << olist.list[i]->description << endl;
    }
    return os;
};

/**Moves Order to new index in list*/
bool OrdersList::move(int index, int destination)
{
    if (index == destination)
        return true;
    if (index < 0 || destination < 0 || index >= this->size() ||
        destination >= this->size())
    {
        std::cout << "Invalid parameters were passed(start index:" << index
                  << " destination: " << destination << " size: " << this->size()
                  << ")." << endl;
        return false;
    }
    Order *temp = this->list.at(index);
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

/**Adds order to back of the list. I would like to pass param o as a const
 * shared_ptr, since I'm not changing the actual pointer, but it seems like I
 * cannot. Is this because vector::push_back() is not a const function?
 */

void OrdersList::add(Order *o)
{
    this->list.push_back(o);
    // Calls in the log file that the order has been added to the list.
    Notify(this);
}

std::string OrdersList::stringToLog() const
{
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

Order::Order(Player *player, const Map *map, std::string name, std::string description)
{
    this->issuer = player;
    this->map = map;
    this->name = std::move(name);
    this->description = std::move(description);
}

Order::~Order()
{
    // redundant because an order does not own any of its pointers
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

Order &Order::operator=(const Order &other)
{
    if (*this != other)
    {
        this->name = other.name;
        this->description = other.description;
    }

    return *this;
}

bool Order::operator==(const Order &other)
{
    return (this->name == other.name && this->description == other.description);
}

bool Order::operator!=(const Order &other)
{
    return !(*this == other);
}

Advance::Advance(Player *player, const Map *map, const Territory *source, const Territory *dest, int units) : Order(player, map, "Advance", "An advance order tells a certain number of army units to move from a source territory to a target adjacent territory.")
{
    this->source_terr = source;
    this->dest_terr = dest;
    this->units_deployed = units;
}

Advance::Advance(const Advance &other) : Order(other)
{
    this->source_terr = other.source_terr;
    this->dest_terr = other.dest_terr;
    this->units_deployed = other.units_deployed;
}

Advance::~Advance()
{
    // redundant destructor, all pointers are non-owned
}

ostream &operator<<(ostream &os, Advance &order)
{
    Order *base = &order;
    os << base;
    os << order.units_deployed << " units are advancing from " << order.source_terr << "to " << order.dest_terr << endl;
    return os;
}

bool Advance::operator==(const Advance &other)
{
    if (Order::operator==(other) && this->source_terr == other.source_terr && this->dest_terr == other.dest_terr && this->units_deployed == other.units_deployed)
        return true;
    return false;
}

bool Advance::operator!=(const Advance &other)
{
    return !(*this == other);
}

Advance &Advance::operator=(const Advance &other)
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
    if (this->issuer->owns(this->source_terr) &&
        (this->issuer->getTerritoryUnits(this->source_terr) >= this->units_deployed || this->issuer->getStrategyType() == StratType::Cheater) &&
        Map::areAdjacent(*map, *source_terr, *dest_terr))
        return true;

    cout << this->name << " order invalid." << std::endl;
    return false;
}

void Advance::execute()
{
    if (validate())
    {
        if (this->issuer->owns(this->dest_terr))
        {
            this->issuer->addTerritory(const_cast<Territory *>(this->dest_terr));
            this->issuer->setTerritoryUnits(this->dest_terr, this->units_deployed);

            std::cout << "Player " << this->issuer->getName() << " has moved " << std::to_string(this->units_deployed) << " units to " << this->dest_terr->getName() << "!" << std::endl;
        }
        else
        {
            int attackers = this->units_deployed;
            int defenders = this->dest_terr->getOwner() == nullptr ? 2 : this->dest_terr->getOwner()->getTerritoryUnits(this->dest_terr);
            int original_defenders = defenders;
            bool attacker_won = false;

            while (attackers > 0 && defenders > 0)
            {
                int seed = rand() % 10;
                if (seed > 3)
                    defenders--;
                if (seed < 7)
                    attackers--;
            }
            int source_units = this->issuer->getTerritoryUnits(this->source_terr) - this->units_deployed;
            if (attackers > 0 || this->issuer->getStrategyType() == StratType::Cheater)
            {
                attacker_won = true;
                if (this->dest_terr->getOwner() != nullptr)
                    this->dest_terr->getOwner()->removeTerritory(this->dest_terr);
                this->issuer->addTerritory(const_cast<Territory *>(this->dest_terr));
                this->issuer->setTerritoryUnits(this->source_terr, source_units);
                this->issuer->setTerritoryUnits(this->dest_terr, attackers);
            }
            else
            {
                attacker_won = false;
                this->issuer->setTerritoryUnits(this->source_terr, source_units);
                if (this->dest_terr->getOwner() != nullptr)
                    this->dest_terr->getOwner()->setTerritoryUnits(this->dest_terr, defenders < 0 ? 0 : defenders);
            }

            std::cout << "Player " << this->issuer->getName() << " has " << (attacker_won ? "conquered " : "tried to attack ") << this->dest_terr->getName() << " (from " << this->source_terr->getName() << ") with " << this->units_deployed << " (" << attackers << " remaining) units, against " << original_defenders << " (" << defenders << " remaining) units!" << std::endl;
        }
    }
    Notify(this);
}

std::string Advance::stringToLog() const { return name; }

Airlift::Airlift(Player *player, const Map *map, const Territory *source, const Territory *dest, int units) : Order(player, map, "Airlift", "An airlift order tells a certain number of army units taken from a source territory to be moved to a target territory, the source and the target territory being owned by the player issuing the order.The airlift order can only be created by playing the airlift card.")
{
    this->source_terr = source;
    this->dest_terr = dest;
    this->units_deployed = units;
}

Airlift::Airlift(const Airlift &other) : Order(other)
{
    this->source_terr = other.source_terr;
    this->dest_terr = other.dest_terr;
    this->units_deployed = other.units_deployed;
}

Airlift::~Airlift()
{
    // redundant destructor, all pointers are non-owned
}

ostream &operator<<(ostream &os, Airlift &order)
{
    Airlift *base = &order;
    os << base;
    os << order.units_deployed << " units airlifted from " << order.source_terr << "to " << order.dest_terr << endl;
    return os;
}

bool Airlift::operator==(const Airlift &other)
{
    Airlift base = *this;
    Airlift baseo = other;
    if (base == baseo && this->source_terr == other.source_terr && this->dest_terr == other.dest_terr && this->units_deployed == other.units_deployed)
        return true;
    return false;
}

bool Airlift::operator!=(const Airlift &other)
{
    return !(this == &other);
}

Airlift &Airlift::operator=(const Airlift &other)
{
    if (this != &other)
    {
        Order::operator=(other);
        this->source_terr = other.source_terr;
        this->dest_terr = other.dest_terr;
        this->units_deployed = other.units_deployed;
    }
    return *this;
}

bool Airlift::validate()
{
    if (this->issuer->owns(this->source_terr) && this->issuer->owns(this->source_terr) && this->issuer->owns(this->dest_terr) && this->units_deployed <= this->issuer->getTerritoryUnits(this->source_terr) && this->issuer->card_count(CardType::airlift) > 0)
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Airlift::execute()
{
    if (validate())
    {
        int source_remainder = this->issuer->getTerritoryUnits(this->source_terr) - this->units_deployed;
        this->issuer->setTerritoryUnits(this->source_terr, source_remainder);
        int dest_sum = this->issuer->getTerritoryUnits(this->dest_terr) + this->units_deployed;
        this->issuer->setTerritoryUnits(this->dest_terr, dest_sum);
        this->issuer->getHand()->play(CardType::airlift);
        Notify(this);
    }
}

std::string Airlift::stringToLog() const
{
    return "Airlift stringToLog: Airlift Executing:";
}

Bomb::Bomb(Player *player, const Map *map, Player *target, const Territory *dest) : Order(player, map, "Bomb", "A bomb order targets a territory owned by another player than the one issuing the order. Its result is to remove half of the army units from this territory.The bomb order can only be created by playing the bomb card.")
{
    this->dest_terr = dest;
    this->target_player = target;
}

Bomb::Bomb(const Bomb &other) : Order(other)
{
    this->dest_terr = other.dest_terr;
    this->target_player = other.target_player;
}

Bomb::~Bomb()
{
    // redundant destructor, all pointers are non-owned
}

ostream &operator<<(ostream &os, Bomb &order)
{
    Bomb *base = &order;
    os << base;
    os << "Player bombs " << order.dest_terr << endl;
    return os;
}

bool Bomb::operator==(const Bomb &other)
{
    Bomb base = *this;
    Bomb baseo = other;
    if (base == baseo && this->dest_terr == other.dest_terr && this->target_player == other.target_player)
        return true;
    return false;
}

bool Bomb::operator!=(const Bomb &other)
{
    return !(*this == other);
}

Bomb &Bomb::operator=(const Bomb &other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->dest_terr = other.dest_terr;
    }
    return *this;
}
bool Bomb::validate()
{
    if (this->target_player->owns(this->dest_terr) && this->issuer->card_count(CardType::bomb) > 0 && !this->issuer->isAllied(this->target_player))
        return true;

    cout << this->name << " order invalid." << std::endl;
    return false;
}

void Bomb::execute()
{
    if (validate())
    {
        this->target_player->setTerritoryUnits(this->dest_terr, this->target_player->getTerritoryUnits(this->dest_terr) / 2);
        this->issuer->getHand()->play(CardType::bomb);

        std::cout << "Player " << this->issuer->getName() << " has bombed " << this->dest_terr->getName() << " (" << this->target_player->getTerritoryUnits(this->dest_terr) << " units remaining)!" << std::endl;
        Notify(this);
    }
}

std::string Bomb::stringToLog() const
{
    return "Bomb stringToLog: Bomb Executing:";
}

Blockade::Blockade(Player *player, const Map *map, Player *neutral, const Territory *dest) : Order(player, map, "Blockade", "A blockade order targets a territory that belongs to the player issuing the order. Its effect is to double the number of army units on the territory and to transfer the ownership of the territory to the Neutral player. The blockade order can only be created by playing the blockade card")
{
    this->neutral_player = neutral;
    this->dest_terr = dest;
}

Blockade::Blockade(const Blockade &other) : Order(other)
{
    this->dest_terr = other.dest_terr;
}

Blockade::~Blockade()
{
    // redundant destructor, all pointers are non-owned
}

ostream &operator<<(ostream &os, Blockade &order)
{
    Blockade *base = &order;
    os << base;
    os << "Blockade created in " << order.dest_terr << endl;
    return os;
}

bool Blockade::operator==(const Blockade &other)
{
    Blockade base = *this;
    Blockade baseo = other;
    if (base == baseo && this->dest_terr == other.dest_terr)
        return true;
    return false;
}

bool Blockade::operator!=(const Blockade &other)
{
    return !(*this == other);
}

Blockade &Blockade::operator=(const Blockade &other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->dest_terr = other.dest_terr;
    }
    return *this;
}

bool Blockade::validate()
{
    if (this->neutral_player == nullptr)
        return false;

    if (this->neutral_player->isNeutral() && this->issuer->card_count(CardType::blockade) > 0 && this->issuer->owns(this->dest_terr))
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Blockade::execute()
{
    if (validate())
    {
        int units = this->issuer->getTerritoryUnits(this->dest_terr) * 2;
        this->issuer->removeTerritory(this->dest_terr);
        this->neutral_player->addTerritory(const_cast<Territory *>(this->dest_terr));
        this->neutral_player->setTerritoryUnits(this->dest_terr, units);
        this->issuer->getHand()->play(CardType::blockade);
        Notify(this);
    }
}

std::string Blockade::stringToLog() const
{
    return "Blockade stringToLog: Order Executing:";
}

Deploy::Deploy(Player *player, const Map *map, const Territory *dest, int units) : Order(player, map, "Deploy", "A deploy order tells a certain number of army units taken from the reinforcement pool to deploy to a target territory owned by the player issuing this order.")
{
    this->dest_terr = dest;
    this->units_deployed = units;
}

Deploy::Deploy(const Deploy &other) : Order(other)
{
    this->dest_terr = other.dest_terr;
    this->units_deployed = other.units_deployed;
}

Deploy::~Deploy()
{
    // redundant destructor, all pointers are non-owned
}

ostream &operator<<(ostream &os, Deploy &order)
{
    Order *base = &order;
    os << base;
    os << order.units_deployed << " units deployed to " << order.dest_terr << endl;
    return os;
}

bool Deploy::operator==(const Deploy &other)
{
    const Order *baseo = &other;
    Order *base = this;
    if (base == baseo && this->units_deployed == other.units_deployed && this->dest_terr == other.dest_terr)
        return true;
    return false;
}

bool Deploy::operator!=(const Deploy &other)
{
    return !(*this == other);
}

Deploy &Deploy::operator=(const Deploy &other)
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
        this->issuer->setTerritoryUnits(this->dest_terr, this->issuer->getTerritoryUnits(this->dest_terr) + this->units_deployed);
        for (int i = 0; i < this->units_deployed; i++)
        {
            this->issuer->getHand()->play(CardType::reinforcement);
        }

        std::cout << "Player " << this->issuer->getName() << " has deployed " << this->units_deployed << " additional units to " << this->dest_terr->getName() << " (" << this->issuer->getTerritoryUnits(this->dest_terr) << " total units)!" << std::endl;

        Notify(this);
    }
}

std::string Deploy::stringToLog() const { return name; }

Negotiate::Negotiate(Player *player, const Map *map, Player *target) : Order(player, map, "Negotiate", "A negotiate order targets an enemy player. It results in the target player and the player issuing the order to not be able to successfully attack each others territories for the remainder of the turn.The negotiate order can only be created by playing the diplomacy card.")
{
    this->target_player = target;
}

Negotiate::Negotiate(const Negotiate &other) : Order(other)
{
    this->target_player = other.target_player;
}

Negotiate::~Negotiate()
{
    // redundant destructor, all pointers are non-owned
}

ostream &operator<<(ostream &os, Negotiate &order)
{
    Negotiate *base = &order;
    os << base;
    os << "Negotiations between " << order.issuer << "and " << order.target_player << " result in a ceasefire for the remainder of the turn." << endl;
    return os;
}

bool Negotiate::operator==(const Negotiate &other)
{
    Negotiate base = *this;
    Negotiate baseo = other;
    if (base == baseo && this->target_player == other.target_player)
        return true;
    return false;
}

bool Negotiate::operator!=(const Negotiate &other)
{
    return !(*this == other);
}

Negotiate &Negotiate::operator=(const Negotiate &other)
{
    if (*this != other)
    {
        Order::operator=(other);
        this->target_player = other.target_player;
    }
    return *this;
}

bool Negotiate::validate()
{
    if (this->issuer->card_count(CardType::diplomacy) > 0)
        return true;
    cout << this->name << " order invalid.";
    return false;
}

void Negotiate::execute()
{
    if (validate())
    {
        this->issuer->addAlly(this->target_player);
        this->target_player->addAlly(this->issuer);
        this->issuer->getHand()->play(CardType::diplomacy);
        Notify(this);
    }
}

std::string Negotiate::stringToLog() const
{
    return "Negotiate stringToLog: Order executing.";
}
