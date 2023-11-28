#include "Orders.h"
#include "Player.h"
#include "Map.h"

#include "PlayerStrategies.h"

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

void HumanPlayer::issue_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> territoriesToDefend, std::vector<Territory *> territoriesToAttack) const noexcept
{
  // print all territories to defend
  cout << player->getName() << "'s territories to defend:" << endl;
  for (Territory *t : territoriesToDefend)
  {
    cout << *t << endl;
  }

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
      if (!std::all_of(str_reinforcements.begin(), str_reinforcements.end(),
                       ::isdigit))
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
          // TODO: add the deploy order to the order list
          player->getPlayerOrderList()->add(Deploy(player, &gameMap, t, 1));
          orderAdded = true;
          break;
        }
      }

      int num_reinforcements = std::stoi(str_reinforcements);
      if (orderAdded && num_reinforcements <= nb_reinforcement_cards &&
          num_reinforcements > 0)
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
        // print all territories to attack (for visibility)
        cout << player->getName() << "'s territories to attack:" << endl;
        for (Territory *t : territoriesToAttack)
        {
          cout << *t << endl;
        }

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
        Territory *source = nullptr;
        Territory *dest = nullptr;

        for (Territory *t : territoriesToAttack)
        {
          if (t->getName() == terr_1)
          {
            source = t;
          }
          else if (t->getName() == terr_2)
          {
            dest = t;
          }
        }

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

        player->getPlayerOrderList()->add(Advance(player, &gameMap, target_player, source,
                                                  dest, std::stoi(str_num_armies)));
      }
      else if (input == "bomb" && cards_count[CardType::bomb] > 0)
      {
        player->getPlayerOrderList()->add(Bomb(player, &gameMap, nullptr, nullptr, nullptr));
        cards_count[CardType::bomb]--;
      }
      else if (input == "blockade" && cards_count[CardType::blockade] > 0)
      {
        player->getPlayerOrderList()->add(
            Blockade(player, &gameMap, nullptr, nullptr, nullptr));
        cards_count[CardType::blockade]--;
      }
      else if (input == "airlift" && cards_count[CardType::airlift] > 0)
      {
        player->getPlayerOrderList()->add(
            Airlift(player, &gameMap, nullptr, nullptr, nullptr, 0));
        cards_count[CardType::airlift]--;
      }
      else if (input == "negotiate" && cards_count[CardType::diplomacy] > 0)
      {
        player->getPlayerOrderList()->add(Negotiate(player, &gameMap, nullptr, nullptr));
        cards_count[CardType::diplomacy]--;
      }
      else
      {
        cout << "Invalid order." << endl;
      }
    }
  }
}

std::vector<Territory *> HumanPlayer::to_attack(const Map &gameMap, Player *player) const noexcept
{
  std::vector<Territory *> territoriesToAttack;

  for (Territory *t : player->getTerritories())
  {
    for (std::shared_ptr<Territory> n : Map::getAdjacentTerritories(gameMap, t->getName()))
    {
      if (!player->owns(n.get()) && territoriesToAttack.end() == std::find(territoriesToAttack.begin(), territoriesToAttack.end(), n.get()))
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

void AggressivePlayer::issue_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> territoriesToDefend, std::vector<Territory *> territoriesToAttack) const noexcept {}

std::vector<Territory *> AggressivePlayer::to_attack(const Map &gameMap, Player *player) const noexcept {}

std::vector<Territory *> AggressivePlayer::to_defend(Player *player) const noexcept {}

AggressivePlayer *AggressivePlayer::clone() const noexcept
{
  return new AggressivePlayer(*this);
}

const StratType BenevolentPlayer::type() const noexcept
{
  return StratType::Benevolent;
}

void BenevolentPlayer::issue_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> territoriesToDefend, std::vector<Territory *> territoriesToAttack) const noexcept {}

std::vector<Territory *> BenevolentPlayer::to_attack(const Map &gameMap, Player *player) const noexcept {}

std::vector<Territory *> BenevolentPlayer::to_defend(Player *player) const noexcept {}

BenevolentPlayer *BenevolentPlayer::clone() const noexcept
{
  return new BenevolentPlayer(*this);
}

const StratType NeutralPlayer::type() const noexcept
{
  return StratType::Neutral;
}

void NeutralPlayer::issue_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> territoriesToDefend, std::vector<Territory *> territoriesToAttack) const noexcept {}

std::vector<Territory *> NeutralPlayer::to_attack(const Map &gameMap, Player *player) const noexcept {}

std::vector<Territory *> NeutralPlayer::to_defend(Player *player) const noexcept {}

NeutralPlayer *NeutralPlayer::clone() const noexcept
{
  return new NeutralPlayer(*this);
}

const StratType CheaterPlayer::type() const noexcept
{
  return StratType::Cheater;
}

void CheaterPlayer::issue_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> territoriesToDefend, std::vector<Territory *> territoriesToAttack) const noexcept {}

std::vector<Territory *> CheaterPlayer::to_attack(const Map &gameMap, Player *player) const noexcept {}

std::vector<Territory *> CheaterPlayer::to_defend(Player *player) const noexcept {}

CheaterPlayer *CheaterPlayer::clone() const noexcept
{
  return new CheaterPlayer(*this);
}