#include "./Player.h"
#include "../Cards/Cards.h"
#include "../Orders/Orders.h"
using namespace std;

Player::Player() : playerId(0), name("player"), order_list(new OrdersList()) {}

Player::Player(int playerID, string name)
    : playerId(playerID), name(name), order_list(new OrdersList()),
      hand(new Hand()) {
  // Deck deck;

  // Random index generator.
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 4);
  for (int i = 0; i < 15; i++) {
    int index = dist6(rng);
    // int index2 = dist6(rng);

    // Inserts random cards inside both deck and the hand.
    hand->insert(Type(index));
    // deck.insert(Type(index2));
  }
}

// constructor for player with all parameters, name id territories hand and
// orderlists
Player::Player(int playerID, string name, vector<Territory *> &territories,
               Hand *hand, OrdersList *orders)
    : playerId(playerID), name(name), territories(territories), hand(hand),
      order_list(orders) {}

// create copy player obj
Player::Player(const Player &p)
    : playerId(p.playerId), name(p.name), hand(new Hand(*(p.hand))),
      order_list(new OrdersList(*(p.order_list))) {
  for (Territory *t : p.territories) {
    this->addTerritory(new Territory(*t));
  }
}

// destructor
Player::~Player() {
  /*     for (Territory *t : territories)
      {
          delete t;
      } */
  delete order_list;
  delete hand;

  territories.clear();
}

// assign one Player object to another.
Player &Player::operator=(const Player &p) {
  playerId = p.playerId;
  name = p.name;
  for (Territory *t : p.territories) {
    this->addTerritory(new Territory(*t));
  }
  this->hand = new Hand(*(p.hand));
  this->order_list = new OrdersList(*(p.order_list));
  return *this;
}

vector<Territory *> Player::toDefend() { return this->territories; }

vector<Territory *> Player::toAttack() { return this->territories; }
// test
void Player::issueOrder(const Order &o) {
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

void Player::addTerritory(Territory *t) {
  territories.push_back(t); // to add t to player
}

// getters
int Player::getPlayerId() { return playerId; }

string Player::getName() { return name; }

Hand *Player::getHand() { return &*hand; }
OrdersList *Player ::getPlayerOrderList() { return order_list; }

vector<Territory *> Player::getTerritories() { return territories; }

// setters
void Player::setPlayerOrderList(OrdersList *orders) {
  this->order_list = orders;
}

void Player::setTerritories(vector<Territory *> t) { this->territories = t; }

ostream &operator<<(ostream &os, Player &p) {
  return os << "Name: " << p.getName() << " ID: " << p.getPlayerId();
}