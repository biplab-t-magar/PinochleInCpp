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
   void setPlayerStrings(std::string handSerialization, std::string meldSerialization, std::string captureSerialization);

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

   void handObjectsToString();
   void meldObjectsToString();
   void captureObjectsToString();

   void handStringToObjects();
   void meldStringToObjects();
   void captureStringToObjects();

   void convertObjectsToString();
   void convertStringToObjects();
   bool isACompleteMeldInstance(MeldInstance meldInstance);

};

#endif