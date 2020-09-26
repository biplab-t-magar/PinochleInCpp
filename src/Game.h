#pragma once
#ifndef GAME_H
#define GAME_H

#include "Round.h"
#define numOfPlayers 2

class Game {
public:
   Game();
   void start();
private:
   int currentRound;
   int gameScores[numOfPlayers];
   int newGameOrLoad();
   void startNewGame();
   void loadGame();
   void startAnotherRound();
   bool playAnotherRound();
   Deck createStockPileObject();
};

#endif