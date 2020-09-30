#include "MeldInstance.h"

/* *********************************************************************
Function Name: MeldInstance
Purpose: Default constructor for the MeldInstance class
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
MeldInstance::MeldInstance() {
   meldIsValid = false;
}

/* *********************************************************************
Function Name: MeldInstance
Purpose: Constructor for the MeldInstance clas
Parameters: 
      cards, the vector of all cards that are part of the MeldInstance object
      trumpSuit, the trumpSuit of the current round
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
MeldInstance::MeldInstance(std::vector<Card> cards, Suit trumpSuit) : GroupOfCards(cards) {
   this->trumpSuit = trumpSuit;
   meldIsValid = checkMeldValidity();
}

/* *********************************************************************
Function Name: addCard
Purpose: adds a card to the meld instance and checks the validity of the new meld formed
Parameters: 
      card, the card to be added
      trumpSuit, the trump suit of the round
Return Value: 
      true when card is added successfully
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::addCard(Card card, Suit trumpSuit) {
   cards.push_back(card);
   this->trumpSuit = trumpSuit;
   meldIsValid = checkMeldValidity();
   return true;
}

/* *********************************************************************
Function Name: removeCardById
Purpose: removes a card of the given id from this meld instance
Parameters: 
      id, the id of the card to be removed
Return Value: 
      true if the card was found and removed, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::removeCardById(int id) {
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         cards.erase(cards.begin() + i);
         meldIsValid = checkMeldValidity();
         return true;
      }
   }
   
   return false;
}


/* *********************************************************************
Function Name: removeCardByPosition
Purpose: removes the card in the given position 
Parameters: 
      position, the position of the card to be removed
Return Value: 
      true when the card is removed successfully, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::removeCardByPosition(int position) {
   if(position >= cards.size() || position < 0) {
      throw PinochleException("Given position is out of bounds");
   }
   if(getNumOfCards()  < 1) {
      return false;
   }
   cards.erase(cards.begin() + position);
   meldIsValid = checkMeldValidity();
   return true;
}


/* *********************************************************************
Function Name: removeAllCards
Purpose: 
      empties the meld intance object of all cards
Parameters: 
Return Value: 
      returns true when successfully emptied
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::removeAllCards() {
   meldIsValid = false;
   cards.clear();
   return true;
}

/* *********************************************************************
Function Name: getMeldType
Purpose: returns the Meld type of this meld instance
Parameters: 
Return Value: 
      the meld type of this current instance, of enum type Meld
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
Meld MeldInstance::getMeldType() const {
   if(meldIsValid == false) {
      throw PinochleException("This is not a valid meld");
   }
   return meldType;
}

/* *********************************************************************
Function Name: getMeldTypeString
Purpose: to get the string representation of the meld formed by this meldinstance
Parameters: 
Return Value: 
      the string representation of the meld type
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
std::string MeldInstance::getMeldTypeString() const {
   if(!meldIsValid) {
      throw PinochleException("This is not a valid meld");
   }

   //Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
   switch (meldType)
   {
   case Meld::Flush:
      return "Flush";
      break;
   case Meld::RoyalMarriage:
      return "Royal Marriage";
      break;
   case Meld::Marriage:
      return "Marriage";
      break;
   case Meld::Dix:
      return "Dix";
      break;
   case Meld::FourAces:
      return "Four Aces";
      break;
   case Meld::FourKings:
      return "Four Kings";
      break;
   case Meld::FourQueens:
      return "Four Queens";
      break;
   case Meld::FourJacks:
      return "Four Jacks";
      break;
   case Meld::Pinochle:
      return "Pinochle";
      break;
   default:
      break;
   }
}

/* *********************************************************************
Function Name: getMeldPoints
Purpose: returns the points corresponding to this meld instance
Parameters: 
Return Value: 
      the points that playig this meld instance would yield, of type int
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
int MeldInstance::getMeldPoints() const {
   if(!meldIsValid) {
      throw PinochleException("This is not a valid meld");
   }
   switch(meldType) {
      case Meld::Flush:
         return 150;
         break;
      case Meld::RoyalMarriage:
         return 40;
         break;
      case Meld::Marriage:
         return 20;
         break;
      case Meld::Dix:
         return 10;
         break;
      case Meld::FourAces:
         return 100;
         break;
      case Meld::FourKings:
         return 80;
         break;
      case Meld::FourQueens:
         return 60;
         break;
      case Meld::FourJacks:
         return 40;
         break;
      case Meld::Pinochle:
         return 40;
         break;
      default:
         return 0;
         break;
   }
}

/* *********************************************************************
Function Name: isValidMeld
Purpose: checks if this meld instance forms a valid meld or not
Parameters: 
Return Value: 
      true if the meld intance forms a valid meld, false if not
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::isValidMeld() const {
   return meldIsValid;
}

/* *********************************************************************
Function Name: checkMeldValidity
Purpose: determines, given all the cards stored in the object, if a valid meld is formed by them
Parameters: 
Return Value: 
      true if valid meld is formed by the cards, false otherwise
Local Variables: 
Algorithm: 
      1) check one by one if the cards represent any of meld type
      2)return true if one of the melds is represented
Assistance Received: None
********************************************************************* */
bool MeldInstance::checkMeldValidity() {
   //if the number of cards does not correspond to any possible meld, return false
   if(cards.size() < 1 || cards.size() == 3 || cards.size() > 5) {
      return false;
   }

   //checking melds from most common to least common
   //check if Dix
   if(isDix()) {
      meldType = Meld::Dix;
      return true;
   }

   //check if Marriage
   if(isAnyMarriage()) {
      meldType = typeOfMarriage();
      return true;
   }

   //check if Pinochle
   if(isPinochle()) {
      meldType = Meld::Pinochle;
      return true;
   }

   //check if Four Aces
   if(cards[0].getRank() == Rank::Ace && isFours()) {
      meldType = Meld::FourAces;
      return true;
   }

   //check if Four Kings
   if(cards[0].getRank() == Rank::King && isFours()) {
      meldType = Meld::FourKings;
      return true;
   }

   //check if Four Queens
   if(cards[0].getRank() == Rank::Queen && isFours()) {
      meldType = Meld::FourQueens;
      return true;
   }

   //check if Four Jacks
   if(cards[0].getRank() == Rank::Jack && isFours()) {
      meldType = Meld::FourJacks;
      return true;
   }

   //check if Flush
   if(isFlush()) {
      meldType = Meld::Flush;
      return true;
   }

   //if none of the Melds match
   return false;

}


/* *********************************************************************
Function Name: isDix
Purpose: determines whether the cards stored form a Dix meld
Parameters: 
Return Value: 
      true if the cards form a Dix, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::isDix() {
   //Note: A Dix is a card of rank Nine and a suit the same as the trump suit

   //check size
   if(cards.size() != 1) {
      return false;
   }

   //if card is not of rank Nine or not of trump suit, return false
   if(cards[0].getRank() != Rank::Nine || cards[0].getSuit() != trumpSuit) {
      return false;
   }
   return true;
}

/* *********************************************************************
Function Name: isAnyMarriage
Purpose: determines whether the cards stored form any type of marriage meld (royal or otherwise)
Parameters: 
Return Value: 
      true if the cards form any type of marriage, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::isAnyMarriage() {
   //check size of meld
   if(cards.size() != 2) {
      return false;
   }

   //If cards are not of the same suit, return false
   if(cards[0].getSuit() != cards[1].getSuit()) {
      return false;
   }

   //if one card is a King and the other a Queen, return true
   if(cards[0].getRank() == Rank::King && cards[1].getRank() == Rank::Queen) {
      return true;
   }
   if(cards[1].getRank() == Rank::King && cards[0].getRank() == Rank::Queen) {
      return true;
   }

   //if none of the conditions match return false
   return false;
}

/* *********************************************************************
Function Name: typeOfMarriage
Purpose: determines what type of marriage meld the cards stored make
Parameters: 
Return Value: 
      the type of marriage meld that the cards comprise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
Meld MeldInstance::typeOfMarriage() {
   //Note: A Marriage contains a King and Queen of any other suit besides the Trump suit

   if(!isAnyMarriage()) {
      throw PinochleException("Cards must be a type of marriage");
   }
   
   //If first card is trump suit, return false (only first card is checked because both cards are the same suit)
   //ensured by isAnyMarriage
   if(cards[0].getSuit() == trumpSuit) {
      return Meld::RoyalMarriage;
   } else {
      return Meld::Marriage;
   }

}

/* *********************************************************************
Function Name: isPinochle
Purpose: determines whether the cards stored form a Pinochle meld
Parameters: 
Return Value: 
      true if the cards form a Pinochle, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::isPinochle() {
   //Note: A flush contains Queen of Spades and Jack of Diamonds 
   
   //check size
   if(cards.size() != 2) {
      return false;
   } 

   //if one card is queen of spades and the other is jack of diamonds, return true
   if(cards[0].getRank() == Rank::Queen && cards[0].getSuit() == Suit::Spades) {
      if(cards[1].getRank() == Rank::Jack && cards[1].getSuit() == Suit::Diamonds) {
         return true;
      }
   }
   //checking for vice versa 
   if(cards[1].getRank() == Rank::Queen && cards[1].getSuit() == Suit::Spades) {
      if(cards[0].getRank() == Rank::Jack && cards[0].getSuit() == Suit::Diamonds) {
         return true;
      }
   }
   return false;
}


/* *********************************************************************
Function Name: isFours
Purpose: determines whether the cards stored forms any one of the Fours melds (Four Kings, Four Aces, Four Queens, or Four Jacks)
Parameters: 
Return Value: 
      true if the cards form a Fours meld, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::isFours() {
   //Note: Fours are types of melds consisting of Four melds of the same Rank but of different suits
   //These melds are: Four Aces, Four Kings, Four Queens, and Four Jacks
   //This general function checks only checks whether it is a Fours type of meld, 
   //it does not care what the specific meld is
   //check card size
   if(cards.size() != 4) {
      return false;
   }
   //we are comparing the ranks of the remaining three card with that of the first card
   Rank whatRank = cards[0].getRank();
   
   //this array of flags is used to keep track of the different suits encountered 
   //as we go through the vector of cards
   //it tells us if any suit has been repeated, in which case, it is not a Fours type meld 
   //and so, we can return false
   //flag[0] corresponds to Clubs, flag[1] to Diamonds, flag[2] to Hearts, and flag[3] to Spades (based
   //on the order of enums as listed in Card.h)
   //all flags set fo false initially
   bool flags[4] = {false, false, false, false};

   //loop once for each card
   for(int i = 0; i < 4; i++) {
      //if rank doesn't match in a card, return false
      if(cards[i].getRank() != whatRank) {
         return false;
      }
      //if the suit has already been encountered before, return false
      if(flags[static_cast<int>(cards[i].getSuit())] == true) {
         return false;
      } 
      //switch flag
      flags[static_cast<int>(cards[i].getSuit())] = !flags[static_cast<int>(cards[i].getSuit())];
   }
   //if all the cards are of the same rank and all their suits are unique, it is a Fours meld
   return true;

}

/* *********************************************************************
Function Name: isFlush
Purpose: determines whether the cards stored form a Flush meld
Parameters: 
Return Value: 
      true if the cards form a Flush, false otherwise
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool MeldInstance::isFlush() {
   //checking if the meld is a Flush
   //Note: A flush contains five cards: Ace, Ten, King, Queen, and Jack, all of Trump suit
   
   //check size of meld
   if(cards.size() != 5) {
      return false;
   }
   //This array of flags will keep track of the cards encountered in this 
   //group of cards. If a card of trump suit is encountered, corresponding flag is switched.
   //flag[0] corresponds to Nine, flag[1] to Joker, flag[2] to Queen, and 
   //so on in ascending order based on the order of enums as listed in Card.h
   //All flags except flag[0] set to false initially, because Nine (represented by flag[0]) is not part of the Flush meld
   //If a card of non-trump suit is encountered, directly return false
   
   bool flags[6] = {true, false, false, false, false, false};
   
   //loop once for each card
   for(int i = 0; i < 5; i++) {
      //if this card is not of the trump suit
      if(cards[i].getSuit() != trumpSuit) {
         return false;
      }
      //if the card has already been encountered before in this group of cards (or if this card is a Nine card)
      //then return false
      if (flags[static_cast<int>(cards[i].getRank())] == true) {
         return false;
      }

      //if this card is of the trump suit, hasn't been encountered before, and if it is not a Nine card, 
      //then switch flag
      flags[static_cast<int>(cards[i].getRank())] = !flags[static_cast<int>(cards[i].getRank())];
   }
   //if all the cards are unique, all are of trump suit, and none of them are of rank Nine, then  
   //the cards make up a Flush meld. 
   return true;
}