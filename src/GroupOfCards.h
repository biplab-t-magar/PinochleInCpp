#pragma once
#ifndef GROUP_OF_CARD_H
#define GROUP_OF_CARD_H

#include "Card.h"
#include <vector>


class GroupOfCards {
public: 
   GroupOfCards();
   GroupOfCards(std::vector<Card> cards);

   Card getCardById(int id) const;
   std::vector<Card> getCardsByRank(Rank rank) const;
   std::vector<Card> getCardsBySuit(Suit suit) const;
   std::vector<Card> getCardsByRankAndSuit(Rank rank, Suit suit) const;
   Card getCardByPosition(int position) const;
   int getCardPosition(Card card);
   int getNumOfCards() const;

   bool addCard(Card card);
   bool removeCardById(int id); 
   bool removeCardByPosition(int position);
   bool searchCardById(int id) const;

   bool operator==(const GroupOfCards& otherGroupOfCards);
   bool operator!=(const GroupOfCards& otherGroupOfCards);
protected: 
   std::vector<Card> cards;
};

#endif