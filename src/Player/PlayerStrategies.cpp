#include "PlayerStrategies.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include <algorithm>

namespace ps
{
  const std::string map(const StratType &type)
  {
    switch (type)
    {
    case StratType::Human:
      return "Human";
    case StratType::Aggressive:
      return "Aggressive";
    case StratType::Benevolent:
      return "Benevolent";
    case StratType::Neutral:
      return "Neutral";
    case StratType::Cheater:
      return "Cheater";
    default:
      return "Unknown";
    }
  }

  PlayerStrategy *make_player_strat(const StratType &type)
  {
    switch (type)
    {
    case StratType::Human:
      return new HumanPlayer();
    case StratType::Aggressive:
      return new AggressivePlayer();
    case StratType::Benevolent:
      return new BenevolentPlayer();
    case StratType::Neutral:
      return new NeutralPlayer();
    case StratType::Cheater:
      return new CheaterPlayer();
    default:
      return nullptr;
    }
  }

  Territory *parse_territory_name(std::vector<Territory *> territories, std::string terr_name)
  {
    // Parse strings to territory objects
    for (Territory *t : territories)
    {
      if (t->getName() == terr_name)
      {
        return t;
      }
    }

    return nullptr;
  }

  void random_deployment(const Map &gameMap, Player *player,
                         std::vector<Territory *> t_defend)
  {

    // Simple function to get the number of reinforcement cards left.
    auto get_no_reinf = [&player]()
    {
      return player->getHand()->card_count().at(CardType::reinforcement);
    };

    int no_reinforcements = get_no_reinf();

    std::random_device rd;  // Generates a seed.
    std::mt19937 rng(rd()); // Random seed
    std::uniform_int_distribution<> random_idx(0, t_defend.size() - 1);
    // Repeatedly randomly add soldiers to territories.
    while (no_reinforcements > 0)
    {
      std::uniform_int_distribution<> random_no_soldiers(1, no_reinforcements);

      const int idx = random_idx(rng);
      const int no_soldiers = random_no_soldiers(rng);

      Deploy *order = new Deploy(player, &gameMap, t_defend[idx], no_soldiers);
      player->getPlayerOrderList()->add(order);

      no_reinforcements = get_no_reinf();
    }
  }

  Territory *find_strongest_territory(const Map &map, Player *player)
  {
    Territory *strongest_t = nullptr;
    int no_units = -1;
    for (auto *t : player->getTerritories())
    {
      if (no_units < player->getTerritoryUnits(t))
      {
        no_units = player->getTerritoryUnits(t);
        strongest_t = t;
      }
    }

    return strongest_t;
  }

  Territory *find_strongest_territory_from_territories(const Map &map, Player *player, std::vector<Territory *> territories)
  {
    Territory *strongest_t = nullptr;
    int no_units = -1;
    for (auto *t : territories)
    {
      // count if any enemy adjacent territories.
      bool has_enemy_adjacent = false;
      for (const std::shared_ptr<Territory> &n :
           Map::getAdjacentTerritories(map, t->getName()))
      {
        if (!player->owns(n.get()))
        {
          has_enemy_adjacent = true;
          break;
        }
      }

      if (no_units < player->getTerritoryUnits(t) && has_enemy_adjacent)
      {
        no_units = player->getTerritoryUnits(t);
        strongest_t = t;
      }
    }

    return strongest_t;
  }

  Territory *strong_deployment(const Map &gameMap, Player *player, std::vector<Territory *> t_defend, int *deployed)
  {
    Territory *strongest_t = find_strongest_territory_from_territories(gameMap, player, t_defend);

    // No of reinforcement cards.
    auto card_count = player->getHand()->card_count();
    *(deployed) = card_count[CardType::reinforcement];

    Deploy *order = new Deploy(player, &gameMap, strongest_t, *deployed);
    player->getPlayerOrderList()->add(order);

    return strongest_t;
  }

  void weak_deployment(const Map &gameMap, Player *player, std::vector<Territory *> t_defend)
  {
    int no_reinforcement_cards =
        player->getHand()->card_count().at(CardType::reinforcement);

    // Very slow O(n^2). Optimization necessary.
    // Increment add to the weakest countries.
    while (no_reinforcement_cards > 0)
    {
      Territory *territory = *std::min_element(
          t_defend.begin(), t_defend.end(),
          [&player](Territory *t1, Territory *t2)
          {
            return player->getTerritoryUnits(t1) < player->getTerritoryUnits(t2);
          });
      Deploy *order = new Deploy(player, &gameMap, territory, 1);
      player->getPlayerOrderList()->add(order);
      no_reinforcement_cards--;
    }
  }

  const std::vector<Territory *> enemy_adjacent_territories(const Map &gameMap,
                                                            Player *player)
  {
    std::vector<Territory *> territories_to_attack;

    // For each adjacent and not owned by the player.
    for (Territory *t : player->getTerritories())
    {
      for (std::shared_ptr<Territory> n :
           Map::getAdjacentTerritories(gameMap, t->getName()))
      {
        if (!player->owns(n.get()) &&
            territories_to_attack.end() ==
                std::find(territories_to_attack.begin(),
                          territories_to_attack.end(), n.get()))
        {
          territories_to_attack.push_back(n.get());
          break;
        }
      }
    }

    return territories_to_attack;
  }

  const std::vector<Territory *> enemy_adjacent_territories_from_territory(const Map &gameMap,
                                                                           Player *player, Territory *t)
  {
    std::vector<Territory *> territories_to_attack;

    // For each adjacent and not owned by the player.
    for (std::shared_ptr<Territory> n :
         Map::getAdjacentTerritories(gameMap, t->getName()))
    {
      if (!player->owns(n.get()) &&
          territories_to_attack.end() ==
              std::find(territories_to_attack.begin(),
                        territories_to_attack.end(), n.get()))
      {
        territories_to_attack.push_back(n.get());
        break;
      }
    }

    return territories_to_attack;
  }

  void random_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> toAttack, std::vector<Territory *> toDefend, const bool &make_harm)
  {
    std::random_device rd;  // Generates a seed.
    std::mt19937 rng(rd()); // Random seed

    /*
        Calls each order depending on the value sampled from the uniform
        distribution.
      */

    // Count - 1 for distribution.
    auto get_count = [&player](CardType type)
    {
      return player->card_count(type);
    };

    if (make_harm)
    {
      // Getting a random over the number of cards available.
      std::uniform_int_distribution<int> airlift(0, get_count(CardType::airlift));
      std::uniform_int_distribution<int> bomb(0, get_count(CardType::bomb));
      std::uniform_int_distribution<int> blockade(0, get_count(CardType::blockade));

      int no_airlift = airlift(rng);
      int no_bomb = bomb(rng);
      int no_blockade = blockade(rng);
      for (int i = 0; i < no_airlift; i++)
      {
        if (toDefend.size() == 0)
          break;

        std::uniform_int_distribution<int> random_idx(0, toDefend.size() - 1);
        const int max_no_soldiers = player->getTerritoryUnits(toDefend[random_idx(rng)]);

        if (max_no_soldiers == 0)
          continue;

        std::uniform_int_distribution<int> random_no_soldiers(1, max_no_soldiers);

        const int idx = random_idx(rng);
        const int no_soldiers = random_no_soldiers(rng);

        const std::vector<Territory *> attackable_territories = enemy_adjacent_territories_from_territory(gameMap, player, toDefend[idx]);

        if (attackable_territories.size() == 0)
          continue;

        Airlift *airlift = new Airlift(player, &gameMap, toDefend[idx], attackable_territories[0], no_soldiers);
        player->getPlayerOrderList()->add(airlift);
      }
      for (int i = 0; i < no_bomb; i++)
      {
        if (toAttack.size() == 0)
          break;

        std::uniform_int_distribution<int> random_idx(0, toAttack.size() - 1);

        Bomb *bomb = new Bomb(player, &gameMap, toAttack[random_idx(rng)]->getOwner(), toAttack[random_idx(rng)]);
        player->getPlayerOrderList()->add(bomb);
      }
      for (int i = 0; i < no_blockade; i++)
      {
        if (toAttack.size() == 0)
          break;

        std::uniform_int_distribution<int> random_idx(0, toAttack.size() - 1);

        Blockade *blockade = new Blockade(player, &gameMap, nullptr, toAttack[random_idx(rng)]);
        player->getPlayerOrderList()->add(blockade);
      }
    }

    std::uniform_int_distribution<> diplomacy(0, get_count(CardType::diplomacy));
    int no_diplomacy = diplomacy(rng);
    for (int i = 0; i < no_diplomacy; i++)
    {
      if (players.size() == 0)
        break;

      Player *nego_player = nullptr;

      for (Player *p : players)
      {
        if (p != player)
        {
          nego_player = p;
          break;
        }
      }

      Negotiate *negotiate = new Negotiate(player, &gameMap, nego_player);
      player->getPlayerOrderList()->add(negotiate);
    }
  }
} // namespace ps

std::ostream &operator<<(std::ostream &os, const StratType &strat)
{
  os << ps::map(strat);
  return os;
}

std::ostream &operator<<(std::ostream &os, const PlayerStrategy &strategy)
{
  os << "{Type: " << strategy.type() << "}";
  return os;
}

const StratType HumanPlayer::type() const noexcept { return StratType::Human; }

void HumanPlayer::issue_order(
    const Map &gameMap, Player *player, std::vector<Player *> players,
    std::vector<Territory *> territoriesToDefend,
    std::vector<Territory *> territoriesToAttack) const noexcept
{
  // print all necessary info
  cout << "Current active players:" << endl;
  for (Player *p : players)
  {
    cout << *p << endl;
  }

  cout << player->getName() << "'s territories to defend:" << endl;
  for (Territory *t : territoriesToDefend)
  {
    cout << *t << endl;
  }

  cout << player->getName() << "'s territories to attack:" << endl;
  for (Territory *t : territoriesToAttack)
  {
    cout << *t << endl;
  }

  // card_count implemented inside Card.

  // Create a map of card types and their count of the player's hand
  map<CardType, int> cards_count;
  int nb_reinforcement_cards = 0;
  for (CardType card : player->getHand()->show_cards())
  {
    if (card != CardType::reinforcement)
    {
      cards_count[card]++;
    }
    else
    {
      nb_reinforcement_cards++;
    }
  }

  // while the user doesn't type 'end turn', keep asking for orders
  string input;
  while (input != "end turn")
  {
    /*
    / Issue Deploy() orders until the player has no more reinforcement cards
    */
    while (nb_reinforcement_cards > 0)
    {
      string terr_name;
      string str_reinforcements;
      bool orderAdded = false;

      cout << player->getName()
           << "'s turn, choose a territory to deploy reinforcements by writing "
              "the name: ";
      getline(cin, terr_name);
      cout << endl;
      cout << "Choose the number of reinforcements to send ("
           << nb_reinforcement_cards << " available): ";
      getline(cin, str_reinforcements);
      cout << endl;

      // if num_reinforcements is not a number, ask again
      if (!std::all_of(str_reinforcements.begin(), str_reinforcements.end(), ::isdigit))
      {
        cout << "Invalid number of reinforcements." << endl;
        continue;
      }

      // add to orders list the order to deploy the number of reinforcements to
      // the territory
      for (Territory *t : territoriesToDefend)
      {
        if (t->getName() == terr_name)
        {
          Deploy *order = new Deploy(player, &gameMap, t, std::stoi(str_reinforcements));
          player->getPlayerOrderList()->add(order);
          orderAdded = true;
          break;
        }
      }

      int num_reinforcements = std::stoi(str_reinforcements);
      if (orderAdded && num_reinforcements <= nb_reinforcement_cards && num_reinforcements > 0)
      {
        nb_reinforcement_cards -= num_reinforcements;
      }
      else
      {
        cout << "Invalid order." << endl;
      }
    }

    /*
    / Other orders
    */
    while (true)
    {

      // print all cards in hand
      cout << player->getName() << "'s cards in hand:" << endl;
      cout << "advance: infinite" << endl;
      for (auto const &[card, count] : cards_count)
      {
        cout << cd::map(card) << ": " << count << " available" << endl;
      }

      // ask for an order
      cout << "Enter an order (or 'end turn'):";
      getline(cin, input);

      // create an order based on the input
      if (input == "end turn")
      {
        break;
      }
      else if (input == "advance")
      {
        string terr_1;
        string terr_2;
        string str_num_armies;

        cout << "Choose a territory to attack from: ";
        getline(cin, terr_1);
        cout << endl;

        cout << "Choose a territory to attack: ";
        getline(cin, terr_2);
        cout << endl;

        cout << "Choose the number of armies to send: ";
        getline(cin, str_num_armies);
        cout << endl;

        // if num_armies is not a number, ask again
        if (!std::all_of(str_num_armies.begin(), str_num_armies.end(),
                         ::isdigit))
        {
          cout << "Invalid number of armies." << endl;
          continue;
        }

        // Parse strings to territory objects
        std::vector<Territory *> allPossibleAdvanceTerritories;
        allPossibleAdvanceTerritories.insert(allPossibleAdvanceTerritories.end(), territoriesToDefend.begin(), territoriesToDefend.end());
        allPossibleAdvanceTerritories.insert(allPossibleAdvanceTerritories.end(), territoriesToAttack.begin(), territoriesToAttack.end());

        Territory *source = ps::parse_territory_name(territoriesToDefend, terr_1);
        Territory *dest = ps::parse_territory_name(allPossibleAdvanceTerritories, terr_2);

        // find the target player
        Player *target_player = nullptr;

        for (size_t i = 0; i < players.size(); i++)
        {
          if (players[i]->owns(dest))
          {
            target_player = players[i];
            break;
          }
        }

        Advance *order = new Advance(player, &gameMap, source, dest, std::stoi(str_num_armies));
        player->getPlayerOrderList()->add(order);
      }
      else if (input == "bomb" && cards_count[CardType::bomb] > 0)
      {
        string terr_to_bomb;

        cout << "Choose a territory to bomb: ";
        getline(cin, terr_to_bomb);
        cout << endl;

        Territory *target = ps::parse_territory_name(territoriesToAttack, terr_to_bomb);
        if (target == nullptr)
        {
          cout << "Invalid target territory." << endl;
          continue;
        }

        Bomb *order = new Bomb(player, &gameMap, target->getOwner(), target);
        player->getPlayerOrderList()->add(order);
        cards_count[CardType::bomb]--;
      }
      else if (input == "blockade" && cards_count[CardType::blockade] > 0)
      {
        string terr_to_blockade;

        cout << "Choose a territory to blockade: ";
        getline(cin, terr_to_blockade);
        cout << endl;

        Territory *target = ps::parse_territory_name(territoriesToAttack, terr_to_blockade);
        if (target == nullptr)
        {
          cout << "Invalid target territory." << endl;
          continue;
        }

        Blockade *order = new Blockade(player, &gameMap, nullptr, target);
        player->getPlayerOrderList()->add(order);
        cards_count[CardType::blockade]--;
      }
      else if (input == "airlift" && cards_count[CardType::airlift] > 0)
      {
        string terr_to_airlift_from;
        string terr_to_drop_airlift;

        cout << "Choose a territory to airlift from: ";
        getline(cin, terr_to_airlift_from);
        cout << endl;

        cout << "Choose a territory to drop airlift: ";
        getline(cin, terr_to_drop_airlift);
        cout << endl;

        Territory *source = ps::parse_territory_name(territoriesToDefend, terr_to_airlift_from);
        if (source == nullptr)
        {
          cout << "Invalid source territory." << endl;
          continue;
        }

        Territory *target = ps::parse_territory_name(territoriesToAttack, terr_to_drop_airlift);
        if (target == nullptr)
        {
          cout << "Invalid target territory." << endl;
          continue;
        }

        Airlift *order = new Airlift(player, &gameMap, source, target, player->getTerritoryUnits(source));
        player->getPlayerOrderList()->add(order);
        cards_count[CardType::airlift]--;
      }
      else if (input == "negotiate" && cards_count[CardType::diplomacy] > 0)
      {
        string player_to_negotiate_with;
        Player *target_player = nullptr;

        cout << "Choose a player to negotiate with: ";
        getline(cin, player_to_negotiate_with);
        cout << endl;

        for (Player *p : players)
        {
          if (p->getName() == player_to_negotiate_with)
          {
            target_player = p;
            break;
          }
        }

        if (target_player == nullptr)
        {
          cout << "Invalid player." << endl;
          continue;
        }

        Negotiate *order = new Negotiate(player, &gameMap, target_player);
        player->getPlayerOrderList()->add(order);
        cards_count[CardType::diplomacy]--;
      }
      else
      {
        cout << "Invalid order." << endl;
      }
    }
  }
}

std::vector<Territory *> HumanPlayer::to_attack(const Map &gameMap,
                                                Player *player) const noexcept
{
  // Could use adjacent territories instead.

  std::vector<Territory *> territoriesToAttack;

  for (Territory *t : player->getTerritories())
  {
    for (std::shared_ptr<Territory> n :
         Map::getAdjacentTerritories(gameMap, t->getName()))
    {
      if (!player->owns(n.get()) &&
          territoriesToAttack.end() == std::find(territoriesToAttack.begin(),
                                                 territoriesToAttack.end(),
                                                 n.get()))
      {
        territoriesToAttack.push_back(n.get());
        break;
      }
    }
  }
  return territoriesToAttack;
}

std::vector<Territory *> HumanPlayer::to_defend(Player *player) const noexcept
{
  return player->getTerritories();
}

HumanPlayer *HumanPlayer::clone() const noexcept
{
  return new HumanPlayer(*this);
}

const StratType AggressivePlayer::type() const noexcept
{
  return StratType::Aggressive;
}

void AggressivePlayer::issue_order(
    const Map &gameMap, Player *player, std::vector<Player *> players,
    std::vector<Territory *> territoriesToDefend,
    std::vector<Territory *> territoriesToAttack) const noexcept
{

  int deployed = 0;

  // All troops deployed on the strongest friendly territory.
  Territory *strongestTerritory = ps::strong_deployment(gameMap, player, territoriesToDefend, &deployed);

  // attacks all adjacent territories of the strongest territory.
  std::vector<Territory *> territoriesToAttackFromStrongest = ps::enemy_adjacent_territories_from_territory(gameMap, player, strongestTerritory);
  for (auto *t : territoriesToAttackFromStrongest)
  {
    Advance *order = new Advance(player, &gameMap, strongestTerritory, t, (player->getTerritoryUnits(strongestTerritory) + deployed) / territoriesToAttackFromStrongest.size());
    player->getPlayerOrderList()->add(order);
  }

  // Randomly calls aggressive orders such as Airlift, Blockade, Bomb.
  ps::random_order(gameMap, player, players, territoriesToAttack, territoriesToDefend, true);
}

std::vector<Territory *>
AggressivePlayer::to_attack(const Map &gameMap, Player *player) const noexcept
{
  auto *strongestTerritory = ps::find_strongest_territory_from_territories(gameMap, player, player->getTerritories());

  return ps::enemy_adjacent_territories_from_territory(gameMap, player, strongestTerritory);
}

std::vector<Territory *>
AggressivePlayer::to_defend(Player *player) const noexcept
{
  return player->getTerritories();
}

AggressivePlayer *AggressivePlayer::clone() const noexcept
{
  return new AggressivePlayer(*this);
}

const StratType BenevolentPlayer::type() const noexcept
{
  return StratType::Benevolent;
}

void BenevolentPlayer::issue_order(
    const Map &gameMap, Player *player, std::vector<Player *> players,
    std::vector<Territory *> territoriesToDefend,
    std::vector<Territory *> territoriesToAttack) const noexcept
{

  // Does not attack.
  ps::weak_deployment(gameMap, player, territoriesToDefend);
  ps::random_order(gameMap, player, players, territoriesToAttack, territoriesToDefend, false);
}

std::vector<Territory *>
BenevolentPlayer::to_attack(const Map &gameMap, Player *player) const noexcept
{
  // No territories to attack.
  return std::vector<Territory *>(0);
}

std::vector<Territory *>
BenevolentPlayer::to_defend(Player *player) const noexcept
{
  return player->getTerritories();
}

BenevolentPlayer *BenevolentPlayer::clone() const noexcept
{
  return new BenevolentPlayer(*this);
}

const StratType NeutralPlayer::type() const noexcept
{
  return StratType::Neutral;
}

void NeutralPlayer::issue_order(
    const Map &gameMap, Player *player, std::vector<Player *> players,
    std::vector<Territory *> territoriesToDefend,
    std::vector<Territory *> territoriesToAttack) const noexcept
{
  // If attacked, becomes an Aggressive Player.
}

std::vector<Territory *>
NeutralPlayer::to_attack(const Map &gameMap, Player *player) const noexcept
{
  // No territory to attack.
  return std::vector<Territory *>(0);
}

std::vector<Territory *>
NeutralPlayer::to_defend(Player *player) const noexcept
{
  return player->getTerritories();
}

NeutralPlayer *NeutralPlayer::clone() const noexcept
{
  return new NeutralPlayer(*this);
}

const StratType CheaterPlayer::type() const noexcept
{
  return StratType::Cheater;
}

void CheaterPlayer::issue_order(
    const Map &gameMap, Player *player, std::vector<Player *> players,
    std::vector<Territory *> territoriesToDefend,
    std::vector<Territory *> territoriesToAttack) const noexcept
{

  //  ps::random_deployment(gameMap, player, territoriesToDefend);

  for (auto *t : territoriesToAttack)
  {
    // get any owned adjacent territory and set it as source territory
    Territory *source = nullptr;
    for (std::shared_ptr<Territory> n : Map::getAdjacentTerritories(gameMap, t->getName()))
    {
      if (player->owns(n.get()))
      {
        source = n.get();
        break;
      }
    }

    Advance *order = new Advance(player, &gameMap, source, t, 99);
    player->getPlayerOrderList()->add(order);
  }

  //  ps::random_order(gameMap, player, true);
}

std::vector<Territory *>
CheaterPlayer::to_attack(const Map &gameMap, Player *player) const noexcept
{
  return ps::enemy_adjacent_territories(gameMap, player);
}

std::vector<Territory *>
CheaterPlayer::to_defend(Player *player) const noexcept
{
  return player->getTerritories();
}

CheaterPlayer *CheaterPlayer::clone() const noexcept
{
  return new CheaterPlayer(*this);
}