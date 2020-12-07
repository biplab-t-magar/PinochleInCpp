/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/

#pragma once
#ifndef CARD_H
#define CARD_H

#include <string>
#include "PinochleException.h"

enum class Rank {
   Nine, Jack, Queen, King, Ten, Ace
};

enum class Suit {
   Clubs, Diamonds, Hearts, Spades
};

class Card {
public:
   Card();
   Card(int id, Rank rank, Suit suit);

   int getId() const;
   Rank getRank() const;
   Suit getSuit() const;
   std::string getRankString() const;
   std::string getSuitString() const;
   std::string getCardString() const;
   std::string getShortCardStr() const;
   
   bool setId(int id);
   bool setRank(Rank rank);
   bool setSuit(Suit suit);

   bool hasGreaterRankThan(Card otherCard);
   bool hasLessRankThan(Card otherCard);
   inline bool operator==(const Card& otherCard) {
      return (this->id == otherCard.getId() && this->rank == otherCard.getRank() && this->suit == otherCard.getSuit());
   }
   inline bool operator!=(const Card& otherCard) {
      return (this->id != otherCard.getId() || this->rank != otherCard.getRank() || this->suit != otherCard.getSuit());
   }
   inline bool compareRankAndSuit(const Card& otherCard) {
      return (this->rank == otherCard.getRank() && this->suit == otherCard.getSuit());
   }
private:
   Rank rank;
   Suit suit;
   int id;
   bool rankInitialized;
   bool suitInitialized;
   bool idInitialized;
};

#endif