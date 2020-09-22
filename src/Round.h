#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "Human.h"
#include "Computer.h"

class Round {
public:
   Round();
   void startRound(int roundNumber, int hGameScore, int cGameScore, int hRoundScore, int cRoundScore);
private:
   Human human;
   Computer computer;

};

#endif