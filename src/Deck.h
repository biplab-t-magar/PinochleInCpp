#pragma once
#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>

class Deck {
public:
   Deck(int numOfEachCard  = 2);
   int getNumRemaining() const;
   std::vector<Card> getAllRemainingCards() const;

   void shuffle();
   Card takeOneFromTop();
   void putCardAtTop(Card card);
   void removeCards(std::vector<Card> cards);
   void clear();
private:
   std::vector<Card> cards;
   const int numOfEachCard;
   void populate();
};

#endif