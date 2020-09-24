#pragma once
#ifndef ROUND_H
#define ROUND_H

#include "./Players/Human.h"
#include "./Players/Computer.h"
#include "Deck.h"

class Round {
public:
   Round();
   void startNewRound(int roundNumber, int &hGameScore, int &cGameScore);
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
   int promptUserForMeld(Player human);
   bool coinToss();
   int cardPoints(Card card);
   void findWinnerAndGivePoints(Card leadCard, Card chaseCard);
   bool leadCardWins(Card leadCard, Card chaseCard);
   void displayTable(int roundNumber, int hGameScore, int cGameScore);
   std::string getHandString(Player player);
   std::string getCaptureString(Player player);
   std::string getMeldsString(Player player);
};

#endif