#pragma once
#ifndef MELD_SERVICES_H
#define MELD_SERVICES_H

#include "Card.h"
#include <vector>

enum class Meld {
    Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
};

class MeldServices {
public:
   MeldServices();
   std::vector<std::vector<Card>> findPotentialMeldsFromHand();
   size_t getMeldPoints(Meld meld);
   bool isValidMeld(std::vector<Card> cards, Meld *whatMeld);
private:
   std::vector<Meld> meldsUsed;
   std::vector<std::vector<Card>> cardsUsedForCreatingMelds;
   Suit trumpSuit; 
   bool hasMeldBeenUsed();
   bool isMeldValid();
   bool isDix(std::vector<Card> cards);
   bool isMarriage(std::vector<Card> cards);
   bool isRoyalMarriage(std::vector<Card> cards);
   bool isPinochle(std::vector<Card> cards);
   bool isFours(std::vector<Card> cards);
   bool isFlush(std::vector<Card> cards);

};

#endif