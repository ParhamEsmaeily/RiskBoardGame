#include "Player.h"
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Cards/Cards.h"

using namespace std;

void testPlayers()
{

    Player *p1 = new Player(1, "Parham");
    Player *p2 = new Player(2, "hashem");
    const auto map = MapLoader::loadMap("maps/world.map");
    const auto territories = Map::getAllTerritories(*map);

    // for (auto &&territory : territories)
    // {
    //    p1-> addTerritory(&*territory);
    // }
    for (size_t index = 0; index < territories.size(); ++index)
    {
        Territory *territory = &*territories[index];
        if (index < territories.size() / 2)
        {
            p1->addTerritory(territory);
        }
        else
        {
            p2->addTerritory(territory);
        }
    }

    // p1->addTerritory(new Territory(territory *t)

    // cout<<p1 << "this is p1"<<endl;

    cout << "Player 1's territories:" << endl;
    for (Territory *t : p1->getTerritories())
    {
        cout << *t << endl;
    }
    cout << "Player 2's territories" << endl;
    for (Territory *t : p2->getTerritories())
    {
        cout << *t << endl;
    }

    //========================ORDER LIST+++++++++++
    // doesn't work yet
    //  cout << "Player's orders list:" << endl;
    Order test_order = Advance();

//    p1->issueOrder(test_order);

    cout << *(p1->getPlayerOrderList()) << endl;

    for (int i = 0; i < p1->getHand()->size(); i++)
    {
        cout << "\nthis is p1 gethand:  " << p1->getHand()->show_cards()[i] << endl;
        cout << "\nthis is p2 gethand:  " << p2->getHand()->show_cards()[i] << endl;
    }

    cout << "\nTerritories to defend:" << endl;
    for (Territory *t : p1->toDefend())
    {
        cout << *t << endl;
    }
    cout << "Territories to attack: uncomment this if needed" << endl;
//    for (Territory *t : p2->toAttack())
//    {
//        cout << *t << endl;
//    }

    // cleanup
    delete p1, p2;
    p1 = p2 = NULL;
}