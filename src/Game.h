/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
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
};

#endif