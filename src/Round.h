#pragma once
#ifndef ROUND_H
#define ROUND_H

#include "Human.h"
#include "Computer.h"
#include "Deck.h"

class Round {
public:
   Round();
   void startNewRound(int roundNumber, int &hGameScore, int &cRoundScore);
   // void continueRound(Serialization )
private:
   Player players[2];
   Deck stock;
   int hRoundScore;
   int cRoundScore;
   Card trumpCard;
   Suit trumpSuit;
   bool humansTurn;
   int promptUser();
   int prompUserForMeld(Player human);
   void findWinnerAndGivePoints(Card leadCard, Card chaseCard);
   bool leadCardWins(Card leadCard, Card chaseCard);
};

#endif