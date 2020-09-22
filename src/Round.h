#pragma once
#ifndef ROUND_H
#define ROUND_H

#include "Human.h"
#include "Computer.h"

class Round {
public:
   Round();
   void startRound(int roundNumber, int hScore, int cScore);
private:
   Human human;
   Computer computer;
};

#endif