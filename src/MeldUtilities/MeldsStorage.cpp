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
}

bool MeldsStorage::addMelds(std::vector<MeldInstance> meldInstances) {
   bool allMeldsAdded = true;
   for(int i = 0; i < meldInstances.size(); i++) {
      if(addMeld(meldInstances[i]) == false) {
         allMeldsAdded = false;
      }
   }
   return allMeldsAdded;
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

int MeldsStorage::getNumOfMeldsByType(Meld meldType) {
   return storage[static_cast<int>(meldType)].size();
}

int MeldsStorage::getNumOfMelds() {
   int numOfMelds = 0;
   for(int i = 0; i < numOfMeldTypes; i++) {
      numOfMelds += storage[i].size();
   }
   return numOfMelds;
}

std::vector<std::vector<MeldInstance>> MeldsStorage::getAllMelds() const {
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
std::vector<MeldInstance> MeldsStorage::getAllMeldsByType(Meld meldType) const {
   return storage[static_cast<int>(meldType)];
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

std::vector<MeldInstance> MeldsStorage::getAllMeldsUsingCard(Card card) {
   std::vector<MeldInstance> allMeldsUsingCard;
   //loop through all meld types
   for(int i = 0; i < storage.size(); i++) {
      //loop through all instances of a meld type
      for(int j = 0; j < storage[i].size(); j++) {
         //if card is in the given meld instance, push it as one of the results
         if(storage[i][j].searchCardById(card.getId()) == true) {
            allMeldsUsingCard.push_back(storage[i][j]);
         }
      }
   }
   return allMeldsUsingCard;
}

bool MeldsStorage::cardsUsedForSameMeld(std::vector<Card> cards, Meld meldType) {
   //get int representation of the given meld
   int meldTypeInt = static_cast<int>(meldType);

   //get the number of instances of that particular meld type
   int numOfMeldsPlayed = storage[meldTypeInt].size();

   bool cardsUsedBySameMeld = true;
   for(int i = 0; i < numOfMeldsPlayed; i++) {
      for(int j= 0; j < cards.size(); j++) {
         //if one of the cards is not found in the meld instance currently being looped through
         if(storage[meldTypeInt][i].searchCardById(cards[j].getId()) != true) {
            cardsUsedBySameMeld =  false;
            break;
         }
      }
      //if there is a meld where all of the given cards are present
      if(cardsUsedBySameMeld == true) {
         return true;
      }
   }
   return false;
}

bool MeldsStorage::cardsUsedForSameMeld(MeldInstance meldInstance, Meld meldType) {
   std::vector<Card> cards;
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      cards.push_back(meldInstance.getCardByPosition(i));
   }
   return cardsUsedForSameMeld(cards, meldType);
}

bool MeldsStorage::isCardUsedByAnyMeld(Card card) {
   
   for(int meldType = 0; meldType < numOfMeldTypes; meldType++) {
      if(isCardUsedByMeld(card, static_cast<Meld>(meldType))) {
         return true;
      }
   }
   return false;
}
