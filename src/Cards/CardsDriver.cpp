#include "Cards.h"

/*
  Test the cards methods.
  Creates a deck of cards, then creates a hand by drawing repeatedly from the
  deck.
*/
void test_cards(int no_cards) {
  Deck deck;
  for (int i = 0; i < no_cards; i++) {
    deck.insert(CardType(i % 5)); // Kind of random insertion of cards.
  }

  Hand hand;
  for (int i = 0; i < no_cards; i++) {
    deck.draw(hand);
  }

  for (int i = 0; i < no_cards; i++) {
    hand.play(CardType(i % 5), deck);
  }
}
