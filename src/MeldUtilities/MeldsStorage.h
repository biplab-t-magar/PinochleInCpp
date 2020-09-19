#pragma once
#ifndef MELD_STORAGE_H
#define MELD_STORAGE_H

//Stores:    Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
#include <MeldInstance.h>

class MeldsStorage {
public:
   MeldsStorage();
   bool addMeld(MeldInstance meldInstance);
   bool removeMeld( MeldInstance meldInstance);
   std::vector<std::vector<MeldInstance>> MeldsStorage::getAllPlayedMelds() const;
   bool isCardUsedByMeld(Card card, Meld meldType);
   int getNumOfMelds(Meld meldType);
private:
   std::vector<std::vector<MeldInstance>> storage;
   // std::vector<MeldInstance> FlushStorage;
   // std::vector<MeldInstance> RoyalMarriageStorage;
   // std::vector<MeldInstance> MarriageStorage;
   // std::vector<MeldInstance> DixStorage;
   // std::vector<MeldInstance> FourAceStorage;
   // std::vector<MeldInstance> FourKingStorage;
   // std::vector<MeldInstance> FourQueenStorage;
   // std::vector<MeldInstance> FourJackStorage;
   // std::vector<MeldInstance> PinochleStorage;

   // bool removeMeldFromStorage(MeldInstance meldInstance, std::vector<MeldInstance> &storage);
};

#endif