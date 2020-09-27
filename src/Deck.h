#pragma once
#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>

class Deck {
public:
   Deck(int numOfEachCard  = 2);
   void shuffle();
   Card takeOneFromTop();
   void putCardAtTop(Card card);
   void removeCards(std::vector<Card> cards);

   int getNumRemaining() const;
   std::vector<Card> getAllRemainingCards() const;
   
   void clear();
private:
   std::vector<Card> cards;
   const int numOfEachCard;
   void populate();
};

#endif