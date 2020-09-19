#include "GroupOfCards.h"

GroupOfCards::GroupOfCards(std::vector<Card> cards) {
   this->cards = cards;
}



bool GroupOfCards::searchCardById(int id, Card* foundCard) const{
   foundCard = NULL;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         if(foundCard != NULL) {
            *foundCard = cards[i] ;
         }
         return true;
      }
   }
   return false;
}

bool GroupOfCards::searchCardsByRank(Rank rank, std::vector<Card>* foundCards) const{
   if(foundCards != NULL) {
      (*foundCards).clear();
   }
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getRank() == rank) {
         if(foundCards != NULL) {
            (*foundCards).push_back(cards[i]);
         }
         
         return true;
      }
   }
   if((*foundCards).size() > 0) {
      return true;
   } else {
      return false;
   }
}

bool GroupOfCards::searchCardsBySuit(Suit suit, std::vector<Card>* foundCards) const {
   if(foundCards != NULL) {
      (*foundCards).clear();
   }
   (*foundCards).clear();
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getSuit() == suit) {
          if(foundCards != NULL) {
            (*foundCards).push_back(cards[i]);
         }
      }
   }
   if((*foundCards).size() > 0) {
      return true;
   } else {
      return false;
   }
}


int GroupOfCards::getNumOfCards() const {
   return cards.size();
}

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

bool GroupOfCards::operator!=(const GroupOfCards& otherGroupOfCards) {
   return !((*this) == otherGroupOfCards);
}


