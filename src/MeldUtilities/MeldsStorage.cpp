#include "MeldsStorage.h"

#define numOfMeldTypes 9

MeldsStorage::MeldsStorage() {
   for(int i = 0; i < numOfMeldTypes; i++) {
      storage.push_back(std::vector<MeldInstance>());
   }
}

bool MeldsStorage::addMeld(MeldInstance meldInstance) {
   if(meldInstance.isValidMeld() == false) {
      return false;
   }

   storage[static_cast<int>(meldInstance.getMeldType())].push_back(meldInstance);
   return true;

   // switch (meldInstance.getMeldType()) {
   //    case Meld::Flush:
   //       FlushStorage.push_back(meldInstance);
   //       break;
   //    case Meld::RoyalMarriage:
   //       RoyalMarriageStorage.push_back(meldInstance);
   //       break;
   //    case Meld::Marriage:
   //       MarriageStorage.push_back(meldInstance);
   //       break;
   //    case Meld::Dix:
   //       DixStorage.push_back(meldInstance);
   //       break;
   //    case Meld::FourAces:
   //       FourAceStorage.push_back(meldInstance);
   //       break;
   //    case Meld::FourKings:
   //       FourKingStorage.push_back(meldInstance);
   //       break;
   //    case Meld::FourQueens:
   //       FourQueenStorage.push_back(meldInstance);
   //       break;
   //    case Meld::FourJacks:
   //       FourJackStorage.push_back(meldInstance);
   //       break;
   //    case Meld::Pinochle:
   //       PinochleStorage.push_back(meldInstance);
   //       break;
   //    default:
   //       return false;
   // }
   // return true;
}



bool MeldsStorage::removeMeld(MeldInstance meldInstance) {
   //get meld type of the given meld (in integer representation)
   int meldTypeInt = static_cast<int>(meldInstance.getMeldType());
   //get the amount of melds of that particular meld type
   int numOfMeldsPlayed = storage[meldTypeInt].size();
   for(int i = 0; i < numOfMeldsPlayed; i++) {
      if(storage[meldTypeInt][i] == meldInstance) {
         storage[meldTypeInt].erase(storage[meldTypeInt].begin() + i);
         return true;
      }
   }
   return false;
   // switch (meldInstance.getMeldType()) {
   //    case Meld::Flush:
   //       return removeMeldFromStorage(meldInstance, FlushStorage);
   //       break;
   //    case Meld::RoyalMarriage:
   //       return removeMeldFromStorage(meldInstance, RoyalMarriageStorage);
   //       break;
   //    case Meld::Marriage:
   //       return removeMeldFromStorage(meldInstance, MarriageStorage);
   //       break;
   //    case Meld::Dix:
   //       return removeMeldFromStorage(meldInstance, DixStorage);
   //       break;
   //    case Meld::FourAces:
   //       return removeMeldFromStorage(meldInstance, FourAceStorage);
   //       break;
   //    case Meld::FourKings:
   //       return removeMeldFromStorage(meldInstance, FourKingStorage);
   //       break;
   //    case Meld::FourQueens:
   //       return removeMeldFromStorage(meldInstance, FourQueenStorage);
   //       break;
   //    case Meld::FourJacks:
   //       return removeMeldFromStorage(meldInstance, FourJackStorage);
   //       break;
   //    case Meld::Pinochle:
   //       return removeMeldFromStorage(meldInstance, PinochleStorage);
   //       break;
   //    default:
   //       return false;
   // }
   // return true;
}


// bool MeldsStorage::removeMeldFromStorage(MeldInstance meldInstance, std::vector<MeldInstance> &storage) {
//    for(int i = 0; i < storage.size(); i++) {
//       if(storage[i] == meldInstance) {
//          storage.erase(storage.begin() + i);
//          return true;
//       }
//    }
//    return false;
// }

int MeldsStorage::getNumOfMelds(Meld meldType) {
   return storage[static_cast<int>(meldType)].size();
}

std::vector<std::vector<MeldInstance>> MeldsStorage::getAllPlayedMelds() const {
   // std::vector<std::vector<MeldInstance>> allPlayedMelds;
   // allPlayedMelds.push_back(FlushStorage);
   // allPlayedMelds.push_back(RoyalMarriageStorage);
   // allPlayedMelds.push_back(MarriageStorage);
   // allPlayedMelds.push_back(DixStorage);
   // allPlayedMelds.push_back(FourAceStorage);
   // allPlayedMelds.push_back(FourKingStorage);
   // allPlayedMelds.push_back(FourQueenStorage);
   // allPlayedMelds.push_back(FourJackStorage);
   // allPlayedMelds.push_back(PinochleStorage);

   return storage;
}



bool MeldsStorage::isCardUsedByMeld(Card card, Meld meldType) {
   //get int representation of the given meld
   int meldTypeInt = static_cast<int>(meldType);
   //get the number of instances of that particular meld type
   int numOfMeldsPlayed = storage[meldTypeInt].size();

   for(int i = 0; i < numOfMeldsPlayed; i++) {
      //if that card is found in a meld instance, return true
      if(storage[meldTypeInt][i].searchCardById(card.getId()) == true) {
         return true;
      }
   }
   //return false;
   return false;
}
