#pragma once
#ifndef GROUP_OF_CARD_H
#define GROUP_OF_CARD_H

#include "Card.h"
#include <vector>


class GroupOfCards {
public: 
   GroupOfCards(std::vector<Card> cards);
   bool searchCardById(int id, Card* foundCard = NULL) const;
   bool searchCardsByRank(Rank rank, std::vector<Card>* foundCards = NULL) const;
   bool searchCardsBySuit(Suit suit, std::vector<Card>* foundCards = NULL) const;
   int getNumOfCards() const;
   bool operator==(const GroupOfCards& otherGroupOfCards);
   bool operator!=(const GroupOfCards& otherGroupOfCards);
protected: 
   std::vector<Card> cards;
};

#endif