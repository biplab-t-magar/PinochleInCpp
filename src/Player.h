#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <Card.h>
#include <MeldServices.h>
#include <string>

class Player {
public:
   Player();
   void nextMove();
   void takeOneCard(Card card);
   bool setTrumpSuit(Suit trumpSuit);
private:
   GroupOfCards hand;
   GroupOfCards capurePile;
   Suit trumpSuit; 
   MeldServices meldServices;
   std::vector<std::string> handStrRep;
   std::vector<std::string> meldStrRep;
   std::vector<std::string> capturePileStrRep;
   Card playFromHand();
   Card playCard(int position);
   void playMeld(MeldInstance meldInstance);
   void suggestNextMeld();
   void suggestNextThrow();
   bool cardsArePresentInHand(std::vector<Card> cards);
};

#endif