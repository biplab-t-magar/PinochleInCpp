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
   virtual void nextMove() = 0;
   void takeOneCard(Card card);
   bool setTrumpSuit(Suit trumpSuit);
   virtual Card playLeadCard() = 0;
   virtual Card playChaseCard() = 0;
   int numCardsInHand();
protected:
   Card suggestLeadCard(std::string &reasoning);
   Card suggestChaseCard(std::string &reasoning);
   MeldInstance suggestNextMeld(std::string &reasoning);
   GroupOfCards hand;
   GroupOfCards capurePile;
private:
   Suit trumpSuit; 
   MeldServices meldServices;
   std::vector<std::string> handStrRep;
   std::vector<std::string> meldStrRep;
   std::vector<std::string> capturePileStrRep;
   Card playFromHand();
   Card playCard(int position);
   void playMeld(MeldInstance meldInstance);
   bool cardsArePresentInHand(std::vector<Card> cards);
};

#endif