/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#include "MeldsStorage.h"

#define numOfMeldTypes 9

/* *********************************************************************
Function Name: MeldsStorage
Purpose: Constructor for MeldsStorage class
Parameters: 
Return Value: 
Assistance Received: None
********************************************************************* */
MeldsStorage::MeldsStorage() {
   for(int i = 0; i < numOfMeldTypes; i++) {
      storage.push_back(std::vector<MeldInstance>());
   }
}

/* *********************************************************************
Function Name: addMeld
Purpose: Adds a meld instance to the melds storage
Parameters: 
      meldInstance, the meld instance to be added
Return Value: 
      true if successfully added, false if not added
Assistance Received: None
********************************************************************* */
bool MeldsStorage::addMeld(MeldInstance meldInstance) {
   if(meldInstance.isValidMeld() == false) {
      return false;
   }

   storage[static_cast<int>(meldInstance.getMeldType())].push_back(meldInstance);
   return true;
}

/* *********************************************************************
Function Name: addMelds
Purpose: Adds multiple meld instances to the melds storage
Parameters: 
      meldInstances, a vector of all the meld instances
Return Value: 
      true if melds were successfully added, false otherwise
Assistance Received: None
********************************************************************* */
bool MeldsStorage::addMelds(std::vector<MeldInstance> meldInstances) {
   bool allMeldsAdded = true;
   for(int i = 0; i < meldInstances.size(); i++) {
      if(addMeld(meldInstances[i]) == false) {
         allMeldsAdded = false;
      }
   }
   return allMeldsAdded;
}


/* *********************************************************************
Function Name: removeMeld
Purpose: Removes a given meld instance from the melds storage
Parameters: 
      meldInstance, the meld instance to be removed
Return Value: 
      returns true of removal was successful, false otherwise
Assistance Received: None
********************************************************************* */
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
}

/* *********************************************************************
Function Name: getNumOfMeldsByType
Purpose: Gets the number of melds of the given type stored in this MeldsStorage
Parameters: 
      meldType, the meld whose instances are to be counted
Return Value: 
      the number of meld instances of the given meld
Assistance Received: None
********************************************************************* */
int MeldsStorage::getNumOfMeldsByType(Meld meldType) {
   return storage[static_cast<int>(meldType)].size();
}

/* *********************************************************************
Function Name: getNumOfMelds
Purpose: gets the number of meld instances stored
Parameters: 
Return Value: 
      the number of meld instances stored
Assistance Received: None
********************************************************************* */
int MeldsStorage::getNumOfMelds() {
   int numOfMelds = 0;
   for(int i = 0; i < numOfMeldTypes; i++) {
      numOfMelds += storage[i].size();
   }
   return numOfMelds;
}

/* *********************************************************************
Function Name: getAllMelds
Purpose: Returns the vector of all the meld instances stored
Parameters: 
Return Value: 
      vector of vectors of MeldInstance objects, comprising all the meld instances stored 
Assistance Received: None
********************************************************************* */
std::vector<std::vector<MeldInstance>> MeldsStorage::getAllMelds() const {
   return storage;
}

/* *********************************************************************
Function Name: getAllMeldsByType
Purpose: Returns all the meld instances of the given type
Parameters: 
      meldType, the meld whose instances are to be returned
Return Value: 
      all the meld instances of the given type
Assistance Received: None
********************************************************************* */
std::vector<MeldInstance> MeldsStorage::getAllMeldsByType(Meld meldType) const {
   return storage[static_cast<int>(meldType)];
}


/* *********************************************************************
Function Name: isCardUsedByMeld
Purpose: Finds out whether a card has been used to create a meld instance of a particular meld type
Parameters: 
      card, the card which is to be checked 
      meldType, the meld whose instances the card is to be checked in
Return Value: 
      true if card is used by a meld instance of the given meld, false otherwise
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: getAllMeldsUsingCard
Purpose: returns all the meld instances that make use of the given card
Parameters: 
      card, the card which is part of the meld instances to be returned
Return Value: 
      vector of MeldInstances that make use of the cards
Assistance Received: None
********************************************************************* */
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


/* *********************************************************************
Function Name: cardsUsedForSameMeld
Purpose: checks if a group of cards have been used together to create a meld before
Parameters: 
      cards, the vector of the cards to be checked
Return Value: 
      true if the group of cards have together been used to create a meld before, false otherwise
Local Variables: 
      numOfMeldsPlayed, to store the number of meld instances of a particular meld type
      cardsUsedBySameMeld, to store the boolean of whether the cards were used by the same meld instance
Algorithm: 
      1) for each meld instance of each meld in the storage, 
      2)    for each card in the given cards
      3)       check if a card is used by a meld instance
      4)          if no, move on to the next meld instance and repeat 
      5)          if yes, do the same for the rest of the given cards
      6)       if all the given cards result in a yes for any meld instance, then return true
      7) if there are no meld instances that contain each of the given cards, return false
Assistance Received: None
********************************************************************* */
bool MeldsStorage::cardsUsedForSameMeld(std::vector<Card> cards) {
   //get int representation of the given meld

   //get the number of instances of that particular meld type
   int numOfMeldsPlayed;

   //loop through every meld type
   for(int meldTypeInt = 0; meldTypeInt < numOfMeldTypes; meldTypeInt++) {
      numOfMeldsPlayed = storage[meldTypeInt].size();
      //loop through each instance of a meld type
      for(int i = 0; i < numOfMeldsPlayed; i++) {
         bool cardsUsedBySameMeld = true;
         //chech if all cards are present in the meld instance
         for(int j= 0; j < cards.size(); j++) {
            //if one of the cards is not found in the meld instance currently being looped through
            if(storage[meldTypeInt][i].searchCardById(cards[j].getId()) != true) {
               cardsUsedBySameMeld =  false;
               break;
            }
         }
         //if a meld instance with all the cards in it has been found, return true;
         if(cardsUsedBySameMeld == true) {
            return true;
         }
      }
   }
   return false;
}

/* *********************************************************************
Function Name: cardsUsedForSameMeld
Purpose: checks if a group of cards have been used together to create a meld before
Parameters: 
      meldInstance, a MeldInstance object storing the cards to be checked
Return Value:
      true if the group of cards comprising the meld intance have together been used to create a meld before, false otherwise
Assistance Received: None
********************************************************************* */
bool MeldsStorage::cardsUsedForSameMeld(MeldInstance meldInstance) {
   std::vector<Card> cards;
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      cards.push_back(meldInstance.getCardByPosition(i));
   }
   return cardsUsedForSameMeld(cards);
}


/* *********************************************************************
Function Name: isCardUsedByAnyMeld
Purpose: Find out whether a card has been used to create any meld before
Parameters: 
      card, the card to be checked for use by meld instances
Return Value:
      true if the card has been used by a meld instance, false if not
Assistance Received: None
********************************************************************* */
bool MeldsStorage::isCardUsedByAnyMeld(Card card) {
   
   for(int meldType = 0; meldType < numOfMeldTypes; meldType++) {
      if(isCardUsedByMeld(card, static_cast<Meld>(meldType))) {
         return true;
      }
   }
   return false;
}

