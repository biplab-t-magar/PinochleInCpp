/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#include "GroupOfCards.h"

/* *********************************************************************
Function Name: GroupOfCards
Purpose: Constructor for GroupOfCards class
Parameters: 
Return Value: 
Assistance Received: None
********************************************************************* */
GroupOfCards::GroupOfCards() {}

/* *********************************************************************
Function Name: GroupOfCards
Purpose: Constructor that initializes all the cards in the GroupOfCards object
Parameters: 
      cards, the vector of cards to be stored
Return Value: 
Assistance Received: None
********************************************************************* */
GroupOfCards::GroupOfCards(std::vector<Card> cards) {
   this->cards = cards;
}

/* *********************************************************************
Function Name: addCard
Purpose: To add a card to the group of cards 
Parameters: 
      card, the card to be added, of Card type
Return Value: 
      bool, true when card was added succesfully
Assistance Received: None
********************************************************************* */
bool GroupOfCards::addCard(Card card) {
   cards.push_back(card);
   return true;
}

/* *********************************************************************
Function Name: removeCardById
Purpose: removes a card of the given id from the group of cards 
Parameters: 
      id, the id of the card to be removed, of type id
Return Value: 
      return true when card is removed successfully
Assistance Received: None
********************************************************************* */
bool GroupOfCards::removeCardById(int id) {
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         cards.erase(cards.begin() + i);
      }
   }
   return true;
}

/* *********************************************************************
Function Name: removeCardByPosition
Purpose: removes a card from the given position in the group of cards hand
Parameters: 
      position, the position of the card in the group of cards, of int type
Return Value: 
      true when card is successfully removed
Assistance Received: None
********************************************************************* */
bool GroupOfCards::removeCardByPosition(int position) {
   if(position >= cards.size()) {
      throw PinochleException("Given position is out of bounds");
   }

   cards.erase(cards.begin() + position);
   return true;
}


/* *********************************************************************
Function Name: searchCardById
Purpose: checks if a card is present within the group of cards
Parameters: 
      id, the id of the card to be checked
Return Value: 
      true if card is found, false otherwise
Assistance Received: None
********************************************************************* */
bool GroupOfCards::searchCardById(int id) const{
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         return true;
      }
   }
   return false;
}

/* *********************************************************************
Function Name: getCardById
Purpose: To get the card in the group of cards when the id of the card is specified
Parameters: 
      id, the id of the card to be returned
Return Value: 
      the card whose id has been passed as argument
Assistance Received: None
********************************************************************* */
Card GroupOfCards::getCardById(int id) const {
   if(searchCardById(id) == false) {
      throw PinochleException("Card with given id is not in this group of cards. Make sure the card is in this group by calling searchCardById()");
   }
   Card card;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         card = cards[i];
      }
   }
   return card;
}

/* *********************************************************************
Function Name: getCardsByRank
Purpose: returns all the cards in the group of cards of the given rank
Parameters: 
      rank, the rank of the cards to be returned
Return Value: 
      vector of all the cards with the given rank
Assistance Received: None
********************************************************************* */
std::vector<Card> GroupOfCards::getCardsByRank(Rank rank) const{
   std::vector<Card> foundCards;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getRank() == rank) {
         foundCards.push_back(cards[i]);
      }
   }
   return foundCards;
}

/* *********************************************************************
Function Name: getCardsBySuit
Purpose: returns all the cards of the given from the group of cards 
Parameters: 
      suit, the suit of the cards to be returned
Return Value: 
      vector of all the cards with the given suit
Assistance Received: None
********************************************************************* */
std::vector<Card> GroupOfCards::getCardsBySuit(Suit suit) const {
   std::vector<Card> foundCards;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getSuit() == suit) {
         foundCards.push_back(cards[i]);
      }
   }
   return foundCards;
}


/* *********************************************************************
Function Name: getCardsByRankAndSuit
Purpose: gets all the cards with the given rank and suit that are present in this group of cards
Parameters: 
      rank, the rank of the cards to be returned
      suit, the suit of the cards to be returned
Return Value: 
      vector of cards of the given rank and suit that are found in this group of cards
Assistance Received: None
********************************************************************* */
std::vector<Card> GroupOfCards::getCardsByRankAndSuit(Rank rank, Suit suit) const {

   std::vector<Card> foundCards;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getRank() == rank && cards[i].getSuit() == suit) {
         foundCards.push_back(cards[i]);
      }
   }
   return foundCards;
}

/* *********************************************************************
Function Name: getCardByPosition
Purpose: returns the card that is present in the given position
Parameters: 
      position, the position of the needed card in this group of cards
Return Value: 
      the card in the given position
Assistance Received: None
********************************************************************* */
Card GroupOfCards::getCardByPosition(int position) const {
   if(position >= cards.size() || position < 0) {
      throw PinochleException("Position is out of range, given the number of cards");
   }
   return cards[position];
}

/* *********************************************************************
Function Name: getCardByPosition
Purpose: Gets the position of the given card in this group of cards 
Parameters: 
      card, the card whose position is to be returned
Return Value: 
      the position of the given card, of type int
Assistance Received: None
********************************************************************* */
int GroupOfCards::getCardPosition(Card card){
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i] == card) {
         return i;
      }
   }
   //if card not found, return -1
   return -1;
}

/* *********************************************************************
Function Name: getNumOfCards
Purpose: to get the number of cards in this Group of Cards
Parameters: 
Return Value: 
      the number of cards contained in this group of cards
Assistance Received: None
********************************************************************* */
int GroupOfCards::getNumOfCards() const {
   return cards.size();
}

/* *********************************************************************
Function Name: operator==
Purpose: function for overloading == operator
Parameters: another GroupOfCards object
Return Value: 
      true if both the Group of Cards have identical cards, false otherwise
Assistance Received: None
********************************************************************* */
bool GroupOfCards::operator==(const GroupOfCards& otherGroupOfCards) {
   if(getNumOfCards() != otherGroupOfCards.getNumOfCards()) {
      return false;
   }
   for(int i = 0; i < getNumOfCards(); i++) {
      if(otherGroupOfCards.searchCardById(this->cards[i].getId()) == false) {
         return false;
      }
   }
   return true;

}

/* *********************************************************************
Function Name: operator!=
Purpose: function for overloading != operator
Parameters: another GroupOfCards object
Return Value: 
      true if any of the cards do not match between the two GroupOfCards
Assistance Received: None
********************************************************************* */
bool GroupOfCards::operator!=(const GroupOfCards& otherGroupOfCards) {
   return !((*this) == otherGroupOfCards);
}



