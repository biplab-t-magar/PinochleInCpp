#pragma once
#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"

class Human : public Player{   
public:
   Human();
   Human(GroupOfCards hand, GroupOfCards capturePile, MeldsStorage meldsPlayed, Suit trumpSuit) : Player(hand, capturePile, meldsPlayed, trumpSuit) {}
   Card playLeadCard();
   Card playChaseCard(Card opponentCard);
   MeldInstance playMeld();
   void getHelpForLeadCard();
   void getHelpForChaseCard(Card opponentCard);
   void getHelpForMeld();
private:
   int promptUser();
   int promptCardThrow();
   // void saveGameProgress();
   std::vector<int> parseMeldPositions(std::string str);
   int parsePosition(std::string str);
};


#endif