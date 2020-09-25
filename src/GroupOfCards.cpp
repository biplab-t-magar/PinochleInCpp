#include "GroupOfCards.h"

GroupOfCards::GroupOfCards() {

}

GroupOfCards::GroupOfCards(std::vector<Card> cards) {
   this->cards = cards;
}

bool GroupOfCards::addCard(Card card) {
   cards.push_back(card);
   return true;
}


bool GroupOfCards::removeCardById(int id) {
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         cards.erase(cards.begin() + i);
      }
   }
   return true;
}
bool GroupOfCards::removeCardByPosition(int position) {
   if(position >= cards.size()) {
      throw PinochleException("Given position is out of bounds");
   }

   cards.erase(cards.begin() + position);
   return true;
}


bool GroupOfCards::searchCardById(int id) const{
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getId() == id) {
         return true;
      }
   }
   return false;
}

// bool GroupOfCards::searchCardByRankAndSuit(Rank rank, Suit suit) {
//     for(int i = 0; i < cards.size(); i++) {
//       if(cards[i].getRank() == rank && cards[i].getSuit() == suit) {
//          return true;
//       }
//    }
//    return false;
// }


// bool GroupOfCards::searchCard(Card card) {
//    for(int i = 0; i < cards.size(); i++) {
//       if(cards[i] == card) {
//          return true;
//       }
//    }
//    return false;
// }

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

std::vector<Card> GroupOfCards::getCardsByRank(Rank rank) const{
   std::vector<Card> foundCards;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getRank() == rank) {
         foundCards.push_back(cards[i]);
      }
   }
   return foundCards;
}

std::vector<Card> GroupOfCards::getCardsBySuit(Suit suit) const {
   std::vector<Card> foundCards;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getSuit() == suit) {
         foundCards.push_back(cards[i]);
      }
   }
   return foundCards;
}

std::vector<Card> GroupOfCards::getCardsByRankAndSuit(Rank rank, Suit suit) const {
   std::vector<Card> foundCards;
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i].getRank() == rank && cards[i].getSuit() == suit) {
         foundCards.push_back(cards[i]);
      }
   }
   return foundCards;
}


Card GroupOfCards::getCardByPosition(int position) const {
   if(position >= cards.size() || position < 0) {
      throw PinochleException("Position is out of range, given the number of cards");
   }
   return cards[position];
}

int GroupOfCards::getCardPosition(Card card) {
   for(int i = 0; i < cards.size(); i++) {
      if(cards[i] == card) {
         return i;
      }
   }
   //if card not found, return -1
   return -1;
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



