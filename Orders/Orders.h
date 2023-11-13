#pragma once
#include "../LoggingObserver/LoggingObserver.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"

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
  Order(Player *player, const Map *map, std::string name, std::string description);
  ~Order();
  virtual bool validate();
  virtual void execute();
  friend std::ostream &operator<<(std::ostream &os, const Order &order);
  Order &operator=(const Order &other);
  bool operator==(const Order &other);
  bool operator!=(const Order &other);
};

class OrdersList : protected ILoggable, protected Subject
{
public:
  vector<std::shared_ptr<Order>> list;

  OrdersList() = default;
  OrdersList(vector<shared_ptr<Order>>); //*
  OrdersList(const OrdersList &other);
  bool move(int index, int destination);
  bool remove(int index);
  void add(const Order &o);
  bool isEmpty() const { return (this->list.size() == 0); };
  int size() const { return this->list.size(); };
  std::string stringToLog() const override;

  std::shared_ptr<Order> &operator[](const int i);
  friend ostream &operator<<(ostream &os, const OrdersList &olist);
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
  Advance(Player *player, const Map *map, Player *target_player, const Territory *source, const Territory *dest, int units);
  ~Advance();
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Advance &order);
  Advance &operator=(const Advance &other);
  bool operator==(const Advance &other);
  bool operator!=(const Advance &other);
  std::string stringToLog() const;
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
  Airlift(Player *player, const Map *map, Card *card, const Territory *source, const Territory *dest, int units);
  ~Airlift();
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Airlift &order);
  Airlift &operator=(const Airlift &other);
  bool operator==(const Airlift &other);
  bool operator!=(const Airlift &other);
  std::string stringToLog() const;
};

class Bomb : public Order, private ILoggable, private Subject
{
public:
  const Territory *dest_terr;
  Card *card;
  Player *target_player;
  Bomb() = delete;
  Bomb(const Bomb &other);
  Bomb(Player *player, const Map *map, Player *target, Card *card, const Territory *dest);
  ~Bomb();
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Bomb &order);
  Bomb &operator=(const Bomb &other);
  bool operator==(const Bomb &other);
  bool operator!=(const Bomb &other);
  std::string stringToLog() const;
};

class Blockade : public Order, private ILoggable, private Subject
{
public:
  const Territory *dest_terr;
  Player *neutral_player;
  Card *card;
  Blockade() = delete;
  Blockade(const Blockade &other);
  Blockade(Player *player, const Map *map, Player *neutral, Card *card, const Territory *dest);
  ~Blockade();
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Blockade &order);
  Blockade &operator=(const Blockade &other);
  bool operator==(const Blockade &other);
  bool operator!=(const Blockade &other);
  std::string stringToLog() const;
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
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Deploy &order);
  Deploy &operator=(const Deploy &other);
  bool operator==(const Deploy &other);
  bool operator!=(const Deploy &other);
  std::string stringToLog() const;
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
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Negotiate &order);
  Negotiate &operator=(const Negotiate &other);
  bool operator==(const Negotiate &other);
  bool operator!=(const Negotiate &other);
  std::string stringToLog() const;
};