#include "Player.h"

using namespace std;

Player::Player()
{
    this->playerId = 0;
    this->name = "player";
    vector<Territory *> t;
    this->territories = t;
    this->order_list = new OrdersList;
}

Player::Player(int playerID, string name)
{
    this->playerId = playerID;
    this->name = name;
    vector<Territory *> t;
    this->territories = t;
    this->order_list = new OrdersList();
    this->hand = new Hand();
    Deck deck;

    for (int i = 0; i < 15; i++)
    {
        Card card(card_type(i % 5));
        deck.insert(card.type); // Kind of random insertion of cards.
    }
    for (int i = 0; i < 15; i++)
    {
        deck.draw(*this->hand);
    }
}

// constructor for player with all parameters, name id territories hand and orderlists
Player::Player(int playerID, string name, vector<Territory *> &territories, Hand *hand, OrdersList *orders)
{
    this->playerId = playerID;
    this->name = name;
    this->territories = territories;
    this->hand = hand;
    this->order_list = orders;
}

// create copy player obj
Player::Player(const Player &p)
{
    playerId = p.playerId;
    name = p.name;
    for (Territory *t : p.territories)
    {
        this->addTerritory(new Territory(*t));
    }
    this->hand = new Hand(*(p.hand));
    this->order_list = new OrdersList(*(p.order_list));
}

// destructor
Player::~Player()
{
    /*     for (Territory *t : territories)
        {
            delete t;
        } */
    delete order_list;
    delete hand;

    territories.clear();
}

// assign one Player object to another.
Player &Player::operator=(const Player &p)
{
    playerId = p.playerId;
    name = p.name;
    for (Territory *t : p.territories)
    {
        this->addTerritory(new Territory(*t));
    }
    this->hand = new Hand(*(p.hand));
    this->order_list = new OrdersList(*(p.order_list));
    return *this;
}

vector<Territory *> Player::toDefend()
{
    return this->territories;
}

vector<Territory *> Player::toAttack()
{
    return this->territories;
}
// test
void Player::issueOrder(const Order &o)
{
    // if(o) {
    //     this->order_list->add(shared_ptr<o>);
    // }
    // Create a shared_ptr to the Order object pointed to by o

    // Add the shared_ptr to the order_list
    this->order_list->add(o);
}
// issueOrder: creates an order object and adds it to the list of orders
//  void Player::issueOrder(){
//      order_list->add(new Deploy);
//  }

void Player::addTerritory(Territory *t)
{
    territories.push_back(t); // to add t to player
}

// getters
int Player::getPlayerId()
{
    return playerId;
}

string Player::getName()
{
    return name;
}

Hand *Player::getHand()
{
    return &*hand;
}
OrdersList *Player ::getPlayerOrderList()
{
    return order_list;
}

vector<Territory *> Player::getTerritories()
{
    return territories;
}

// setters
void Player::setPlayerOrderList(OrdersList *orders)
{
    this->order_list = orders;
}

void Player::setTerritories(vector<Territory *> t)
{
    this->territories = t;
}

ostream &operator<<(ostream &os, Player &p)
{
    return os << "Name: " << p.getName() << " ID: " << p.getPlayerId();
}