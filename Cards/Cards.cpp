#include "Cards.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>

namespace cd {
std::string type_map(const int index) {
  std::string types[5] = {"bomb", "reinforcement", "blockade", "airlift",
                          "diplomacy"};
  return types[index];
}
} // namespace cd

std::ostream &operator<<(std::ostream &os, const Card &c) {
  // Mapping is made from enum index to the proper string type.
  os << "Card is of type: " << cd::type_map(c.type);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Buffer &b) {
  os << "The buffer contains " << b.size() << " cards.";
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

// --Card.

Card::Card(const card_type type) : type{card_type(type)} {}
Card::Card(const Card &card) : type{card_type(card.type)} {}

void Card::play(Deck &deck) const noexcept {
  // Play the card. To be implemented.

  // The card placed back inside the deck.
  deck.insert(*this);
}

const std::string Card::value() const noexcept { return cd::type_map(type); }

// void Card::operator=(const Card &card) noexcept {
// }

// --Buffer

Buffer::Buffer() : buffer{card_vector()} {}
Buffer::Buffer(const Buffer &buf) {
  // Buffer of the same size.
  buffer = card_vector(buf.size());

  // Copy the buffer by creating new cards.
  // Have two different decks.
  for (int i = 0; i < buffer.size(); i++) {
    // Deep Copy of the buffer made.
    buffer[i] = std::make_unique<Card>(*buf.buffer[i]);
  }
}

card_ptr Buffer::remove(const int index) {
  if (index > buffer.size() - 1 || index < 0) {
    std::cerr << "Index out of range inside Card buffer/container. Please "
                 "verify the Cards.cpp."
              << std::endl;
    throw std::runtime_error("Index out of range");
  } // Index out of range.

  int end_index = buffer.size() - 1;
  // Removes the card from the buffer.
  auto card = std::move(buffer[index]);
  // Swap last element w/ buffer[i]
  buffer[index] = std::move(buffer[end_index]);
  buffer.pop_back(); // Reduce size.

  return card;
}

void Buffer::insert(const Card &card) noexcept {
  buffer.push_back(std::make_unique<Card>(card));
}

void Buffer::insert(const card_type type) noexcept {
  buffer.push_back(std::make_unique<Card>(type));
}

std::vector<card_type> Buffer::show_cards() const noexcept {
  // Creates array of same size as buffer.
  auto arr = std::vector<card_type>(buffer.size());
  // Each card of the buffer taken and converted to a card_type.
  for (int i = 0; i < buffer.size(); i++) {
    arr[i] = buffer[i]->type;
  }
  return arr;
}

int Buffer::size() const noexcept { return buffer.size(); }

void Buffer::clear() noexcept { buffer = card_vector(); }

void Buffer::operator=(const Buffer &buf) noexcept {
  // Copies every element of the buffer to the new buffer.
  for (int i = 0; i < buffer.size(); i++) {
    buffer[i] = std::make_unique<Card>(*buf.buffer[i]);
  }
}

// --Deck.

void Deck::draw(Hand &hand) {
  const int end_index = buffer.size() - 1;

  // Random index generated.
  std::random_device rd;  // Generates a seed.
  std::mt19937 rng(rd()); // Random seed
  std::uniform_int_distribution<> distrib(0, end_index);

  // Index of card to be drawn.
  const int index = distrib(rng);

  // Removes the card from the buffer.
  auto sampled_card = this->remove(index);

  hand.insert(*sampled_card);
}

// --Hand.

bool Hand::play(const card_type type, Deck &deck) {
  // Searching through the whole hand to search for a card of the proper
  for (int i = 0; i < buffer.size(); i++) {
    if (buffer[i]->type == type) {
      // Removes card and returns it.
      auto ptr = this->remove(i);

      deck.insert(*ptr); // Inserts the card inside the buffer.
      return true;
    }
  }

  // If card has not been found.
  return false;
}
