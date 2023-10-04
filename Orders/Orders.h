#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include<string>
using namespace std;

namespace std
{

    class Order
    {
        public:

            std::string description;
            std::string name;

            Order() = default;
            Order(Order const &other);
            bool validate();
            void execute();
            friend ostream &operator<<(ostream &os, const Order &order);
    };
    class OrdersList
    {
        public:

            vector<std::shared_ptr<Order>> list;

            OrdersList() = default;
            OrdersList(vector<shared_ptr<Order>>);//*
            OrdersList(const OrdersList &other);
            bool move(int index, int destination);
            bool remove(int index);
            void add(shared_ptr<Order> o);
            bool isEmpty()const{return (this->list.size()==0);};
            int size()const{return this->list.size();};

            std::shared_ptr<Order>& operator[](const int i);
            friend ostream &operator<<(ostream &os, const OrdersList& olist);
    };

    class Deploy : public Order
    {
        public:
            Deploy();
    };

    class Advance : public Order
    {
        public:
            Advance();
    };

    class Bomb : public Order
    {
        public:
            Bomb();
    };

    class Blockade : public Order
    {
        public:
            Blockade();
    };

    class Airlift : public Order
    {
        public:
            Airlift();
    };

    class Negotiate : public Order
    {
        public:
            Negotiate();
    };
}