#pragma once
#ifndef ROUND_H
#define ROUND_H

#include "Human.h"
#include "Computer.h"
#include "Deck.h"
#include "Serialization.h"

#define numOfPlayers 2

class Round {
public:
   Round();
   ~Round();
   void startNewRound(int roundNumber, int &hGameScore, int &cGameScore);
   // void continueRound(Serialization )
private:
   Player* players[numOfPlayers];
   int roundScores[numOfPlayers];
   int gameScores[numOfPlayers];
   Serialization serializations[numOfPlayers];
   Deck stock;
   Card trumpCard;
   Suit trumpSuit;
   bool humansTurn;
   int roundNumber;
   int promptUser();
   int promptUserForMeld(Player* human);
   bool coinToss();
   int cardPoints(Card card);
   void findWinnerAndGivePoints(Card leadCard, Card chaseCard);
   bool leadCardWins(Card leadCard, Card chaseCard);
   void displayTable();
   std::string getHandString(Player* player);
   std::string getCaptureString(Player* player);
   std::string getMeldsString(Player* player);
   void saveGame();
   void promptSaveGame();
};

#endif