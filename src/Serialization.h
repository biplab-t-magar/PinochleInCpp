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

   std::string getHandString();
   std::string getCaptureString();
   std::string getMeldString();

   GroupOfCards getHand();
   MeldsStorage getMeldsPlayed();
   GroupOfCards getCapturePile();
private:
   bool playerObjectsEntered;
   bool playerStringsEntered;
   GroupOfCards hand;
   MeldsStorage meldsPlayed;
   GroupOfCards capturePile; 

   std::string handString;
   std::string meldString;
   std::string captureString;

   // void handObjectsToString();
   // void meldObjectsToString();
   // void captureObjectsToString();

   GroupOfCards convertStringsToObjects(GroupOfCards allCards);
   GroupOfCards handStrToObject(GroupOfCards allCards);
   GroupOfCards meldStrToObject(GroupOfCards allCards, Suit trumpSuit);
   GroupOfCards captureStrToObject(GroupOfCards allCards);

   void convertObjectsToStrings();
   bool isACompleteMeldInstance(MeldInstance meldInstance);
   
};

#endif