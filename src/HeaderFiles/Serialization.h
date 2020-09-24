#pragma once
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <vector>
#include "Card.h"
#include "MeldsStorage.h"

class Serialization {
public:
   Serialization();
   bool getPlayerCards(GroupOfCards hand, MeldsStorage meldsPlayed, GroupOfCards capturePile);
   bool getPlayerData(std::string handSerialization, std::string meldSerialization, std::string captureSerialization);

   std::string getHandSerialization();
   std::string getCaptureSerialization();
   std::string getMeldSerialization();

   GroupOfCards getHand();
   MeldsStorage getMeldsPlayed();
   GroupOfCards getCapturePile();
private:
   bool playerCardsEntered;
   bool playerDataEntered;
   GroupOfCards hand;
   MeldsStorage meldsPlayed;
   GroupOfCards capturePile; 

   std::string handSerialization;
   std::string meldSerialization;
   std::string captureSerialization;
};

#endif