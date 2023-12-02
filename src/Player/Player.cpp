#include <cstddef>
#include <map>
#include <set>

#include "Cards.h"
#include "Map.h"
#include "Orders.h"
#include "PlayerStrategies.h"

#include "Player.h"

using namespace std;

Player::Player() : playerId(0), name("player"), order_list(new OrdersList()) {}

Player::Player(int playerID, string name) // Default is neutral player strategy.
    : playerId(playerID), name(name), order_list(new OrdersList()),
      hand(new Hand()), m_strategy(ps::make_player_strat(StratType::Neutral))
{
}

/*
    Fully parameterized constructor.
*/
Player::Player(int playerID, string name, vector<Territory *> &territories,
               Hand *hand, OrdersList *orders, const StratType &strat)
    : playerId(playerID), name(name), territories(territories), hand(hand),
      order_list(orders), m_strategy(ps::make_player_strat(strat)) {}

Player::Player(bool isNeutral) : Player() { this->is_neutral = true; }

// Creates a copy of the player object.
Player::Player(const Player &p)
    : playerId(p.playerId), name(p.name), hand(new Hand(*(p.hand))),
      order_list(new OrdersList(*(p.order_list))),
      m_strategy(p.m_strategy->clone())
{
  for (Territory *t : p.territories)
  {
    this->addTerritory(new Territory(*t));
  }
}

// Destructor.
Player::~Player()
{
  delete order_list;
  delete hand;
  delete m_strategy;

  territories.clear();
}

// Assigns one Player object to another.
Player &Player::operator=(const Player &p)
{
  // Else memory leak when assignment.
  delete hand;
  delete order_list;
  delete m_strategy;

  playerId = p.playerId;
  name = p.name;
  for (Territory *t : p.territories)
  {
    this->addTerritory(new Territory(*t));
  }
  this->hand = new Hand(*(p.hand));
  this->order_list = new OrdersList(*(p.order_list));
  m_strategy = p.m_strategy->clone();
  return *this;
}

Player &Player::operator=(Player &&p)
{
  // Performs no operation if assigned to itself.
  if (this != &p)
  {
    // Delete allocated ptr first.
    delete order_list;
    delete hand;
    delete m_strategy;
    // Move the data.
    playerId = std::move(p.playerId);
    name = std::move(p.name);
    for (Territory *t : p.territories)
    {
      this->addTerritory(new Territory(*t));
    }
    // Move constructor here as well.
    hand = new Hand(std::move(*p.hand));
    // Move constructor not implemented here.
    order_list = new OrdersList(std::move(*p.order_list));
    m_strategy = p.m_strategy->clone();
  }
  return *this;
}

// check equality between player objects
bool Player::operator==(const Player &other)
{
  return playerId == other.playerId;
}

bool Player::operator!=(const Player &other) { return !(*this == other); }

vector<Territory *> Player::toDefend() { return m_strategy->to_defend(this); }

vector<Territory *> Player::toAttack(const Map &gameMap)
{
  return m_strategy->to_attack(gameMap, this);
}

void Player::issueOrder(const Map &gameMap, std::vector<Player *> players)
{
  m_strategy->issue_order(gameMap, this, players, this->toDefend(),
                          this->toAttack(gameMap));
}

void Player::addTerritory(Territory *t)
{
  t->setOwner(this);
  units_map[t->getName()] = 0;
  territories.push_back(t);
}

void Player::removeTerritory(const Territory *t)
{
  for (int i = 0; i < territories.size(); i++)
  {
    if (territories[i]->getName() == t->getName())
    {
      territories.erase(territories.begin() + i);
      territories[i]->setOwner(nullptr);
      break;
    }
  }
  units_map.erase(t->getName());
}

void Player::addAlly(const Player *p)
{
  this->allies.push_back(const_cast<Player *>(p));
}

void Player::resetTurnValues()
{
  this->conquered_this_turn = false;
  this->allies.clear();
}

bool Player::isAllied(Player *p)
{
  for (int i = 0; i < this->allies.size(); i++)
  {
    if (*p == *(allies[i]))
      return true;
  }
  return false;
}

bool Player::owns(const Territory *t) const
{
  return t->getOwner() == this;
}

// getters
int Player::getPlayerId() { return playerId; }

string Player::getName() { return name; }

StratType Player::getStrategyType() const { return m_strategy->type(); }

int Player::card_count(const CardType &type) const noexcept
{
  size_t count = 0;
  try
  {
    count = hand->card_count().at(type);
  }
  catch (const std::exception &)
  {
  }

  return count;
}

Hand *Player::getHand() { return &*hand; }
OrdersList *Player ::getPlayerOrderList() { return order_list; }

vector<Territory *> Player::getTerritories() { return territories; }

int Player::getTerritoryUnits(const Territory *t) const
{
  std::string str = t->getName();
  return units_map.at(str);
}

bool Player::conqueredThisTurn() { return conquered_this_turn; }

bool Player::isNeutral() { return this->is_neutral; }

// setters
void Player::setPlayerOrderList(OrdersList *orders)
{
  this->order_list = orders;
}

void Player::setTerritories(vector<Territory *> newTerritories)
{
  for (Territory *t : newTerritories)
  {
    addTerritory(t);
  }
}

ostream &operator<<(ostream &os, Player &p)
{
  return os << "{Name: " << p.getName() << ", ID: " << p.getPlayerId()
            << ", Strategy: " << p.getStrategyType() << "}";
}

void Player::setTerritoryUnits(const Territory *t, int units)
{
  units_map[t->getName()] = units;
}

void Player::setConqueredThisTurn(bool b) { this->conquered_this_turn = b; }

void Player::setStrategy(const PlayerStrategy *strat)
{
  // Proper deep copy is made this way.
  m_strategy = strat->clone();
}