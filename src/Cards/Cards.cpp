#include "Cards.h"

// Created by Maxime Landry (maxime334) on 23-10-14

namespace cd {
const std::string map(const CardType type) {
  /*
    Using switch case, maps the enum class to a string value for
    better usage.
  */
  switch (type) {
  case CardType::airlift:
    return "airlift";
  case CardType::blockade:
    return "blockade";
  case CardType::bomb:
    return "bomb";
  case CardType::diplomacy:
    return "diplomacy";
  default:
    return "reinforcement";
  }
}
} // namespace cd

std::ostream &operator<<(std::ostream &os, const CardType &type) {
  os << cd::map(type);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Card &c) {
  // Mapping is made from enum index to the proper string m_type.
  os << "Card is of type: " << c.m_type;
  return os;
}

std::ostream &operator<<(std::ostream &os, const Buffer &b) {
  os << "The m_buffer contains " << b.size() << " cards.";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Hand &h) {
  os << "The hand contains " << h.size() << " cards.";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Deck &d) {
  os << "The deck contains " << d.size() << " cards.";
  return os;
}

bool operator==(const Card &c1, const Card &c2) noexcept {
  return c1.m_type == c2.m_type;
}

// --Card.

Card::Card(CardType &&type) : m_type{type} {}
Card::Card(const CardType &type) : m_type{CardType(type)} {
  // By ref but copy is made.
}
Card::Card(Card &&card) : m_type{card.m_type} {}
Card::Card(const Card &card) : m_type{CardType(card.m_type)} {}

void Card::play(Deck &deck) const noexcept {
  // Play the card. To be implemented.

  // The card placed back inside the deck.
  deck.insert(*this);
}

const std::string Card::type() const noexcept { return cd::map(m_type); }

// --Buffer

Buffer::Buffer() : m_buffer{card_vector()} {}
Buffer::Buffer(const Buffer &buf) {
  // Buffer of the same size.
  m_buffer = card_vector(buf.size());

  // Copy the m_buffer by creating new cards.
  // Have two different decks.
  for (int i = 0; i < m_buffer.size(); i++) {
    // Deep Copy of the m_buffer made.
    m_buffer[i] = std::make_unique<Card>(*buf.m_buffer[i]);
  }
}

card_ptr Buffer::remove(const int index) {
  if (index > m_buffer.size() - 1 || index < 0) {
    std::cerr << "Index out of range inside Card m_buffer/container. Please "
                 "verify the Cards.cpp."
              << std::endl;
    throw std::runtime_error("Index out of range");
  } // Index out of range.

  int end_index = m_buffer.size() - 1;
  // Removes the card from the m_buffer.
  auto card = std::move(m_buffer[index]);
  // Swap last element w/ m_buffer[i]
  m_buffer[index] = std::move(m_buffer[end_index]);
  m_buffer.pop_back(); // Reduce size.

  // Decrease counter.
  m_card_count[card->m_type]--;

  return card;
}

void Buffer::insert(const Card card) noexcept {
  // Copy constructor of card.
  m_buffer.push_back(std::make_unique<Card>(card));
  // Increase counter.
  m_card_count[card.m_type]++;
}

void Buffer::insert(const CardType type) noexcept {
  m_buffer.push_back(std::make_unique<Card>(type));
  m_card_count[type]++;
}

void Buffer::random_insert(const int &number) noexcept {
  // Random index generated.
  std::random_device rd;  // Generates a seed.
  std::mt19937 rng(rd()); // Random seed
  // Type goes from 0 to 4 inclusive.
  std::uniform_int_distribution<> distrib(0, 4);

  // Random insertion.
  for (int i = 0; i < number; i++) {
    m_buffer.push_back(std::make_unique<Card>(CardType(distrib(rng))));
  }
}

std::vector<CardType> Buffer::show_cards() const noexcept {
  // Creates array of same size as m_buffer.
  auto arr = std::vector<CardType>(m_buffer.size());
  // Each card of the m_buffer taken and converted to a Type.
  for (int i = 0; i < m_buffer.size(); i++) {
    arr[i] = m_buffer[i]->m_type;
  }
  return arr;
}

int Buffer::size() const noexcept { return m_buffer.size(); }

void Buffer::clear() noexcept {
  m_buffer = card_vector(); // New empty vector is created.
}

Buffer &Buffer::operator=(const Buffer &buf) noexcept {
  // Copies every element of the m_buffer to the new m_buffer.
  for (const card_ptr &p : buf.m_buffer) {
    m_buffer.push_back(std::make_unique<Card>(*p));
  }
  return *this;
}

const std::map<CardType, int> &Buffer::card_count() const noexcept {
  return m_card_count;
}

// --Deck.

Deck::Deck() : Buffer() {}
Deck::Deck(const Deck &d) : Buffer(d) {}

void Deck::draw(Hand &hand) {
  const int end_index = m_buffer.size() - 1;

  // Random index generated.
  std::random_device rd;  // Generates a seed.
  std::mt19937 rng(rd()); // Random seed
  std::uniform_int_distribution<> distrib(0, end_index);

  // Index of card to be drawn.
  const int index = distrib(rng);

  // Removes the card from the m_buffer.
  auto sampled_card = this->remove(index);

  hand.insert(*sampled_card);
}

Deck &Deck::operator=(const Deck &d) noexcept {
  // Copies every element of the m_buffer to the new m_buffer.
  for (const card_ptr &p : d.m_buffer) {
    m_buffer.push_back(std::make_unique<Card>(*p));
  }
  return *this;
}
// Very similar to Buffer's.

// --Hand.
Hand::Hand() : Buffer() {}               // Constructor Overloading.
Hand::Hand(const Hand &h) : Buffer(h) {} // Constructor Overloading.

bool Hand::play(const CardType type, Deck &deck) {
  // Searching through the whole hand to search for a card of the proper
  for (int i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i]->m_type == type) {
      // Removes card and returns it.
      auto ptr = this->remove(i);

      deck.insert(*ptr); // Inserts the card inside the m_buffer.
      return true;
    }
  }

  // If card has not been found.
  return false;
}

Hand &Hand::operator=(const Hand &h) noexcept {
  for (const card_ptr &p : h.m_buffer) {
    m_buffer.push_back(std::make_unique<Card>(*p));
  }
  return *this;
}

// Same as above but without the deck.
bool Hand::play(CardType type) {
  // Searching through the whole hand to search for a card of the proper
  for (int i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i]->m_type == type) {
      // Removes card and returns it.
      auto ptr = this->remove(i);
      return true;
    }
  }

  // If card has not been found.
  return false;
}
// Very similar to Buffer's.