#pragma once
#ifndef GAME_H
#define GAME_H

#include "Round.h"
#define numOfPlayers 2

class Game {
public:
   Game();
private:
   int currentRound;
   int gameScores[numOfPlayers];
   int loadedRoundScores[numOfPlayers];
   std::string loadedHandStrs[numOfPlayers];
   std::string loadedCaptureStrs[numOfPlayers];
   std::string loadedMeldStrs[numOfPlayers];
   std::string loadedStockStr;
   Card loadedTrumpCard;
   std::string loadedNextPlayerStr;
   Serialization serializations[2];
   int newGameOrLoad();
   void startNewGame();
   void loadGame();
   void startAnotherRound();
   bool playAnotherRound();
   void loadGameData();
   std::vector<std::string> getSaveFileContent();
};

#endif