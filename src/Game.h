#pragma once
#ifndef GAME_H
#define GAME_H

#include "Round.h"

class Game {
public:
   Game();
private:
   int humanScore;
   int computerScore;
   int currentRound;
   int newGameOrLoad();
   void startNewGame();
   void loadGame();
   void startAnotherRound();
   bool playAnotherRound();
};

#endif