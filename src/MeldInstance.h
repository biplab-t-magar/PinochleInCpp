#pragma once
#ifndef MELD_INSTANCE_H
#define MELD_INSTANCE_H

#include "GroupOfCards.h"

enum class Meld {
   Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
};

class MeldInstance : public GroupOfCards {
public:
   MeldInstance(std::vector<Card> cards, Suit trumpSuit);
   Meld getMeldType() const;
   bool isValidMeld() const;
private:
   Meld meldType;
   bool meldIsValid;

   bool checkMeldValidity(Suit trumpSuit);
   bool isDix(Suit trumpSuit);
   bool isAnyMarriage();
   Meld typeOfMarriage(Suit trumpSuit);
   bool isPinochle(Suit trumpSuit);
   bool isFours();
   bool isFlush(Suit trumpSuit);
};

#endif