#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include "MeldServices.h"
#include <string>

class Player {
public:
   Player();
   Player(GroupOfCards hand, GroupOfCards capturePile, MeldsStorage meldsPlayed, Suit trumpSuit);
   virtual ~Player();

   GroupOfCards getHand() const;
   MeldsStorage getMeldsPlayed() const;
   GroupOfCards getCapturePile() const;
   int numCardsInHand();

   bool setTrumpSuit(Suit trumpSuit);

   void takeOneCard(Card card);
   virtual Card playLeadCard() = 0;
   virtual Card playChaseCard(Card opponentCard) = 0;
   virtual MeldInstance playMeld() = 0;
   void addToCapturePile(Card card1, Card card2);
   virtual void getHelpForLeadCard() = 0;
   virtual void getHelpForChaseCard(Card opponentCard) = 0;
   virtual void getHelpForMeld() = 0;
   bool isMeldPossible();
   

protected:
   int getCardPositionInHand(Card card);
   Card suggestLeadCard(std::string &reasoning);
   Card suggestChaseCard(std::string &reasoning, Card opponentCard);
   MeldInstance suggestMeld(std::string &reasoning);
   Card playFromHand(int position);
   Card playFromHand(Card card);
   MeldInstance createMeld(std::vector<int> positions);
   MeldInstance createMeld(MeldInstance meldInstance);
private:
   GroupOfCards hand;
   GroupOfCards capturePile;
   Suit trumpSuit; 
   MeldServices meldServices;
   std::vector<std::string> handPile;
   std::vector<std::string> meldStrRep;
   std::vector<std::string> capturePileStrRep;

   Card getLeastRankedCard();
   Card getLeastRankedFrom(std::vector<Card> cards);
   std::vector<Card> bestCardsForLeadThrow();
   MeldInstance findBestMeldToPlay(std::vector<MeldInstance> meldsToCompare);
   
   
};

#endif