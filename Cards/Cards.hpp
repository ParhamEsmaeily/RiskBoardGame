#pragma once
#include <algorithm>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

enum card_type { bomb, reinforcement, blockade, airlift, diplomacy };

namespace cd {
/*
Maps enum value to a string value. Enum value directly does not give a string
value.
*/
std::string type_map(const int);
} // namespace cd

class Card;
class Deck;
class Hand;
// Forward-Declaration.

using card_ptr = std::unique_ptr<Card>;
// Vector holding unique_ptr to Cards.
using card_vector = std::vector<card_ptr>;

/*
    Warzone card.
*/
class Card {
public:
  // Type of the card. 5 types available.
  const card_type type;

  Card(const card_type);
  Card(const Card &);
  // Destructor does not seem necessary for this class.

  /*
    Creates an order an adds it to the player's list of orders
    and then returns the card to the deck.
  */
  void play(Deck &) const noexcept;

  /*
    Returns string type of the card.
  */
  const std::string value() const noexcept;

  friend std::ostream &operator<<(std::ostream &os, const Card &c) {
    // Mapping is made from enum index to the proper string type.
    os << "Card is of type: " << cd::type_map(c.type);
    return os;
  }

  /*
    Deep copy of the Card is made.
  */
  void operator=(const Card &) noexcept;
};

/*
  Base class when holding cards is necessary.
  Container of cards.
*/
class Buffer {
protected:
  card_vector buffer;

  /*
    Manages the removal of the card from the buffer.
    Returns a unique_ptr to the card.
  */
  card_ptr remove(const int index);

public:
  Buffer();
  Buffer(const Buffer &);

  /*
    Insert a card by reference inside the deck.
  */
  void insert(const Card &) noexcept;

  /*
    Inserts a card of the proper type inside the player's Hand.
    Creates a new card.
  */
  void insert(const card_type) noexcept;

  /*
    Returns a vector of all the cards, as types, contained inside the hand.
    The vector can be implicitely converted to a vector of cards inside a
    foreach loop.
  */
  std::vector<card_type> show_cards() const noexcept;

  /*
    Returns number of cards left inside the player's hand.
  */
  int size() const noexcept;

  void clear() noexcept;

  /*
    Deep copy of the buffer is made.
  */
  void operator=(const Buffer &) noexcept;
};

/*
  Finite collection of Warzone cards.
*/
class Hand : public Buffer {
public:
  /*
    The card with desired type will be played, and removed from the player's
    hands.
  */
  void play(const card_type, Deck &);
};

/*
  Contains finite collection of Warzone cards.
*/
class Deck : public Buffer {
public:
  /*
    Draw a card at random from the cards remaining in the deck.
    Place it in the their hand of cards.
   */
  void draw(Hand &);

  void operator=(const Card &) const noexcept;
};
