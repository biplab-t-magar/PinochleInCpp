#pragma once
#ifndef MELD_STORAGE_H
#define MELD_STORAGE_H

//Stores:    Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
#include "MeldInstance.h"

class MeldsStorage {
public:
   MeldsStorage();

   int getNumOfMeldsByType(Meld meldType);
   int getNumOfMelds();
   std::vector<MeldInstance> getAllMeldsByType(Meld meldType) const;
   std::vector<MeldInstance> getAllMeldsUsingCard(Card card);
   std::vector<std::vector<MeldInstance>> getAllMelds() const;

   bool addMeld(MeldInstance meldInstance);
   bool addMelds(std::vector<MeldInstance> meldInstances);
   bool removeMeld( MeldInstance meldInstance);
   
   bool cardsUsedForSameMeld(std::vector<Card> cards);
   bool cardsUsedForSameMeld(MeldInstance cards);
   bool isCardUsedByAnyMeld(Card card);
   bool isCardUsedByMeld(Card card, Meld meldType);
   
private:
   std::vector<std::vector<MeldInstance>> storage;
};

#endif