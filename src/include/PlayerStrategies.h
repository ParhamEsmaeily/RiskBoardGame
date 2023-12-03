#pragma once

#include <iostream>
#include <memory>

#include "Map.h"

void testPlayerStrategies();

enum class StratType : char
{
  Human,
  Aggressive,
  Benevolent,
  Neutral,
  Cheater
};

class OrdersList;
class Player;
class PlayerStrategy;

namespace ps
{
  /*
      Maps a StratType to a std::string value.
  */
  const std::string map(const StratType &);
  /*
      Returns a PlayerStrategy pointer, by polymorphism, w/ only a StratType as
     argument. Pointer is the responsability of the user.
  */
  PlayerStrategy *make_player_strat(const StratType &);

  /* Parses the territory name to the object within the passed vector. */
  Territory *parse_territory_name(std::vector<Territory *> territories, std::string terr_name);
  /* Randomly deploys soldiers to territories.*/
  void random_deployment(const Map &, Player *, std::vector<Territory *>);
  /* Returns the strongest territory of a certain player. If all are equal, returns the first territory. */
  Territory *find_strongest_territory(const Map &, Player *);
  /* Returns the strongest territory from a vector of territories. If all are equal, returns the first territory. */
  Territory *find_strongest_territory_from_territories(const Map &, Player *, std::vector<Territory *>);
  /* Deploys all troops on the strongest territory. Returns strongest territory. */
  Territory *strong_deployment(const Map &, Player *, std::vector<Territory *>);
  /* Deploys troops to the weakest territories. */
  void weak_deployment(const Map &, Player *, std::vector<Territory *>);
  /* Randomly calls order other than Deploy and Advance. As such, only calls
  Airlift, Blockade, Diplomacy, Bomb.*/
  void random_order(const Map &gameMap, Player *player, std::vector<Player *> players, std::vector<Territory *> toAttack, std::vector<Territory *> toDefend, const bool &make_harm);
  /* Returns all adjacent and non-owned territories. */
  const std::vector<Territory *> enemy_adjacent_territories(const Map &, Player *);
  /* Returns all adjacent and non-owned territories from a territory. */
  const std::vector<Territory *> enemy_adjacent_territories_from_territory(const Map &gameMap, Player *player, Territory *t);

} // namespace ps

std::ostream &operator<<(std::ostream &os, const StratType &type);

std::ostream &operator<<(std::ostream &os, const PlayerStrategy &strategy);

/*
    Interface.
*/
class PlayerStrategy
{
public:
  virtual void
  issue_order(const Map &gameMap, Player *player, std::vector<Player *> players,
              std::vector<Territory *> territoriesToDefend,
              std::vector<Territory *> territoriesToAttack) const noexcept = 0;
  virtual std::vector<Territory *> to_defend(Player *player) const noexcept = 0;
  virtual std::vector<Territory *> to_attack(const Map &gameMap,
                                             Player *player) const noexcept = 0;
  virtual inline const StratType type() const noexcept = 0;
  /*
    Acts as a virtual copy constructors. Uses covariant return types. As such,
    a Base class holding a derived can be copied such that a new Base class will
    still hold a derived class.
  */
  virtual PlayerStrategy *clone() const noexcept = 0;

  PlayerStrategy() = default;
  // Use the clone() for the copy constructor.
  virtual ~PlayerStrategy() = default;

protected:
  // Use the clone() for the copy constructor.
  PlayerStrategy(const PlayerStrategy &) = default;
};

class HumanPlayer : public PlayerStrategy
{
public:
  HumanPlayer() = default;
  ~HumanPlayer() override = default;

  void issue_order(
      const Map &gameMap, Player *player, std::vector<Player *> players,
      std::vector<Territory *> territoriesToDefend,
      std::vector<Territory *> territoriesToAttack) const noexcept override;
  std::vector<Territory *> to_defend(Player *player) const noexcept override;
  std::vector<Territory *> to_attack(const Map &gameMap,
                                     Player *player) const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  HumanPlayer *clone() const noexcept override;
};

/*
    Computer player that focuses on attack (deploys or advances armies on its
    strongest country, then always advances to enemy territories until it cannot
   do so anymore; will use any card with an aggressive purpose.
*/
class AggressivePlayer : public PlayerStrategy
{
public:
  AggressivePlayer() = default;
  ~AggressivePlayer() override = default;

  void issue_order(
      const Map &gameMap, Player *player, std::vector<Player *> players,
      std::vector<Territory *> territoriesToDefend,
      std::vector<Territory *> territoriesToAttack) const noexcept override;
  std::vector<Territory *> to_defend(Player *player) const noexcept override;
  std::vector<Territory *> to_attack(const Map &gameMap,
                                     Player *player) const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  AggressivePlayer *clone() const noexcept override;
};

/*
    Computer player that focuses on protecting its weak countries (deploys or
   advances armies on its weakest countries, never advances to enemy
   territories; may use cards but will never use a card in a way that
   purposefully will harm anyone)
*/
class BenevolentPlayer : public PlayerStrategy
{
public:
  BenevolentPlayer() = default;
  ~BenevolentPlayer() override = default;

  void issue_order(
      const Map &gameMap, Player *player, std::vector<Player *> players,
      std::vector<Territory *> territoriesToDefend,
      std::vector<Territory *> territoriesToAttack) const noexcept override;
  std::vector<Territory *> to_defend(Player *player) const noexcept override;
  std::vector<Territory *> to_attack(const Map &gameMap,
                                     Player *player) const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  BenevolentPlayer *clone() const noexcept override;
};

/*
Computer player that never issues any order, nor uses any cards, though it may
have or receive cards. If a Neutral player is attacked, it becomes an Aggressive
player.
*/
class NeutralPlayer : public PlayerStrategy
{
public:
  NeutralPlayer() = default;
  ~NeutralPlayer() override = default;

  void issue_order(
      const Map &gameMap, Player *player, std::vector<Player *> players,
      std::vector<Territory *> territoriesToDefend,
      std::vector<Territory *> territoriesToAttack) const noexcept override;
  std::vector<Territory *> to_defend(Player *player) const noexcept override;
  std::vector<Territory *> to_attack(const Map &gameMap,
                                     Player *player) const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  NeutralPlayer *clone() const noexcept override;
};

/*
  Computer player that automatically conquers all territories that are adjacent
  to its own territories (only once per turn). Does not use cards, though it may
  have or receive cards.
*/
class CheaterPlayer : public PlayerStrategy
{
public:
  CheaterPlayer() = default;
  ~CheaterPlayer() override = default;

  void issue_order(
      const Map &gameMap, Player *player, std::vector<Player *> players,
      std::vector<Territory *> territoriesToDefend,
      std::vector<Territory *> territoriesToAttack) const noexcept override;
  std::vector<Territory *> to_defend(Player *player) const noexcept override;
  std::vector<Territory *> to_attack(const Map &gameMap,
                                     Player *player) const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  CheaterPlayer *clone() const noexcept override;
};