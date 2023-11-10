#include "./Cards.h"
#include "../Orders/Orders.h"

// Created by Maxime Landry (maxime334) on 23-10-14

namespace cd {
const std::string map(const Type type) {
  /*
    Using switch case, maps the enum class to a string value for
    better usage.
  */
  switch (type) {
  case Type::airlift:
    return "airlift";
  case Type::blockade:
    return "blockade";
  case Type::bomb:
    return "bomb";
  case Type::diplomacy:
    return "diplomacy";
  default:
    return "reinforcement";
  }
}
} // namespace cd

std::ostream &operator<<(std::ostream &os, const Type &type) {
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

Card::Card(Type &&type) : m_type{type} {}
Card::Card(const Type &type) : m_type{Type(type)} {
  // By ref but copy is made.
}
Card::Card(Card &&card) : m_type{card.m_type} {}
Card::Card(const Card &card) : m_type{Type(card.m_type)} {}

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

  return card;
}

void Buffer::insert(const Card card) noexcept {
  // Copy constructor of card.
  m_buffer.push_back(std::make_unique<Card>(card));
}

void Buffer::insert(const Type type) noexcept {
  m_buffer.push_back(std::make_unique<Card>(type));
}

std::vector<Type> Buffer::show_cards() const noexcept {
  // Creates array of same size as m_buffer.
  auto arr = std::vector<Type>(m_buffer.size());
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

// --Deck.

Deck::Deck() : Buffer() {}
Deck::Deck(const Deck &d) : Buffer(d) {}
// Constructor overloading.

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

bool Hand::play(const Type type, Deck &deck) {
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
// Very similar to Buffer's.