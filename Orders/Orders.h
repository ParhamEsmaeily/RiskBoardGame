#ifndef ORDERS
#define ORDERS

#include <iostream>
using std::ostream;
#include <vector>
#include <memory>
#include<string>
using namespace std;

namespace std
{
    class OrdersList
    {
        private:

            unique_ptr<vector<Order>> list; //the actual list of orders
            OrdersList(OrdersList &o); //copy constructor

        public:

            bool move(int index, int destination);
            bool remove(int index);
            bool add();
            bool isEmpty();
            unique_ptr<vector<Order>> get_list(){return unique_ptr<vector<Order>>temp(this->list);}//I do not know how to write the getter for this
    };

    class Order
    {
        private:

            string description;
            string name;       

        public:

            virtual bool validate();
            virtual void execute();
            void set_desc(string s){this->description=s;}
            void set_name(string s){this->name=s;}
            string get_desc(){return this->description;}
            string get_name(){return this->name;}
    };

    class Deploy : Order
    {
        public:

            bool validate() override;
            void execute() override;
    };

    class Advance : Order
    {
        public:

            bool validate() override;
            void execute() override;
    };

    class Bomb : Order
    {
        public:

            bool validate() override;
            void execute() override;
    };

    class Blockade : Order
    {
        public:

            bool validate() override;
            void execute() override;
    };

    class Airlift : Order
    {
        public:

            bool validate() override;
            void execute() override;
    };

    class Negotiate : Order
    {
        public:

            bool validate() override;
            void execute() override;
    };
}
#endif