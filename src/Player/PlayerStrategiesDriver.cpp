#include "Player.h"
#include "GameEngine.h"

#include "PlayerStrategies.h"

void testPlayerStrategies()
{
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
    p2->setStrategy(aggressive);
    p3->setStrategy(benevolent);
    p4->setStrategy(neutral);

    p1->setTerritories(territoriesInFirstContinent);
    p2->setTerritories(territoriesInSecondContinent);
    p3->setTerritories(territoriesInThirdContinent);
    p4->setTerritories(territoriesInFourthContinent);

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
    const auto strategies = std::vector<PlayerStrategy *>{human, aggressive, benevolent};

    std::cout << "Testing Player Strategies" << std::endl;
    std::cout << "-------------------------" << std::endl;

    std::cout << "Strategies:" << std::endl;
    for (const auto &strategy : strategies)
    {
        std::cout << *strategy << std::endl;
    }

    std::cout << "Players:" << std::endl;
    for (const auto &player : players)
    {
        std::cout << *player << std::endl;
    }

    std::cout << "Testing to_defend():" << std::endl;
    for (const auto &player : players)
    {
        std::cout << *player << std::endl;
        std::cout << "to_defend():" << std::endl;
        for (const auto &territory : player->toDefend())
        {
            std::cout << *territory << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Testing to_attack():" << std::endl;
    for (const auto &player : players)
    {
        std::cout << *player << std::endl;
        std::cout << "to_attack():" << std::endl;
        for (const auto &territory : player->toAttack(*map))
        {
            std::cout << *territory << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Testing issue_order():" << std::endl;
    for (const auto &player : players)
    {
        std::cout << *player << std::endl;
        std::cout << "issue_order():" << std::endl;
        player->issueOrder(*map, players);
        std::cout << std::endl;
    }

    std::cout << "Changing Player Strategies" << std::endl;
    std::cout << "--------------------------" << std::endl;

    p1->setStrategy(benevolent);
    p2->setStrategy(human);
    p3->setStrategy(aggressive);
    p4->setStrategy(cheater);

    std::cout << "Players:" << std::endl;
    for (const auto &player : players)
    {
        std::cout << *player << std::endl;
    }

    std::cout << "Testing issue_order():" << std::endl;
    for (const auto &player : players)
    {
        std::cout << *player << std::endl;
        std::cout << "issue_order():" << std::endl;
        player->issueOrder(*map, players);
        std::cout << std::endl;
    }

    std::cout << "Completed Player Strategies Test" << std::endl;

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