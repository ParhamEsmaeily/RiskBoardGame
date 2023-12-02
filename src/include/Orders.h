#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Cards.h"
#include "LoggingObserver.h"
#include "Map.h"
#include "Player.h"

using namespace std;

void testOrdersList();
class Order;
class Player;
class OrdersList;

class Order
{
public:
  std::string description;
  std::string name;
  Player *issuer;
  const Map *map;

  Order() = delete;
  Order(const Order &other);
  Order(std::string name, std::string description);
  Order(Player *player, const Map *map, std::string name,
        std::string description);
  ~Order();

  friend std::ostream &operator<<(std::ostream &os, const Order &order);

  Order &operator=(const Order &other);
  bool operator==(const Order &other);
  bool operator!=(const Order &other);

  virtual bool validate();
  virtual void execute();
};

class OrdersList : protected ILoggable, protected Subject
{
public:
  vector<Order *> list;

  OrdersList() = default;
  OrdersList(vector<Order *>);
  OrdersList(const OrdersList &other);
  ~OrdersList();

  Order *operator[](const int i);

  friend ostream &operator<<(ostream &os, const OrdersList &olist);

  bool move(int index, int destination);
  bool remove(int index);
  void add(Order *o);
  bool isEmpty() const { return (this->list.size() == 0); };
  int size() const { return this->list.size(); };

  std::string stringToLog() const override;
};

class Advance : public Order, private ILoggable, private Subject
{
public:
  const Territory *source_terr;
  const Territory *dest_terr;
  Player *target_player;
  int units_deployed;

  Advance() = delete;
  Advance(const Advance &other);
  Advance(Player *player, const Map *map, Player *target_player,
          const Territory *source, const Territory *dest, int units);
  ~Advance();

  Advance &operator=(const Advance &other);
  bool operator==(const Advance &other);
  bool operator!=(const Advance &other);

  friend ostream &operator<<(ostream &os, const Advance &order);

  bool validate() override;
  void execute() override;

  std::string stringToLog() const override;
};

class Airlift : public Order, private ILoggable, private Subject
{
public:
  const Territory *source_terr;
  const Territory *dest_terr;
  Card *card;
  int units_deployed;

  Airlift() = delete;
  Airlift(const Airlift &other);
  Airlift(Player *player, const Map *map, Card *card, const Territory *source,
          const Territory *dest, int units);
  ~Airlift();

  Airlift &operator=(const Airlift &other);
  bool operator==(const Airlift &other);
  bool operator!=(const Airlift &other);

  friend ostream &operator<<(ostream &os, const Airlift &order);

  bool validate() override;
  void execute() override;

  std::string stringToLog() const override;
};

class Bomb : public Order, private ILoggable, private Subject
{
public:
  const Territory *dest_terr;
  Card *card;
  Player *target_player;

  Bomb() = delete;
  Bomb(const Bomb &other);
  Bomb(Player *player, const Map *map, Player *target, Card *card,
       const Territory *dest);
  ~Bomb();

  Bomb &operator=(const Bomb &other);
  bool operator==(const Bomb &other);
  bool operator!=(const Bomb &other);

  friend ostream &operator<<(ostream &os, const Bomb &order);

  bool validate() override;
  void execute() override;

  std::string stringToLog() const override;
};

class Blockade : public Order, private ILoggable, private Subject
{
public:
  const Territory *dest_terr;
  Player *neutral_player;
  Card *card;

  Blockade() = delete;
  Blockade(const Blockade &other);
  Blockade(Player *player, const Map *map, Player *neutral, Card *card,
           const Territory *dest);
  ~Blockade();

  Blockade &operator=(const Blockade &other);
  bool operator==(const Blockade &other);
  bool operator!=(const Blockade &other);

  friend ostream &operator<<(ostream &os, const Blockade &order);

  bool validate() override;
  void execute() override;

  std::string stringToLog() const override;
};

class Deploy : public Order, private ILoggable, private Subject
{
public:
  const Territory *dest_terr;
  int units_deployed;
  Deploy() = delete;
  Deploy(const Deploy &other);
  Deploy(Player *player, const Map *map, const Territory *dest, int units);
  ~Deploy();

  Deploy &operator=(const Deploy &other);
  bool operator==(const Deploy &other);
  bool operator!=(const Deploy &other);

  friend ostream &operator<<(ostream &os, const Deploy &order);

  bool validate() override;
  void execute() override;

  std::string stringToLog() const override;
};

class Negotiate : public Order, private ILoggable, private Subject
{
public:
  Player *target_player;
  Card *card;

  Negotiate() = delete;
  Negotiate(const Negotiate &other);
  Negotiate(Player *issuer, const Map *map, Player *target, Card *card);
  ~Negotiate();

  Negotiate &operator=(const Negotiate &other);
  bool operator==(const Negotiate &other);
  bool operator!=(const Negotiate &other);
  friend ostream &operator<<(ostream &os, const Negotiate &order);

  bool validate() override;
  void execute() override;

  std::string stringToLog() const override;
};