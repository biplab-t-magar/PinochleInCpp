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
   // virtual void nextMove() = 0;
   void takeOneCard(Card card);
   bool setTrumpSuit(Suit trumpSuit);
   virtual Card playLeadCard() = 0;
   virtual Card playChaseCard(Card opponentCard) = 0;
   virtual void playMeld() = 0;
   int numCardsInHand();
   bool isMeldPossible();
protected:
   Card suggestLeadCard(std::string &reasoning);
   Card suggestChaseCard(std::string &reasoning, Card opponentCard);
   MeldInstance suggestNextMeld(std::string &reasoning);
   Card playFromHand(int position);
   Card playFromHand(Card card);

   //throw exception if not valid meld
   MeldInstance createMeld(std::vector<int> positions);

   //throws exception if not valid meld
   void createMeld(MeldInstance meldInstance);
private:
   GroupOfCards hand;
   GroupOfCards capturePile;
   Suit trumpSuit; 
   MeldServices meldServices;
   std::vector<Card> Player::bestCardsForLeadThrow();
   Card getLeastRankedCard();
   Card getLeastRankedFrom(std::vector<Card> cards);
   MeldInstance findBestMeldToPlay(std::vector<MeldInstance> meldsToCompare);
   // std::vector<std::string> handStrRep;
   // std::vector<std::string> meldStrRep;
   // std::vector<std::string> capturePileStrRep;
   
   // Card playCard(int position);
   // bool cardsArePresentInHand(std::vector<Card> cards);
};

#endif