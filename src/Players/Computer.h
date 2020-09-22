#pragma once
#ifndef COMPUTER_H
#define COMPUTER_H

#include "Player.h"

class Computer : public Player {
public:
   Computer();
   Card playLeadCard();
   Card playChaseCard(Card opponentCard);
   void playMeld();
private:

};

#endif