#pragma once
#ifndef MELD_SERVICES_H
#define MELD_SERVICES_H

#include "Card.h"
#include <vector>

enum class Meld {
    Flush, RoyalMarriage, Marriage, Dix, FourAces, FourKings, FourQueens, FourJacks, Pinochle
};

class MeldServices {
public:
   MeldServices();
   MeldServices(Suit trumpSuit);
   bool setTrumpSuit(Suit trumpSuit);
   bool findAllPlayableMeldsFromHand(std::vector<Card> hand, std::vector<Card> meldPile, Suit trumpSuit);
   size_t getMeldPoints(Meld meld);
   bool isValidMeld(std::vector<Card> cards, Meld *whatMeld);
   bool meldPlayedFirstTime(Meld meld);
   bool meldUsesOnlyHandPile(std::vector<Card> handPile, std::vector<Card> meld);
   bool meldHasCardFromHandPile(std::vector<Card> handPile, std::vector<Card> meld);

private:
   std::vector<Meld> meldsPlayed;
   std::vector<std::vector<Card>> cardsUsedForCreatingPlayedMelds;
   std::vector<Meld> possibleMelds;
   std::vector<std::vector<Card>> cardsForCreatingPossibleMelds;
   Suit trumpSuit;
   bool trumpSuitSpecified;

   bool hasMeldBeenUsed();

   std::vector<std::vector<Card>> searchForDixes(std::vector<Card> handPile);


   bool isDix(std::vector<Card> cards);
   bool isMarriage(std::vector<Card> cards);
   bool isRoyalMarriage(std::vector<Card> cards);
   bool isPinochle(std::vector<Card> cards);
   bool isFours(std::vector<Card> cards);
   bool isFlush(std::vector<Card> cards);

};

#endif