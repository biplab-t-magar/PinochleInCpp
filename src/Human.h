#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "Card.h"
#include "Player.h"

class Human : Player{   
public:
   Human();
   void nextMove();
   Card playLeadCard();
   Card playChaseCard(Card card);
private:
   int promptUser();
   void saveGameProgress();
   void helpForNextMove();
   int promptCardThrow();
   std::string removeWhiteSpace(std::string str);
};


#endif