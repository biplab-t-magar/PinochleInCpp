#include <Card.h>

/* *********************************************************************
Function Name: Card
Purpose: Default constructor for Card class that initializes a card object. Creates a Card with undefined rank and suit
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
      1) Mark card as have undefined rank and suit
Assistance Received: None
********************************************************************* */
Card::Card() {
   rankInitialized = false;
   suitInitialized = false;
}

/* *********************************************************************
Function Name: Card
Purpose: Overloaded constructor for Card class that initializes a card object. 
      Creates a Card with a rank and suit based on the values sent into the parameter
Parameters: 
      rank, the rank to be assigned to the card
      suit, the suit to be assigned to the card
Return Value: 
Local Variables: 
Algorithm: 
      1) Assign card's rank to what was defined in the function argument
      2) Assign card's suit to what was defined at the function argument
      3) Mard card as having defined suit and rank
Assistance Received: none
********************************************************************* */
Card::Card(int id, Rank rank, Suit suit) {
   this->id = id;
   this->rank = rank;
   this->suit = suit;
   idInitialized = true;
   rankInitialized = true;
   suitInitialized = true;
}

int Card::getId() const {
   if(!idInitialized) {
      throw PinochleException("Id for this card has not been specified");
   } 

   return id;
    
}

bool Card::setId(int id) {
   this->id = id;
   idInitialized = true;
}



/* *********************************************************************
Function Name: getRank
Purpose: a selector function to that returns the rank of the card
Parameters: 
Return Value: 
      the rank of the card, of type Rank
Local Variables: 
Algorithm: 
      1) Check if rank has been defined for the card
      2) if rank has been defined: return the rank
      3) else: throw an exception
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
Local Variables: 
Algorithm: 
      1) Check if suit has been defined for the card
      2) if suit has been defined: return the suit
      3) else: throw an exception
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
Local Variables: 
Algorithm: 
      1) Check if rank has been initialized for the card
      2) if yes, send a string corresponding to the rank
      3) else: throw an exception
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
Local Variables: 
Algorithm: 
      1) Check if suit has been initialized for the card
      2) if yes, return a string corresponding to the suit
      3) else: throw an exception
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
Algorithm: 
      1) Check if rank and string have both been initialized for the card
      2) if not, throw exception
      3) if yes, get the card's rank and string and combine to a string
      4) return the combined string
Assistance Received: none
********************************************************************* */
std::string Card::getCardString() const {

   //check if both suit and rank have not been initialized 
   if(!suitInitialized && !rankInitialized) {
      throw PinochleException("Suit and rank for this card have not been specified");
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
Function Name: setRank
Purpose: a mutator function to that set the rank of the card
Parameters: 
      rank, the rank to be assigned to the card
Return Value: 
      returns true if mutation was successful, false if not
Local Variables: 
Algorithm: 
      1) assign rank of card to the value sent as argument
      2) mark card's rank as being initialized
      3) return true
Assistance Received: none
********************************************************************* */
bool Card::setRank(Rank rank) {
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
Local Variables: 
Algorithm: 
      1) assign suit of card to the value sent as argument
      2) mark card's suit as being initialized
      3) return true
Assistance Received: none
********************************************************************* */
bool Card::setSuit(Suit suit) {
   this->suit = suit;
   //mark suit as being initialized
   suitInitialized = true;
   return true;åå
}