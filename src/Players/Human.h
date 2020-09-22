#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "Card.h"
#include "Player.h"

class Human : public Player{   
public:
   Human();
   Card playLeadCard();
   Card playChaseCard(Card opponentCard);
   void playMeld();
   std::string getHelpForLeadCard();
   std::string getHelpForChaseCard(Card opponentCard);
   std::string getHelpForMeld();
private:
   int promptUser();
   int promptCardThrow();
   // void saveGameProgress();
   std::string Human::stripString(std::string str);
   std::string removeWhiteSpace(std::string str);
   std::vector<int> parseMeldPositions(std::string str);
};


#endif