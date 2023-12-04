#include "Player.h"
#include "GameEngine.h"

#include "PlayerStrategies.h"

void testPlayerStrategies()
{
    GameEngine *engine = new GameEngine();
    const auto map = MapLoader::loadMap("maps/world.map");

    const auto continents = Map::getAllContinents(*map);
    std::vector<Territory *> territoriesInFirstContinent, territoriesInSecondContinent, territoriesInThirdContinent, territoriesInFourthContinent;

    // assigns territories to players (3 players, 3 continents)
    if (continents.size() >= 3)
    {
        auto tempTerritories = Map::getAllTerritoriesInContinent(*map, *continents[0]);
        for (const auto &territory : tempTerritories)
        {
            territoriesInFirstContinent.push_back(territory.get());
        }

        tempTerritories = Map::getAllTerritoriesInContinent(*map, *continents[1]);
        for (const auto &territory : tempTerritories)
        {
            territoriesInSecondContinent.push_back(territory.get());
        }

        tempTerritories = Map::getAllTerritoriesInContinent(*map, *continents[2]);
        for (const auto &territory : tempTerritories)
        {
            territoriesInThirdContinent.push_back(territory.get());
        }

        tempTerritories = Map::getAllTerritoriesInContinent(*map, *continents[3]);
        for (const auto &territory : tempTerritories)
        {
            territoriesInFourthContinent.push_back(territory.get());
        }
    }

    Player *p1 = new Player(1, "Parham");
    Player *p2 = new Player(2, "Augusto");
    Player *p3 = new Player(3, "Maxime");
    Player *p4 = new Player(4, "Nikola");

    PlayerStrategy *human = new HumanPlayer();
    PlayerStrategy *aggressive = new AggressivePlayer();
    PlayerStrategy *benevolent = new BenevolentPlayer();
    PlayerStrategy *neutral = new NeutralPlayer();
    PlayerStrategy *cheater = new CheaterPlayer();

    p1->setStrategy(human);
    p2->setStrategy(neutral);
    p3->setStrategy(benevolent);
    p4->setStrategy(aggressive);

    p1->setTerritories(territoriesInFirstContinent);
    p2->setTerritories(territoriesInSecondContinent);
    p3->setTerritories(territoriesInThirdContinent);
    p4->setTerritories(territoriesInFourthContinent);

    for (Territory *t : territoriesInFirstContinent)
    {
        p1->setTerritoryUnits(t, 2);
    }

    for (Territory *t : territoriesInSecondContinent)
    {
        p2->setTerritoryUnits(t, 2);
    }

    for (Territory *t : territoriesInThirdContinent)
    {
        p3->setTerritoryUnits(t, 2);
    }

    for (Territory *t : territoriesInFourthContinent)
    {
        p4->setTerritoryUnits(t, 2);
    }

    // initial setup

    p1->getHand()->insert(Card(CardType::reinforcement));
    p1->getHand()->insert(Card(CardType::reinforcement));
    p1->getHand()->random_insert(3);

    p2->getHand()->insert(Card(CardType::reinforcement));
    p2->getHand()->insert(Card(CardType::reinforcement));
    p2->getHand()->random_insert(3);

    p3->getHand()->insert(Card(CardType::reinforcement));
    p3->getHand()->insert(Card(CardType::reinforcement));
    p3->getHand()->random_insert(3);

    p4->getHand()->insert(Card(CardType::reinforcement));
    p4->getHand()->insert(Card(CardType::reinforcement));
    p4->getHand()->random_insert(3);

    const auto players = std::vector<Player *>{p1, p2, p3, p4};
    const auto strategies = std::vector<PlayerStrategy *>{human, aggressive, benevolent, neutral, cheater};

    std::cout << "Testing Player Strategies" << std::endl;
    std::cout << "-------------------------" << std::endl;

    engine->executeCommand("loadmap");
    engine->executeCommand("validate");
    engine->executeCommand("addplayers");
    engine->executeCommand("assigncountries");

    engine->mainGameLoop(players, *map, 2);

    delete engine;

    std::cout << "Changing Player Strategies" << std::endl;
    std::cout << "--------------------------" << std::endl;

    p1->setStrategy(benevolent);
    p2->setStrategy(aggressive);
    p3->setStrategy(neutral);
    p4->setStrategy(cheater);

    engine = new GameEngine();

    engine->executeCommand("loadmap");
    engine->executeCommand("validate");
    engine->executeCommand("addplayers");
    engine->executeCommand("assigncountries");

    engine->mainGameLoop(players, *map, 2);

    std::cout << "Completed Player Strategies Test" << std::endl;

    delete engine;
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete human;
    delete aggressive;
    delete benevolent;
    delete neutral;
    delete cheater;
}