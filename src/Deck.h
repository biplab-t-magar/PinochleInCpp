#pragma once
#ifndef DECK_H
#define DECK_H

#include <Card.h>
#include <vector>

#define numOfEachCard 2
#define numOfRankTypes 6
#define numOfSuitTypes 4

class Deck {
public:
    Deck();
    void shuffle();
    Card takeOne();
    unsigned int getNumRemaining();
private:
    std::vector<Card> cards;
};

#endif