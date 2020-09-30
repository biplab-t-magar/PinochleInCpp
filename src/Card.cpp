/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/

#include "Card.h"
#define numOfRanks 6
#define numOfSuits 4


/* *********************************************************************
Function Name: Card
Purpose: Default constructor for Card class that initializes a card object. Creates a Card with undefined rank, suit, and id
Parameters: 
Return Value: 
Assistance Received: None
********************************************************************* */
Card::Card() {
   rankInitialized = false;
   suitInitialized = false;
   idInitialized = false;
}

/* *********************************************************************
Function Name: Card
Purpose: Overloaded constructor for Card class that initializes a card object. 
      Creates a Card with a rank, suit, and id based on the values sent into the parameter
Parameters: 
      id, the id to be assigned to the card
      rank, the rank to be assigned to the card
      suit, the suit to be assigned to the card
Return Value: 
Assistance Received: none
********************************************************************* */
Card::Card(int id, Rank rank, Suit suit) {
   if(static_cast<int>(rank) >= numOfRanks || static_cast<int>(rank) < 0) {
      throw PinochleException("invalid rank");
   }
   if(static_cast<int>(suit) >= numOfSuits || static_cast<int>(suit) < 0) {
      throw PinochleException("invalid suit");
   }

   this->id = id;
   this->rank = rank;
   this->suit = suit;
   idInitialized = true;
   rankInitialized = true;
   suitInitialized = true;
}

/* *********************************************************************
Function Name: getRank
Purpose: a selector function that returns the id of the card
Parameters: 
Return Value: 
      the id of the card, an int value
Assistance Received: none
********************************************************************* */
int Card::getId() const {
   if(!idInitialized) {
      throw PinochleException("Id for this card has not been specified");
   } 

   return id;
    
}


/* *********************************************************************
Function Name: getRank
Purpose: a selector function to that returns the rank of the card
Parameters: 
Return Value: 
      the rank of the card, of type Rank
Local Variables: 
Assistance Received: none
********************************************************************* */
Rank Card::getRank() const {
   //only return rank if card's rank has been marked as initialized
   if(rankInitialized) {
      return rank;
   } else {
      throw PinochleException("Rank for this card has not been specified");
   }
}


/* *********************************************************************
Function Name: getSuit
Purpose: a selector function to that returns the suit of the card
Parameters: 
Return Value: 
      the suit of the card, of type Suit
Assistance Received: none
********************************************************************* */
Suit Card::getSuit() const {
   //only return rank if card's rank has been marked as initialized
   if(suitInitialized) {
      return suit;
   } else {
      throw PinochleException("Suit for this card has not been specified");
   }
}

/* *********************************************************************
Function Name: getRankString
Purpose: a selector function that returns the rank of the card in string format
Parameters: 
Return Value: 
      the rank of the card, as a string
Assistance Received: none
********************************************************************* */
std::string Card::getRankString() const {
   //only return rank string if card's rank has been marked as initialized
   if(rankInitialized) {
      switch(rank) {
         case Rank::Nine:
               return "Nine";
               break;
         case Rank::Jack:
               return "Jack";
               break;
         case Rank::Queen:
               return "Queen";
               break;
         case Rank::King:
               return "King";
               break;
         case Rank::Ten:
               return "Ten";
               break;
         case Rank::Ace:
               return "Ace";
               break;
         default:
               throw PinochleException("Unrecognized Rank");
               break;
      }
   } else {
      throw PinochleException("Rank for this card has not been specified");
   }
}

/* *********************************************************************
Function Name: getSuitString
Purpose: a selector function that returns the suit of the card in string format
Parameters: 
Return Value: 
      the suit of the card, as a string
Assistance Received: none
********************************************************************* */
std::string Card::getSuitString() const {
   //only return suit string if card's suit has been marked as initialized
   if(suitInitialized) {
      switch(suit) {
         case Suit::Clubs:
               return "Clubs";
               break;
         case Suit::Diamonds:
               return "Diamonds";
               break;
         case Suit::Hearts:
               return "Hearts";
               break;
         case Suit::Spades:
               return "Spades";
               break;
         default:
               throw PinochleException("Unrecognized Suit");
               break;
      }
   } else {
      throw PinochleException("Suit for this card has not been specified");
   }
}

/* *********************************************************************
Function Name: getCardString
Purpose: a selector function to that returns the rank and suit of the card in string format
Parameters: 
Return Value: 
      the rank and suit of the card, as a single string, in "<rank> of <suit>"" formation
Local Variables: 
      cardString, to hold the card string to the returned
Algorithm: 
      1) Check if rank and string have both been initialized for the card
      2) if not, throw exception
      3) if yes, get the card's rank and string and combine to a string
      4) return the combined string
Assistance Received: none
********************************************************************* */
std::string Card::getCardString() const {

   //check if suit or rank have not been initialized 
   if(!suitInitialized || !rankInitialized) {
      throw PinochleException("Suit or rank for this card have not been specified");
   } else {
      //cardString holds the string to be returned
      std::string cardString;
      try {
         //here, both getRankString() and getSuitString() can return exceptions if either
         //one has not been initialized. Catch exception and pass it up
         cardString = getRankString() + " of " + getSuitString();
      } catch (PinochleException &e) {
         throw e;
      }
      return cardString;
   }
}

/* *********************************************************************
Function Name: getShortCardStr
Purpose: a selector function to that returns the rank and suit of the card in shortened string format
Parameters: 
Return Value: 
      the rank and suit of the card, as a single string, in "<rank> of <suit>"" formation
Local Variables: 
      cardString, to hold the card's string to be returned
      rankStr, to hold the card's rank's string value
      suitStr, to hold the card's suit's string value
Algorithm: 
      1) Check if rank and string have both been initialized for the card
      3) Get the card's rank and string and form corresponding shortform strings
      4) return the combined string
Assistance Received: none
********************************************************************* */

std::string Card::getShortCardStr() const {
   //check if suit or rank has not been initialized 
   if(!suitInitialized || !rankInitialized) {
      throw PinochleException("Suit or rank for this card have not been specified");
   } else {
      //cardString holds the string to be returned

      //here, both getRankString() and getSuitString() can return exceptions if either
      //one has not been initialized. Catch exception and pass it up
      std::string cardString;
      try {
         //getting string representation of rank and suit
         std::string rankStr, suitStr;
         //if Rank is not Ten or Nine, then rank is first string of getCardRank()
         rankStr = getRankString()[0];
         if(rank == Rank::Ten) {
            rankStr = "X";
         } else if(rank == Rank::Nine) {
            rankStr = "9";
         }

         suitStr = getSuitString()[0];

         cardString = rankStr + suitStr;
      } catch (PinochleException &e) {
         throw e;
      }
      return cardString;
   }
}


/* *********************************************************************
Function Name: setId
Purpose: a mutator function to set the id of the card
Parameters: 
      id, the id to be assigned to the card
Return Value: 
      returns true if mutation was successful, false if not
Assistance Received: none
********************************************************************* */
bool Card::setId(int id) {
   this->id = id;
   idInitialized = true;
   return true;
}


/* *********************************************************************
Function Name: setRank
Purpose: a mutator function to that set the rank of the card
Parameters: 
      rank, the rank to be assigned to the card
Return Value: 
      returns true if mutation was successful, false if not
Assistance Received: none
********************************************************************* */
bool Card::setRank(Rank rank) {
   if(static_cast<int>(rank) >= numOfRanks || static_cast<int>(rank) < 0) {
      throw PinochleException("invalid rank");
   }
   this->rank = rank;
   //mark rank as being initialized for the card
   rankInitialized = true;
   return true;
}

/* *********************************************************************
Function Name: setSuit
Purpose: a mutator function to that set the suit of the card
Parameters: 
      suit, the suit to be assigned to the card
Return Value: 
      returns true if mutation was successful, false if not
Assistance Received: none
********************************************************************* */
bool Card::setSuit(Suit suit) {
   if(static_cast<int>(suit) >= numOfSuits || static_cast<int>(suit) < 0) {
      throw PinochleException("invalid suit");
   }
   this->suit = suit;
   //mark suit as being initialized
   suitInitialized = true;
   return true;
}



/* *********************************************************************
Function Name: hasGreaterRankThan
Purpose: to know if a card has greater rank than another card
Parameters: 
      otherCard, the card that this card will be compared to
Return Value: 
      returns true if this card has greater rank, false otherwise
Assistance Received: none
********************************************************************* */
bool Card::hasGreaterRankThan(Card otherCard) {
   if(static_cast<int>(this->rank) > static_cast<int>(otherCard.getRank())) {
      return true;
   } else {
      return false;
   }
}

/* *********************************************************************
Function Name: hasLessRankThan
Purpose: to know if a card has less rank than another card
Parameters: 
      otherCard, the card that this card will be compared to
Return Value: 
      returns true if this card has less rank, false otherwise
Assistance Received: none
********************************************************************* */
bool Card::hasLessRankThan(Card otherCard) {
   if(static_cast<int>(this->rank) < static_cast<int>(otherCard.getRank())) {
      return true;
   } else {
      return false;
   }
}