#pragma once
#ifndef CARD_H
#define CARD_H

#include <string>
#include <PinochleException.h>

enum class Rank {
   Nine, Jack, Queen, King, Ten, Ace
};

enum class Suit {
   Clubs, Diamonds, Hearts, Spades
};

class Card {
public:
   Card();
   Card(Rank rank, Suit suit);
   Rank getRank() const;
   Suit getSuit() const;
   std::string getRankString() const;
   std::string getSuitString() const;
   std::string getCardString() const;
   bool setRank(Rank rank);
   bool setSuit(Suit suit);
private:
   Rank rank;
   Suit suit;
   bool rankInitialized;
   bool suitInitialized;
};

#endif