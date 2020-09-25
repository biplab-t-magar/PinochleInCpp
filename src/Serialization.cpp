#include "Serialization.h"
#include "StringUtilities.h"

Serialization::Serialization(){
   playerStringsEntered = false;
   playerObjectsEntered = false;
}

void Serialization::setPlayerObjects(GroupOfCards hand, MeldsStorage meldsPlayed, GroupOfCards capturePile) {
   //first, store all the cards
   this->hand = hand;
   this->meldsPlayed = meldsPlayed;
   this->capturePile = capturePile;
   playerObjectsEntered = true;
   try {
      convertObjectsToString();
   } catch (PinochleException &e) {
      throw e;
   }
}

void Serialization::setPlayerStrings(std::string handString, std::string meldString, std::string captureString) {
   //first, store all the strings
   this->handString = handString;
   this->meldString = meldString;
   this->captureString = captureString;
   playerStringsEntered = true;
   try {
      void handStringToObjects();
   } catch(PinochleException &e) {
      throw e;
   }

   try {
      void meldStringToObjects();
   } catch(PinochleException &e) {
      throw e;
   }

   try {
      void captureStringToObjects();
   } catch(PinochleException &e) {
      throw e;
   }
}

void Serialization::convertObjectsToString() {
   if(!playerObjectsEntered) {
      throw PinochleException("Player Objects has not been specifiec, and so cannot be serialized.");
   }
   //for each card in hand, we count how many "complete" meld instances it is part of 
   //By "complete" meld instances, we mean those meld instances whose component cards are all still in hand
   //if any card of a previously played instance has already been thrown, it is not a complete meld instance

   //if a card has 0 such meld instances, the card goes to hand string
   //if a card has 1 such meld instances, the card goes to meld string
   //if a card has more than 1 such meld instances, the card goes to meld string marked by an asterisk(*)

   //a vector for the count of how many complete meld instances is each card a part of
   std::vector<int> meldInstanceCount;
   //initialize all counts to 0
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      meldInstanceCount.push_back(0);
   }

   //vector of every single meld instance a card is part of
   std::vector<MeldInstance> allMeldsUsingCard;
   //vector of every single "complete" meld instance
   std::vector<MeldInstance> completeMelds;
   //looping through all cards in hand
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      //get all the meld instances that the card is part of
      allMeldsUsingCard = meldsPlayed.getAllMeldsUsingCard(hand.getCardByPosition(i));

      //loop through all the meld instances that use this card and see if all the other cards in each meld instance are still in hand
      //keep count of those meld instances whose component card are all still in hand
      
      //loop through each meld instance that a card is part of
      for(int j = 0; j < allMeldsUsingCard.size(); j++) {
         if(isACompleteMeldInstance(allMeldsUsingCard[j])) {
            completeMelds.push_back(allMeldsUsingCard[j]);
            meldInstanceCount[i]++;
         }
      }
   }
   
   //now, create the hand string
    //create the hand string
   handString = "";
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      //if the card is part of no complete meld instance
      if(meldInstanceCount[i] == 0) {
         handString = handString + hand.getCardByPosition(i).getShortCardStr() + " ";
      }
   }

   //create capture string
   captureString = "";
   for(int i = 0; i < capturePile.getNumOfCards(); i++) {
      captureString = captureString + capturePile.getCardByPosition(i).getShortCardStr() + " ";
   }

   //create the meld string
   meldString = "";
   Card meldCard;
   //loop through all the complete meld instances
   for(int i = 0; i < completeMelds.size(); i++) {
      //loop through each card of a complete meld instance
      for(int j = 0; j < completeMelds[i].getNumOfCards(); j++) {
         //add each card to the string
         meldCard = completeMelds[i].getCardByPosition(j);
         meldString = meldString + meldCard.getShortCardStr();
         //if a card in the meld occurs in another complete meld as well, add an asterisk to it
         //refer to meldInstanceCount for number of occurences of card in other melds
         if(meldInstanceCount[hand.getCardPosition(meldCard)] > 1) {
            meldString = meldString + "*";
         }
         if(j < completeMelds[i].getNumOfCards() - 1) {
            meldString = meldString + " ";
         }
      }
      if(i < completeMelds.size() - 1; i++) {
         meldString = meldString + ", ";
      }
   }

}

void Serialization::convertStringToObjects() {
   
}



bool Serialization::isACompleteMeldInstance(MeldInstance meldInstance) {
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      if(!hand.searchCardById(meldInstance.getCardByPosition(i).getId())) {
         return false;
      }
   }
   return true;
}


std::string Serialization::getHandString() {
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return handString;
}
std::string Serialization::getCaptureString() {
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return captureString;
}
std::string Serialization::getMeldString() {
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return meldString;
}

GroupOfCards getHand();
MeldsStorage getMeldsPlayed();
GroupOfCards getCapturePile();


void Serialization 