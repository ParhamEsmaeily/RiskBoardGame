#include <iostream>

enum class StratType : char { Human, Aggressive, Benevolent, Neutral, Cheater };

class PlayerStrategy;

namespace ps {
/*
    Maps a StratType to a std::string value.
*/
const std::string map(const StratType &);
/*
    Returns a PlayerStrategy pointer, by polymorphism, w/ only a StratType as
   argument. Pointer is the responsability of the user.
*/
PlayerStrategy *make_player_strat(const StratType &);
} // namespace ps

std::ostream &operator<<(std::ostream &os, const StratType &type);

/*
    Interface.
*/
class PlayerStrategy {
public:
  virtual void issue_order() const noexcept = 0;
  virtual void to_defend() const noexcept = 0;
  virtual void to_attack() const noexcept = 0;
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

class HumanPlayer : public PlayerStrategy {
public:
  HumanPlayer() = default;
  ~HumanPlayer() override = default;

  void issue_order() const noexcept override;
  void to_defend() const noexcept override;
  void to_attack() const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  HumanPlayer *clone() const noexcept override;
};

/*
    Computer player that focuses on attack (deploys or advances armies on its
    strongest country, then always advances to enemy territories until it cannot
   do so anymore; will use any card with an aggressive purpose.
*/
class AggressivePlayer : public PlayerStrategy {
public:
  AggressivePlayer() = default;
  ~AggressivePlayer() override = default;

  void issue_order() const noexcept override;
  void to_defend() const noexcept override;
  void to_attack() const noexcept override;

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
class BenevolentPlayer : public PlayerStrategy {
public:
  BenevolentPlayer() = default;
  ~BenevolentPlayer() override = default;

  void issue_order() const noexcept override;
  void to_defend() const noexcept override;
  void to_attack() const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  BenevolentPlayer *clone() const noexcept override;
};

class NeutralPlayer : public PlayerStrategy {
public:
  NeutralPlayer() = default;
  ~NeutralPlayer() override = default;

  void issue_order() const noexcept override;
  void to_defend() const noexcept override;
  void to_attack() const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  NeutralPlayer *clone() const noexcept override;
};

class CheaterPlayer : public PlayerStrategy {
public:
  CheaterPlayer() = default;
  ~CheaterPlayer() override = default;

  void issue_order() const noexcept override;
  void to_defend() const noexcept override;
  void to_attack() const noexcept override;

  inline const StratType type() const noexcept override;
  // Acts as a virtual copy constructor.
  CheaterPlayer *clone() const noexcept override;
};