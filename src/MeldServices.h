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
   std::vector<int> countMeldsFromHand(std::vector<Card> handPile, std::vector<Card> meldPile);
   int getMeldPoints(Meld meld);
   bool isValidMeld(std::vector<Card> cards, Meld *whatMeld);
   bool meldTypePlayedFirstTime(Meld meld);
   bool meldCanNeglectMeldPile(std::vector<Card> handPile, std::vector<Card> meld);
   bool meldHasCardFromHandPile(std::vector<Card> handPile, std::vector<Card> meld);
   bool playMeld(std::vector<Card> meldToBePlayed, std::vector<Card>* handPile, std::vector<Card>* meldPile);
private:
   std::vector<Meld> meldsPlayed;
   std::vector<std::vector<Card>> cardsUsedForCreatingPlayedMelds;
   std::vector<Meld> possibleMelds;
   std::vector<std::vector<Card>> cardsForCreatingPossibleMelds;
   Suit trumpSuit;
   bool trumpSuitSpecified;

   std::vector<int> potentialPointsFromHand(std::vector<Card> handPile, std::vector<Card>meldPile1);
   int compareHands(std::vector<Card> handPile1, std::vector<Card>meldPile1, std::vector<Card> handPile2, std::vector<Card> meldPile2);
   bool hasMeldBeenUsed();
   int howManyDixes(std::vector<Card> handPile);
   int howManyPinochles(std::vector<Card> handPile, std::vector<Card> meldPile);
   int howManyMarriages(Suit suit, std::vector<Card> handPile, std::vector<Card> meldPile);
   int countSameSuitMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Suit suit, Rank startingRank, int howManyCards);
   int countSameRankMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Rank rank);
   // int removeExtraneousChoices(Meld meld, std::vector<int> howManyOfEachCard, std::vector<int> howManyFromMeldPile);

   // void getMeldCoordinates(std::vector<Card>* handPile, std::vector<Card>* meldPile, Meld meld);

   bool isDix(std::vector<Card> cards);
   bool isMarriage(std::vector<Card> cards);
   bool isRoyalMarriage(std::vector<Card> cards);
   bool isAnyMarriage(std::vector<Card> cards);
   bool isPinochle(std::vector<Card> cards);
   bool isFours(std::vector<Card> cards);
   bool isFlush(std::vector<Card> cards);

};

#endif