#pragma once

#include <vector>
#include <iostream>

#include "Cards.h"
#include "Orders.h"
#include "Map.h"

using namespace std;

void testPlayers();

class Hand;
class Player
{

private:
    string name;
    Hand *hand;
    vector<Territory *> territories; // List of owned territories
    OrdersList *order_list;
    int playerId;

public:
    /**
     * default constructor
     */
    Player();

    // param constructor for ID and name
    Player(int playerID, string name);

    // constructor for full player object
    Player(int playerID, string name, vector<Territory *> &territories, Hand *hand, OrdersList *orders);

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

    // methods

    vector<Territory *> toAttack(const Map &gameMap);          // return a list of territories to be Attacked
    vector<Territory *> toDefend();          // return a list of territories to be defended
    void issueOrder(const Map &gameMap);         // uses the console to add orders to the player's list of orders
    void addTerritory(Territory *territory); // add territories to player

    // sets
    void setPlayerOrderList(OrdersList *orders);
    void setTerritories(vector<Territory *> t);
};

/**
 * Stream insertion operator
 */
ostream &operator<<(ostream &os, Player &p);