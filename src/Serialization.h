#pragma once
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <vector>
#include "Card.h"
#include "MeldsStorage.h"

class Serialization {
public:
   Serialization();
   void setPlayerObjects(GroupOfCards hand, MeldsStorage meldsPlayed, GroupOfCards capturePile);
   GroupOfCards setPlayerStrings(std::string handSerialization, std::string meldSerialization, std::string captureSerialization, GroupOfCards allRemCards, Suit trumpSuit);

   std::string getHandString() const;
   std::string getCaptureString() const;
   std::string getMeldString() const;

   GroupOfCards getHand() const;
   MeldsStorage getMeldsPlayed()const ;
   GroupOfCards getCapturePile() const;
private:
   bool playerObjectsEntered;
   bool playerStringsEntered;
   GroupOfCards hand;
   MeldsStorage meldsPlayed;
   GroupOfCards capturePile; 

   std::string handString;
   std::string meldString;
   std::string captureString;

   GroupOfCards convertStringsToObjects(GroupOfCards allCards);
   GroupOfCards handStrToObject(GroupOfCards allCards);
   GroupOfCards meldStrToObject(GroupOfCards allCards, Suit trumpSuit);
   GroupOfCards captureStrToObject(GroupOfCards allCards);

   void convertObjectsToStrings();
   bool isACompleteMeldInstance(MeldInstance meldInstance);
   
};

#endif