#include "Cards.hpp"
#include <iostream>

/*
  Test the cards methods.
  Creates a deck of cards, then creates a hand by drawing repeatedly from the
  deck.
*/
void test_cards(int no_cards = 15) {
  Deck deck;
  for (int i = 0; i < no_cards; i++) {
    deck.insert(card_type(i % 5)); // Kind of random insertion of cards.
  }

  Hand hand;
  for (int i = 0; i < no_cards; i++) {
    deck.draw(hand);
  }

  for (int i = 0; i < no_cards; i++) {
    hand.play(card_type(i % 5), deck);
  }

  std::cout << deck.size();
}

// Testing purposes.
int main() {
  Card c(bomb);
  Card c1(airlift);

  Hand h;
  Deck d;

  std::cout << h.play(bomb, d) << std::endl;

  return 0;
}