#pragma once
#include "../LoggingObserver/LoggingObserver.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

void testOrdersList();

class Order {
public:
  std::string description;
  std::string name;

  Order() = default;
  Order(std::string, std::string);
  Order(Order const &other);
  /**Validate and execute will eventually be virtual functions overridden by
   * every child class, but no pointin doing it now, as we have no real
   * implementation.*/
  bool validate();
  void execute();
  friend ostream &operator<<(ostream &os, const Order &order);
};

class OrdersList : protected ILoggable, protected Subject {
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
  /*
      Overriden by all type of orders subclasses.
      Cannot be pure virtual, as we do not want an abstract class.
  */
  virtual std::string stringToLog() const;

  std::shared_ptr<Order> &operator[](const int i);
  friend ostream &operator<<(ostream &os, const OrdersList &olist);
};

class Deploy : public Order, private ILoggable, private Subject {
public:
  Deploy();
  std::string stringToLog() const;
};

class Advance : public Order, private ILoggable, private Subject {
public:
  Advance();
  std::string stringToLog() const;
};

class Bomb : public Order, private ILoggable, private Subject {
public:
  Bomb();
  std::string stringToLog() const;
};

class Blockade : public Order, private ILoggable, private Subject {
public:
  Blockade();
  std::string stringToLog() const;
};

class Airlift : public Order, private ILoggable, private Subject {
public:
  Airlift();
  std::string stringToLog() const;
};

class Negotiate : public Order, private ILoggable, private Subject {
public:
  Negotiate();
  std::string stringToLog() const;
};