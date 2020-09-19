#include "MeldsStorage.h"

bool MeldsStorage::addMeld(MeldInstance meldInstance) {
   switch (meldInstance.getMeldType()) {
      case Meld::Flush:
         FlushStorage.push_back(meldInstance);
         break;
      case Meld::RoyalMarriage:
         RoyalMarriageStorage.push_back(meldInstance);
         break;
      case Meld::Marriage:
         MarriageStorage.push_back(meldInstance);
         break;
      case Meld::Dix:
         DixStorage.push_back(meldInstance);
         break;
      case Meld::FourAces:
         FourAceStorage.push_back(meldInstance);
         break;
      case Meld::FourKings:
         FourKingStorage.push_back(meldInstance);
         break;
      case Meld::FourQueens:
         FourQueenStorage.push_back(meldInstance);
         break;
      case Meld::FourJacks:
         FourJackStorage.push_back(meldInstance);
         break;
      case Meld::Pinochle:
         PinochleStorage.push_back(meldInstance);
         break;
      default:
         return false;
   }
   return true;
}



bool MeldsStorage::removeMeld(MeldInstance meldInstance) {
   switch (meldInstance.getMeldType()) {
      case Meld::Flush:
         return removeMeldFromStorage(meldInstance, FlushStorage);
         break;
      case Meld::RoyalMarriage:
         return removeMeldFromStorage(meldInstance, RoyalMarriageStorage);
         break;
      case Meld::Marriage:
         return removeMeldFromStorage(meldInstance, MarriageStorage);
         break;
      case Meld::Dix:
         return removeMeldFromStorage(meldInstance, DixStorage);
         break;
      case Meld::FourAces:
         return removeMeldFromStorage(meldInstance, FourAceStorage);
         break;
      case Meld::FourKings:
         return removeMeldFromStorage(meldInstance, FourKingStorage);
         break;
      case Meld::FourQueens:
         return removeMeldFromStorage(meldInstance, FourQueenStorage);
         break;
      case Meld::FourJacks:
         return removeMeldFromStorage(meldInstance, FourJackStorage);
         break;
      case Meld::Pinochle:
         return removeMeldFromStorage(meldInstance, PinochleStorage);
         break;
      default:
         return false;
   }
   return true;
}

bool MeldsStorage::removeMeldFromStorage(MeldInstance meldInstance, std::vector<MeldInstance> &storage) {
   for(int i = 0; i < storage.size(); i++) {
      if(storage[i] == meldInstance) {
         storage.erase(storage.begin() + i);
         return true;
      }
   }
   return false;
}


bool MeldsStorage::isCardUsedByMeld(int cardId, Meld meldType) {

}

