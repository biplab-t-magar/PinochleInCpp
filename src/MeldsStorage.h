#pragma once
#ifndef MELD_STORAGE_H
#define MELD_STORAGE_H

//Stores:    Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
#include "MeldInstance.h"

class MeldsStorage {
public:
   MeldsStorage();
   bool addMeld(MeldInstance meldInstance);
   bool addMelds(std::vector<MeldInstance> meldInstances);
   bool removeMeld( MeldInstance meldInstance);
   std::vector<std::vector<MeldInstance>> getAllMelds() const;
   std::vector<MeldInstance> getAllMeldsByType(Meld meldType) const;
   
   std::vector<MeldInstance> getAllMeldsUsingCard(Card card);
   bool cardsUsedForSameMeld(std::vector<Card> cards);
   bool cardsUsedForSameMeld(MeldInstance cards);
   bool isCardUsedByAnyMeld(Card card);
   // bool isCardTypeUsedByAnyMeld(Rank rank, Suit suit);
   bool isCardUsedByMeld(Card card, Meld meldType);
   // bool isCardTypeUsedByMeld(Rank rank, Suit suit, Meld meldType);
   int getNumOfMeldsByType(Meld meldType);
   int getNumOfMelds();
private:
   std::vector<std::vector<MeldInstance>> storage;
};

#endif