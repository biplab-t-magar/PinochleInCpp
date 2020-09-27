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
      convertObjectsToStrings();
   } catch (PinochleException &e) {
      throw e;
   }
}

GroupOfCards Serialization::setPlayerStrings(std::string handString, std::string meldString, std::string captureString, GroupOfCards allRemCards, Suit trumpSuit) {
   //first, store all the strings
   this->handString = handString;
   this->meldString = meldString;
   this->captureString = captureString;
   playerStringsEntered = true;
   
   try {
       //calculate hand
      allRemCards = handStrToObject(allRemCards);
   } catch (PinochleException &e){
      throw e;
   }

   try {
      //get capturePile
      allRemCards = captureStrToObject(allRemCards);
      //getMeldPile
   } catch (PinochleException &e){
      throw e;
   }

   try {
      //get capturePile
      allRemCards = meldStrToObject(allRemCards, trumpSuit);
      //getMeldPile
   } catch (PinochleException &e){
      throw e;
   }

   return allRemCards;
}

void Serialization::convertObjectsToStrings() {
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
   bool meldEncounteredBefore;
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      //get all the meld instances that the card is part of
      allMeldsUsingCard = meldsPlayed.getAllMeldsUsingCard(hand.getCardByPosition(i));

      //loop through all the meld instances that use this card and see if all the other cards in each meld instance are still in hand
      //keep count of those meld instances whose component card are all still in hand
      
      //loop through each meld instance that a card is part of
      for(int j = 0; j < allMeldsUsingCard.size(); j++) {
         meldEncounteredBefore = false;
         if(isACompleteMeldInstance(allMeldsUsingCard[j])) {
            //check if the meld instance has already been encountered (through another card that is part of the meld)
            for(int k = 0; k < completeMelds.size(); k++) {
               if(completeMelds[k] == allMeldsUsingCard[j]) {
                  meldEncounteredBefore = true;
                  break;
               }
            }
            //only push the meld if it was not encountered before
            if(!meldEncounteredBefore) {
               completeMelds.push_back(allMeldsUsingCard[j]);
            }
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
      if(i < completeMelds.size() - 1) {
         meldString = meldString + ", ";
      }
   }

}

GroupOfCards Serialization::handStrToObject(GroupOfCards allRemCards) {
   //first, get the invidual cards from the hand string and add them to hand
   //none of the cards in the hand string are assumed to be a part of the meld

   //getting individual card strings from hand string
   Card handCard;
   std::vector<std::string> handCardStrs;
   try {
      handCardStrs = StringUtilities::splitCardsInString(handString);
   } catch(PinochleException &e) {
      throw e;
   }
   
   //check if all card strings are of correct length (i.e. check if any card contains asterisk )
   for(int i = 0; i < handCardStrs.size(); i++) {
      if(handCardStrs[i].size() > 2) {
         throw PinochleException(handCardStrs[i] + " is an invalid card to put in the hand serialization");
      }
      try {
         handCard = StringUtilities::strToCard(handCardStrs[i]);
         //find first instance of the card in allRemCards 
         if(allRemCards.getCardsByRankAndSuit(handCard.getRank(), handCard.getSuit()).size() == 0) {
            throw PinochleException("This card cannot be present in the because it has already been used somewhere else");
         }
         handCard = allRemCards.getCardsByRankAndSuit(handCard.getRank(), handCard.getSuit())[0];
         //remove the instance from allRemCards
         allRemCards.removeCardById(handCard.getId());
         //add hand card to hand
         hand.addCard(handCard);
      } catch (PinochleException &e) {
         throw e;
      }
   }
   return allRemCards;
}

GroupOfCards Serialization::captureStrToObject(GroupOfCards allRemCards) {
   //first, get the invidual cards from the capture string and add them to capturePile
   //none of the cards in the capture string are assumed to be a part of the meld

   //getting individual card strings from capture string
   Card captureCard;
   std::vector<std::string> captureCardStrs;
   try {
      captureCardStrs = StringUtilities::splitCardsInString(captureString);
   } catch(PinochleException &e) {
      throw e;
   }
   
   //check if all card strings are of correct length (i.e. check if any card contains asterisk )
   for(int i = 0; i < captureCardStrs.size(); i++) {
      if(captureCardStrs[i].size() > 2) {
         throw PinochleException(captureCardStrs[i] + " is an invalid card to put in the hand serialization");
      }
      try {
         captureCard = StringUtilities::strToCard(captureCardStrs[i]);
         //find first instance of the card in allRemCards 
         if(allRemCards.getCardsByRankAndSuit(captureCard.getRank(), captureCard.getSuit()).size() == 0) {
            throw PinochleException("This card cannot be present in the because it has already been used somewhere else");
         }
         captureCard = allRemCards.getCardsByRankAndSuit(captureCard.getRank(), captureCard.getSuit())[0];
         //remove the instance from allRemCards
         allRemCards.removeCardById(captureCard.getId());
         //add capture card to capturePile
         capturePile.addCard(captureCard);
      } catch (PinochleException &e) {
         throw e;
      }
   }
   return allRemCards;
}

GroupOfCards Serialization::meldStrToObject(GroupOfCards allRemCards, Suit trumpSuit) {
   //first split the string into component melds
   std::vector<std::vector<std::string>> meldVector;
   try {
      meldVector = StringUtilities::splitMeldsInString(meldString);
   } catch(PinochleException &e) {
      throw e;
   }
   

   //for handling repetition of cards in mmeld
   bool cardHasAsterisk;
   //to hold a card temporarily
   Card cardHolder;
   //to hold cards temporarily
   std::vector<Card> cardsHolder;
   MeldInstance meldInstance;
   std::vector<Card> cardsWithAstrk;
   std::vector<Card> cardsWithoutAstrk;
   std::vector<Card> cardsExtracted;
   // std::vector<MeldInstance> meldsToBeStored;
   bool cardWasExtracted;
   //loop through each meld instance
   for(int i = 0; i < meldVector.size(); i++) {
      cardsWithAstrk.clear();
      cardsWithoutAstrk.clear();
      meldInstance.removeAllCards();
      //loop through all the cards in a meld instance
      for(int j = 0; j < meldVector[i].size(); j++) {
         cardHasAsterisk = false;
         //check if card string is valid
         if(meldVector[i][j].size() == 3) {
            if(meldVector[i][j][2] == '*') {
               cardHasAsterisk = true;
            } else {
               throw PinochleException("Invalid card in meld string");
            }
         } 
         //if the card is valid
         if(meldVector[i][j].size() == 2 || meldVector[i][j].size() == 3) {
            if(cardHasAsterisk) {
               cardHolder = StringUtilities::strToCard(meldVector[i][j].substr(0, 2));
               cardsWithAstrk.push_back(cardHolder);
            } else {
               cardHolder = StringUtilities::strToCard(meldVector[i][j]);
               cardsWithoutAstrk.push_back(cardHolder);
            }
         } else {
            throw PinochleException("Invalid card in meld string");
         }
      }
      
      //checking if all the cards with asterisks have already been extracted from allRemCards or not
      for(int n = 0; n < cardsWithAstrk.size(); n++) {
         //go through cardsExracted to search for unextracted cards
         cardWasExtracted = false;
         for(int m = 0; m < cardsExtracted.size(); m++) {
            //if the card was previously extracted
            if(cardsExtracted[m].compareRankAndSuit(cardsWithAstrk[n]) == true) {
               cardWasExtracted = true;
               cardsWithAstrk[n] = cardsExtracted[m];
               break;
            } 
         }
         //if the card was not previously extracted, extract it from allRemCards
         if(cardWasExtracted == false) {
            cardsHolder = allRemCards.getCardsByRankAndSuit(cardsWithAstrk[n].getRank(), cardsWithAstrk[n].getSuit());
            if(cardsHolder.size() == 0) {
               throw PinochleException("The card " + cardsWithoutAstrk[n].getShortCardStr() + " could not be extracted. There may one or more "
                  + "extra instances of this card in the serialization file. Make sure there are only two instances of this card.\n\n");
            }
            cardsWithAstrk[n] = cardsHolder[0];

            allRemCards.removeCardById(cardsWithAstrk[n].getId());
            cardsExtracted.push_back(cardsWithAstrk[n]);
            //each time a card is extracted, also add it to the hand
            hand.addCard(cardsWithAstrk[n]);
         }
         //add the card to the meld instance object
         meldInstance.addCard(cardsWithAstrk[n], trumpSuit);
      }
      //now, extracting all non-asterisk cards from allRemCards
      for(int n = 0; n < cardsWithoutAstrk.size(); n++) {
         cardsHolder = allRemCards.getCardsByRankAndSuit(cardsWithoutAstrk[n].getRank(), cardsWithoutAstrk[n].getSuit());
         if(cardsHolder.size() == 0) { 
            throw PinochleException("The card " + cardsWithoutAstrk[n].getShortCardStr() + " could not be extracted. There may one or more "
               + "extra instances of this card in the serialization file. Make sure there are only two instances of this card.\n\n");
         }
         cardsWithoutAstrk[n] = allRemCards.getCardsByRankAndSuit(cardsWithoutAstrk[n].getRank(), cardsWithoutAstrk[n].getSuit())[0];
         
         allRemCards.removeCardById(cardsWithoutAstrk[n].getId());
         cardsExtracted.push_back(cardsWithoutAstrk[n]);
         meldInstance.addCard(cardsWithoutAstrk[n], trumpSuit);
         //each time a card is extracted, also add it to the hand
         hand.addCard(cardsWithoutAstrk[n]);
      }
      //check if valid meld instance has been created
      if(meldInstance.isValidMeld() == false) {
         throw PinochleException("Error in serialization. Invalid meld is listed.");
      }

      meldsPlayed.addMeld(meldInstance);
      // //get all the meld instances from meldsPlayed that have previously used any of the asterisked cards 
      // for(int n = 0; n < cardsWithAstrk.size(); n++) {

      // }
   }
   return allRemCards;
}

bool Serialization::isACompleteMeldInstance(MeldInstance meldInstance) {
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      if(!hand.searchCardById(meldInstance.getCardByPosition(i).getId())) {
         return false;
      }
   }
   return true;
}


std::string Serialization::getHandString() const {
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return handString;
}
std::string Serialization::getCaptureString() const{
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return captureString;
}
std::string Serialization::getMeldString() const{
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return meldString;
}

GroupOfCards Serialization::getHand() const {
   if(playerStringsEntered == false) {
      throw PinochleException("Player card strings have not been entered yet");
   }
   return hand;
}
MeldsStorage Serialization::getMeldsPlayed() const {
   if(playerStringsEntered == false) {
      throw PinochleException("Player card strings have not been entered yet");
   }
   return meldsPlayed;
}
GroupOfCards Serialization::getCapturePile() const{
   if(playerStringsEntered == false) {
      throw PinochleException("Player card strings have not been entered yet");
   }
   return capturePile;
}


