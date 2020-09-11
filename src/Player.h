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
    void takeOneCard(Card card);
    bool setTrumpSuit(Suit trumpSuit);
private:
    Suit trumpSuit; 
    std::vector<Card> hand;
    std::vector<Card> meldPile;
    std::vector<Card> capturePile;
    std::vector<std::vector<Card>> meldsUsed;
    Card playFromHand();
    void playMeld();
    void suggestNextMeld();
    void suggestNextThrow();
    void getPotentialMelds();
    size_t getMeldPoints();
    bool isValidMeld(std::vector<Card> cards, Meld *whatMeld);
    bool hasMeldBeenUsed();
    bool isMeldValid();
    bool isDix(Card card);
    bool isMarriage(std::vector<Card> cards);
    bool isRoyalMarriage(std::vector<Card> cards);
    bool isPinochle(std::vector<Card> cards);
    bool isFours(std::vector<Card> cards);
    bool isFlush(std::vector<Card> cards);

};

#endif