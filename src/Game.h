#pragma once
#ifndef GAME_H
#define GAME_H

#include "Round.h"

class Game {
public:
   Game();
   void startGame();
private:
   int humanScore;
   int computerScore;
   int currentRound;
};

#endif