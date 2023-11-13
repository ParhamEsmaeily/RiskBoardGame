#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Orders.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"
using namespace std;

void testOrdersList()
{
    // Test: load map
    const shared_ptr<Map> gameMap = MapLoader::loadMap("../maps/world.map");
    SharedTerritoriesVector territories = Map::getAllTerritories(*gameMap);
    // create two normal players and one neutral player with a set of territories and cards already
    vector<Player> players;
    Player *p1 = new Player(1, "p1");
    Player *p2 = new Player(2, "p2");
    Player *neutral = new Player(true);
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(neutral);
    for (int i = 0; i < territories.size(); i++)
        players[i % players.size()].addTerritory(&*territories[i]);
    // Test validation of every order
    // Deploy
    Deploy *deploy = new Deploy(p1, gameMap.get(), &*territories[0], 6);
    cout << "validation of:\n"
         << deploy << "\n"
         << deploy->validate() << endl;
    Deploy *deploy2 = new Deploy(p1, gameMap.get(), &*territories[1], 6);
    cout << "validation of:\n"
         << deploy << "\n"
         << deploy->validate() << endl;
    // Advance
    Advance *advance = new Advance(p1, gameMap.get(), p2, &*territories[0], &*territories[1], 6);
    cout << "validation of:\n"
         << advance << "\n"
         << advance->validate() << endl;
    Advance *advance2 = new Advance(p1, gameMap.get(), p2, &*territories[0], &*territories[7], 6);
    cout << "validation of:\n"
         << advance << "\n"
         << advance->validate() << endl;
    // Test advance and combat mechanics
    p2->setTerritoryUnits(&*territories[1], 20);
    p1->setTerritoryUnits(&*territories[0], 10);
    Advance *advance3 = new Advance(p1, gameMap.get(), p2, &*territories[0], &*territories[1], 6);
    cout << advance3;
    advance3->execute();
    cout << "p1(attacker) lost the battle. p2 has " << p2->getTerritoryUnits(&*territories[1]) << " units left from an initial 20." << endl;
    // Show territory transfer from resulting combat
    p2->setTerritoryUnits(&*territories[1], 3);
    p1->setTerritoryUnits(&*territories[0], 50);
    Advance *advance4 = new Advance(p1, gameMap.get(), p2, &*territories[0], &*territories[1], 20);
    cout << advance;
    advance4->execute();
    cout << "p1(attacker) won the battle. check if p1 now owns territories[1]: " << p1->owns(&*territories[1]) << endl;
    // Simulate end of turn, show that players who have conquered a territory get a card
    cout << "p1 should now be listed to receive an extra card at end of turn. Check: " << p1->conqueredThisTurn() << endl;
    // Show negotiate prevents attacks between players
    Card *negocard = new Card(CardType::diplomacy);
    Negotiate *nego = new Negotiate(p1, gameMap.get(), p2, negocard);
    Advance *advance5 = new Advance(p1, gameMap.get(), p2, &*territories[1], &*territories[4], 1);
    cout << "p1 just negotiated with p2. check if they can now attack them: " << advance5->validate() << endl;
    // Show that blockade transfers to neutral player
    Card *blocard = new Card(CardType::blockade);
    Blockade *blockade = new Blockade(p1, gameMap.get(), neutral, blocard, &*territories[1]);
    cout << "Blockade from p1 on territory 1 should give the territory to the neutral player. Check: " << neutral->owns(&*territories[1]);

    // deletes
    delete p1;
    delete p2;
    delete neutral;
    delete advance;
    delete advance2;
    delete advance3;
    delete advance4;
    delete advance5;
    delete deploy;
    delete deploy2;
    delete negocard;
    delete nego;
    delete blocard;
    delete blockade;
}
