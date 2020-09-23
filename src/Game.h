#pragma once
#ifndef GAME_H
#define GAME_H

#include "Round.h"

class Game {
public:
   Game();
   int newGameOrLoad();
   void startNewGame();
   void loadGame();
   void Game::startAnotherRound();
private:
   int humanScore;
   int computerScore;
   int currentRound;
};

#endif