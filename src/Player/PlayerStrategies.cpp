#include "PlayerStrategies.h"

namespace ps {
const std::string map(const StratType &type) {
  switch (type) {
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
  }
}

PlayerStrategy *make_player_strat(const StratType &type) {
  switch (type) {
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

std::ostream &operator<<(std::ostream &os, const StratType &strat) {
  os << ps::map(strat);
  return os;
}

const StratType HumanPlayer::type() const noexcept { return StratType::Human; }

void HumanPlayer::issue_order() const noexcept {}

void HumanPlayer::to_attack() const noexcept {}

void HumanPlayer::to_defend() const noexcept {}

HumanPlayer *HumanPlayer::clone() const noexcept {
  return new HumanPlayer(*this);
}

const StratType AggressivePlayer::type() const noexcept {
  return StratType::Aggressive;
}

void AggressivePlayer::issue_order() const noexcept {}

void AggressivePlayer::to_attack() const noexcept {}

void AggressivePlayer::to_defend() const noexcept {}

AggressivePlayer *AggressivePlayer::clone() const noexcept {
  return new AggressivePlayer(*this);
}

const StratType BenevolentPlayer::type() const noexcept {
  return StratType::Benevolent;
}

void BenevolentPlayer::issue_order() const noexcept {}

void BenevolentPlayer::to_attack() const noexcept {}

void BenevolentPlayer::to_defend() const noexcept {}

BenevolentPlayer *BenevolentPlayer::clone() const noexcept {
  return new BenevolentPlayer(*this);
}

const StratType NeutralPlayer::type() const noexcept {
  return StratType::Neutral;
}

void NeutralPlayer::issue_order() const noexcept {}

void NeutralPlayer::to_attack() const noexcept {}

void NeutralPlayer::to_defend() const noexcept {}

NeutralPlayer *NeutralPlayer::clone() const noexcept {
  return new NeutralPlayer(*this);
}

const StratType CheaterPlayer::type() const noexcept {
  return StratType::Cheater;
}

void CheaterPlayer::issue_order() const noexcept {}

void CheaterPlayer::to_attack() const noexcept {}

void CheaterPlayer::to_defend() const noexcept {}

CheaterPlayer *CheaterPlayer::clone() const noexcept {
  return new CheaterPlayer(*this);
}