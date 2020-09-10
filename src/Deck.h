#pragma once
#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>

class Deck {
public:
    Deck(int numOfEachCard  = 2);
    void shuffle();
    Card takeOne();
    int getNumRemaining();
private:
    std::vector<Card> cards;
    const int numOfEachCard;
    void populate();
};

#endif