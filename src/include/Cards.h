#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <vector>
// Created by Maxime Landry (maxime334) on 23-10-14

/*
  Five possible type of cards:
  bomb,
  reinforcement,
  blockade,
  airlift,
  diplomacy.
*/
enum class CardType : char {
  bomb,
  reinforcement,
  blockade,
  airlift,
  diplomacy
};

namespace cd {
/*
Maps class enum value to a string value.
Used internally inside the stream insertion operator overload.
*/
const std::string map(const CardType);
} // namespace cd

void test_cards(int = 15);

class Card;
class Buffer;
class Deck;
class Hand;
// Forward-Declaration.`

// Stream insertion overload.
std::ostream &operator<<(std::ostream &, const CardType &);

std::ostream &operator<<(std::ostream &, const Card &);

std::ostream &operator<<(std::ostream &, const Buffer &);

std::ostream &operator<<(std::ostream &, const Hand &);

std::ostream &operator<<(std::ostream &, const Deck &);

bool operator==(const Card &, const Card &) noexcept;

// Unique_ptr to card.
using card_ptr = std::unique_ptr<Card>;
// Vector holding unique_ptr to Cards.
using card_vector = std::vector<card_ptr>;

/*
    Warzone card.
    To declare a type for the card, use Type::type where type is to be
   chosen.
*/
class Card {
public:
  // Type of the card. 5 types available.
  // Type::type to be used. Check enum class for further details.
  const CardType m_type;

  Card() = delete; // Assures class is called w/ initializer.
  // Takes rvalue as argument. Better optimization.
  Card(CardType &&);
  Card(const CardType &);
  // Move constructor.
  Card(Card &&);
  Card(const Card &);
  ~Card() = default;

  /*
    Creates an order an adds it to the player's list of orders
    and then returns the card to the deck.
  */
  void play(Deck &) const noexcept;

  /*
    Returns string type of the card.
  */
  const std::string type() const noexcept;

  /*
    Card class should not be assignable, and a new Card should just be created.
    Compile-time error will occur, so nothing happens in the dark.
  */
  Card &operator=(const Card &) = delete;
};

/*
  Base class when holding cards is necessary.
  Container of cards.
*/
class Buffer {
protected:
  card_vector m_buffer;
  // Keeps count of number of each card type inside the Buffer.
  std::map<CardType, int> m_card_count;

  /*
    Manages the removal of the card from the m_buffer.
    Returns a unique_ptr to the card.
  */
  card_ptr remove(const int index);

public:
  Buffer();
  Buffer(const Buffer &);
  ~Buffer() = default;

  /*
    Insert a card inside the m_buffer.
    Passed by value.
  */
  void insert(const Card) noexcept;

  /*
    Inserts a card of the proper type inside the player's Hand.
    Creates a new card.
  */
  void insert(const CardType) noexcept;

  /*
    Inserts a specified number of random cards inside the card's container.
  */
  void random_insert(const int &) noexcept;

  /*
    Returns a vector of all the cards, as types, contained inside the hand.
    The vector can be implicitely converted to a vector of cards inside a
    for-each loop.
  */
  std::vector<CardType> show_cards() const noexcept;

  /*
    Returns number of cards left inside the player's hand.
  */
  int size() const noexcept;

  /*
    Removes every card inside the m_buffer. Clears it.
  */
  void clear() noexcept;

  /*
    Deep copy of the m_buffer is made.
  */
  Buffer &operator=(const Buffer &) noexcept;

  const std::map<CardType, int> &card_count() const noexcept;
};

/*
  Finite collection of Warzone cards.
*/
class Hand : public Buffer {
public:
  Hand();
  Hand(const Hand &);
  ~Hand() = default;
  /*
    The card with desired type will be played, and removed from the player's
    hands. The card is then placed back in the deck.
    Returns false if the card is not in the player's hand.
  */
  bool play(const CardType, Deck &);
  bool play(CardType);

  /*
    Deep copy of the Hand is made.
  */
  Hand &operator=(const Hand &) noexcept;
};

/*
  Contains finite collection of Warzone cards.
*/
class Deck : public Buffer {
public:
  Deck();
  Deck(const Deck &);
  ~Deck() = default;
  /*
    Draw a card at random from the cards remaining in the deck.
    Place it in the their hand of cards.
   */
  void draw(Hand &);

  /*
    Deep copy of the Deck is made.
  */
  Deck &operator=(const Deck &) noexcept;
};