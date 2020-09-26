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
   void startNewRound(int &roundNumber, int &hGameScore, int &cGameScore);
   void continueRound(int &roundNumber, int &hGameScore, int &cGameScore);
private:
   Player* players[numOfPlayers];
   int roundScores[numOfPlayers];
   int gameScores[numOfPlayers];
   Deck stock;
   Card trumpCard;
   Suit trumpSuit;
   bool humansTurn;
   int roundNumber;
   std::string loadedCaptureStrs[numOfPlayers];
   std::string loadedHandStrs[numOfPlayers];
   std::string loadedMeldStrs[numOfPlayers];
   std::string loadedStockStr;
   void beginRoundLoop();
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
   std::string saveGame();
   void promptSaveGame();
   void loadGameData();
   std::vector<std::string> getSaveFileContent();
};

#endif