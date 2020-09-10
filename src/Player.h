#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <Card.h>

enum class Meld {
    Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
};

class Player {
public:
    Player();
    void nextMove();
    void takeOneCard();
private:
    std::vector<Card> hand;
    std::vector<Card> meldPile;
    std::vector<Card> capturePile;
    Card playFromHand();
    void playMeld();
    void suggestNextMeld();
    void suggestNextThrow();

};

#endif