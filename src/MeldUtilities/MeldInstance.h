#pragma once
#ifndef MELD_INSTANCE_H
#define MELD_INSTANCE_H

#include "GroupOfCards.h"

enum class Meld {
   Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
};

class MeldInstance : public GroupOfCards {
public:
   MeldInstance();
   MeldInstance(std::vector<Card> cards, Suit trumpSuit);
   bool MeldInstance::addCard(Card card, Suit trumpSuit);
   bool removeCardById(int id); 
   bool removeCardByPosition(int position);
   Meld getMeldType() const;
   std::string getMeldTypeString() const;
   bool isValidMeld() const;
private:
   Meld meldType;
   bool meldIsValid;
   Suit trumpSuit;

   bool checkMeldValidity();
   bool isDix();
   bool isAnyMarriage();
   Meld typeOfMarriage();
   bool isPinochle();
   bool isFours();
   bool isFlush();
};

#endif