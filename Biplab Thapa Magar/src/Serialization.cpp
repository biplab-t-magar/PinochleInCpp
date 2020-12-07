/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#include "Serialization.h"
#include "StringUtilities.h"

/* *********************************************************************
Function Name: Serialization
Purpose: Constructor for the Serialization object
Parameters: 
Return Value:
Assistance Received: None
********************************************************************* */
Serialization::Serialization(){
   playerStringsEntered = false;
   playerObjectsEntered = false;
}

/* *********************************************************************
Function Name: setPlayerObjects
Purpose: Receives player hand, meldsPlayed, and capturePile objects in order to serialize them to string form
Parameters: 
      hand, the hand of a player, of GroupOfCards type
      meldsPlayed, the melds played by a player, of MeldsStorage data type
      capturePile, the capture pile of a player, of GroupOfCards type
Return Value:
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: setPlayerStrings
Purpose: Receives the string representation of the player's hand, melds, and capture pile, (along with other information needed to 
         determine the player's information) in order to convert them object form
Parameters: 
      handString: the string representation of the player's hand pile 
      meldString: the string representation of the player's meld pile
      captureString: the string representation of the player's capture pile
      allRemCards: a GroupOfCards object representing all the cards that are not in the stock pile
      suit: the trump suit of the round
Return Value:
      return a GroupOfCards that includes all the cards except those in the stock pile and except those possessed by the player
Local Variables: 
Algorithm: 
      1) Generate the player's hand object and store the returned group of cards
      2) Generate the player's capture pile object (supplying it with the group of cards returned from step 1) and store the returned group of cards
      3) Generate the player's meld pile object (supplying it with the group of cards returned from step 2) and store the returned group of cards
      4) Return the group of cards stored in step 3
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: convertObjectsToStrings
Purpose: 
      Converts the stored hand, capturePile, and meldsPlayed object into string representation
Parameters: 
Return Value:
Local Variables: 
      meldInstanceCount, to hold the number of meld instances that a card is part of
      allMeldsUsingCard, to hold all the meld instances that a card is part of
      completeMelds, to hold all the "complete melds", i.e. those meld instances that have all their constituent cards still in hand
      meldEncounteredBefore, to keep track of whether a card has been encountered before in the hand
Algorithm: 
      1) for each card in hand:
      2)    for each meld instance that the card is part of
      3)         if the meld instance is a "complete meld instance", as defined above, then 
      4)               check if the meld instance has already been encountered before (through another card in hand)
      5)                   if not encountered before, then store the meld instance
      6)                   if encountered before, then do not store the meld instance
      7)               increment the count for the number of meld instances that the card is part of
      8) create a hand string representation from the hand object, comprising of only those cards that have meld instance count of 0
      9) create a capture string from the capturePile object
      10) create a meld string representation of all the cards in hand with more than 0 meld instances,
               and if the card has more than 1 meld instances, add an asterick next to it 
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: handStrToObject
Purpose: Converts string representation of hand to hand object
Parameters: 
      allRemCards, all the cards that have not yet been used up to create other card piles (like stock, other player's hand, etc) in the round
Return Value:
      group of all the cards remaining after removing the cards needed by the player's hand
Local Variables: 
      handCard, stores a card to be assigned to the player's hand
      handCardStrs, to store, as elements in a vector, all the cards (int string form) of the hand player
Algorithm: 
      1) Split the hand string into individual cards (these cards have only rank and suit, but no id. They are only used for finding an actual game card with a proper id)
      2) Extract the first instance of each card from the string with the needed rank and suit from the given group of all remaining cards.
               This is the instance that represents the actual card to be added to hand. 
      3) Put each of the extracted cards into hand
      4) return the group of all cards remaining after extracting cards for the hand 
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: captureStrToObject
Purpose: Converts string representation of capture pile to capture pile object
Parameters: 
      allRemCards, all the cards that have not yet been used up to create other card piles (like stock, other player's hand, etc) in the round
Return Value:
      group of all the cards remaining after removing the cards needed by the player's capture pile
Local Variables: 
      captureCard, to hold a card from the capture pile
      captureCardStrs, a vector of all the cards in the capture file, in string form
Algorithm: 
      1) Split the capture string into individual cards (these cards have only rank and suit, but no id. They are only used for finding an actual game card with a proper id)
      2) Extract the first instance of the card with the needed rank and suit from the given group of all remaining cards .
               This is the instance that represents the actual card to be added to the capture pile 
      3) Put each of the extracted cards into capture pile
      4) return the group of all cards remaining after extracting cards for the capture pile 
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: meldStrToObject
Purpose: Converts string representation of melds to meld pile object and also adds cards from the string to hand 
Parameters: 
      allRemCards, all the cards that have not yet been used up to create other card piles (like stock, other player's hand, etc) in the round
Return Value:
      group of all the cards remaining after removing the cards needed by the player's meld and hands
Local Variables: 
      meldVector, to separately store all melds instances from the meld string in string form
      cardHasAsterisk, to check if a card has an asterisk symbol, in order to decide if it is a repeated card
      cardsHolder, to temprorarily hold a collection of cards
      meldInstance, to hold a meld instance parsed from the meld string
      cardsWithAsterisk, to hold all the cards that appear with an asterisk
      cardsWithoutAsterisk, to hold all the cards that do not appear with an asterisk
      cardsExtracted, to hold the cards extracted from the group of remaining cards
      cardWasExtracted, to keep track of whether a card has been extracted from the group of remaining cards
Algorithm: 
      1) get the individual meld instance strings from the meld string and store them
      2) for each individual meld instance string
      3)       loop through all the cards in the instance and keep track of whether each card has asterisk or not
      4)       for each card with an asterisk in the meld instance, check if the card has already been extracted or not
      5)             if a card with an asterisk has not already been extracted, extract it and put it into the list of extracted cards and into the player's hand
      6)             whether or not the cards were already been extracted, add them to the same meld instance 
      7)       extract each non-asterisk card from the group of all remaining cards and add it the same meld instance and to the hand
      8)       check if the meld instance created from adding all asterisk and non-asterisk cards form a valid meld
      9)             if they do, add the meld instance to the meldsPlayed object
      10)return the group of all cards remaining after extracting cards for the meld and hand cards
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: isACompleteMeldInstance
Purpose: Checks whether a meld instance is "complete", i.e. whether all the cards
            that comprise the meld instance are still in the player's hand
Parameters: 
      meldInstance, the instance of the meld to be checked for completeness
Return Value:
      true if meld instance is complete, false otherwise
Assistance Received: None
********************************************************************* */
bool Serialization::isACompleteMeldInstance(MeldInstance meldInstance) {
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      if(!hand.searchCardById(meldInstance.getCardByPosition(i).getId())) {
         return false;
      }
   }
   return true;
}

/* *********************************************************************
Function Name: getHandString
Purpose: Returns the string representation of the player's hand
Parameters: 
Return Value:
      string representation of the player's hand
Assistance Received: None
********************************************************************* */
std::string Serialization::getHandString() const {
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return handString;
}

/* *********************************************************************
Function Name: getCaptureString
Purpose: Returns the string representation of the player's capture pile
Parameters: 
Return Value:
      string representation of the player's capture pile
Assistance Received: None
********************************************************************* */
std::string Serialization::getCaptureString() const{
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return captureString;
}

/* *********************************************************************
Function Name: getMeldString
Purpose: Returns the string representation of the player's melds
Parameters: 
Return Value:
      string representation of the player's melds
Assistance Received: None
********************************************************************* */
std::string Serialization::getMeldString() const{
   if(playerObjectsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return meldString;
}

/* *********************************************************************
Function Name: getHand
Purpose: Returns the object representation of the player's hand
Parameters: 
Return Value:
      object representation of the player's hand
Assistance Received: None
********************************************************************* */
GroupOfCards Serialization::getHand() const {
   if(playerStringsEntered == false) {
      throw PinochleException("Player card strings have not been entered yet");
   }
   return hand;
}

/* *********************************************************************
Function Name: getMeldsPlayed
Purpose: Returns the object representation of the player's melds played
Parameters: 
Return Value:
      object representation of the player's melds played
Assistance Received: None
********************************************************************* */
MeldsStorage Serialization::getMeldsPlayed() const {
   if(playerStringsEntered == false) {
      throw PinochleException("Player card strings have not been entered yet");
   }
   return meldsPlayed;
}

/* *********************************************************************
Function Name: getCapturePile
Purpose: Returns the object representation of the player's capture pile
Parameters: 
Return Value:
      object representation of the player's capture pile
Assistance Received: None
********************************************************************* */
GroupOfCards Serialization::getCapturePile() const{
   if(playerStringsEntered == false) {
      throw PinochleException("Player card strings have not been entered yet");
   }
   return capturePile;
}


