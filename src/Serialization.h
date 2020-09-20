#pragma once
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <vector>
#include "Card.h"

class Serialization {
public:
   Serialization();
   void cardPlayed();
   void meldPlayed();
private:
   std::vector<Card> handPile;
   std::vector<Card> meldPile;
   std::vector<Card> capturePile; 
};

#endif