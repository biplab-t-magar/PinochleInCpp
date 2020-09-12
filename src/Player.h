#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <Card.h>
#include <MeldServices.h>

class Player {
public:
    Player();
    void nextMove();
    void takeOneCard(Card card);
    bool setTrumpSuit(Suit trumpSuit);

private:
    Suit trumpSuit; 
    std::vector<Card> handPile;
    std::vector<Card> meldPile;
    std::vector<Card> capturePile;
    Card playFromHand();
    void playMeld(std::vector<Card> cards);
    void suggestNextMeld();
    void suggestNextThrow();
    bool cardsArePresentInHand(std::vector<Card> cards);

};

#endif