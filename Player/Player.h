#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Cards.h"
#include "Map.h"
#include "Orders.h"


using namespace std;

void testPlayers();

class Hand;
class OrdersList;
class Order;
class Player
{

private:
  string name;
  Hand *hand;
  vector<Territory *> territories; // List of owned territories
  std::vector<Player*> allies; //list of allies for the turn
  OrdersList *order_list;
  std::unordered_map<string, int> units_map;
  int playerId;
  bool conquered_this_turn;
  bool is_neutral;

public:
  /**
   * default constructor
   */
  Player();

  // param constructor for ID and name
  Player(int playerID, string name);

  //Constructor for Neutral Player
  Player(bool isNeutral);

  // constructor for full player object
  Player(int playerID, string name, vector<Territory *> &territories,
         Hand *hand, OrdersList *orders);

  // copy constructor
  Player(const Player &p);

  // destructor
  ~Player();

  // assignment operator
  Player &operator=(const Player &p);

  // get
  int getPlayerId();
  OrdersList *getPlayerOrderList();
  Hand *getHand();
  string getName();
  vector<Territory *> getTerritories();
  int getTerritoryUnits(const Territory* t) const;
  bool isNeutral();
  bool conqueredThisTurn();

  // methods
  vector<Territory *> toAttack(const Map &gameMap); // return a list of territories to be Attacked
  vector<Territory *> toDefend();                   // return a list of territories to be defended
  void issueOrder(const Map &gameMap);              // uses the console to add orders to the player's list of orders
  void addTerritory(Territory *territory);          // add territories to player
  void removeTerritory(const Territory* t);  //removes territory and corresponding units from player
  bool owns(const Territory* t) const;
  void addAlly(const Player* p);
  bool isAllied(Player* p);

  // sets
  void setPlayerOrderList(OrdersList *orders);
  void setTerritories(vector<Territory *> t);
  void setTerritoryUnits(const Territory* t, int units);
  void setConqueredThisTurn(bool b);
  void resetTurnValues();
};

/**
 * Stream insertion operator
 */
ostream &operator<<(ostream &os, Player &p);