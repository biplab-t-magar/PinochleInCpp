/* Card.h
   Card.h contains the Card class and enums used to list hte different ranks
   and suits a card can have.
   The Card class represents a "card" in a game of Pinochle. A card is identified
   by its rank and suit. It can have Nine, Jack, Queen, King, Ten, or Ace as its
   rank, and Clubs, Diamonds, Hearts, or Spades as its suit.
*/

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
   bool setId(int id);
   Rank getRank() const;
   Suit getSuit() const;
   std::string getRankString() const;
   std::string getSuitString() const;
   std::string getCardString() const;
   bool setRank(Rank rank);
   bool setSuit(Suit suit);
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