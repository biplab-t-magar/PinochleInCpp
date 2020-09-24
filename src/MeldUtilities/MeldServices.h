#pragma once
#ifndef MELD_SERVICES_H
#define MELD_SERVICES_H

#include "Card.h"
#include <vector>
#include "MeldInstance.h"
#include "MeldsStorage.h"


class MeldServices {
public:
   MeldServices();
   MeldServices(Suit trumpSuit);
   MeldsStorage getMeldsPlayed() const;

   bool setTrumpSuit(Suit trumpSuit);
   // bool playMeld(std::vector<Card> meldToBePlayed, std::vector<Card>* handPile, std::vector<Card>* meldPile);
   
   static int getMeldPoints(Meld meld);
   // bool isValidMeld(std::vector<Card> cards, Meld *whatMeld);
   // bool meldTypePlayedFirstTime(Meld meld);
   // bool meldCanNeglectMeldPile(std::vector<Card> handPile, std::vector<Card> meld);
   // bool meldHasCardFromHandPile(std::vector<Card> handPile, std::vector<Card> meld);
   bool MeldServices::storeMeld(GroupOfCards hand, MeldInstance meldInstance);
   // bool isLegalMeld(GroupOfCards hand, MeldInstance meldInstance);
   bool allCardsPresentInHand(GroupOfCards hand, MeldInstance meldInstance);
   bool meldIsNotARepeat(MeldInstance meldInstance);
   bool meldHasANewCard(MeldInstance meldInstance);
   int compareHandsForMelds(GroupOfCards hand1, GroupOfCards hand2);
   std::vector<int> potentialPointsFromHand(GroupOfCards hand);
   MeldsStorage getMeldsFromHand(GroupOfCards hand);
private:
   MeldsStorage meldsPlayed;
   // std::vector<std::vector<Card>> cardsUsedForCreatingPlayedMelds;
   // std::vector<Meld> possibleMelds;
   // std::vector<std::vector<Card>> cardsForCreatingPossibleMelds;
   Suit trumpSuit;
   bool trumpSuitSpecified;

   
   std::vector<int> countMeldsFromHand(GroupOfCards hand);
   
   // int countDixes(GroupOfCards hand);
   // int countPinochles(GroupOfCards hand);
   // int countMarriages(GroupOfCards hand);

   // int countSameSuitMelds(Meld meld, GroupOfCards hand, Suit suit, Rank startingRank, int howManyCards);
   // int countSameRankMelds(Meld meld, GroupOfCards hand, Rank rank);

   std::vector<MeldInstance> getDixes(GroupOfCards hand);
   std::vector<MeldInstance> getPinochles(GroupOfCards hand);
   std::vector<MeldInstance> getMarriages(GroupOfCards hand);
   std::vector<MeldInstance> getSameRankMelds(Meld meld, GroupOfCards hand, Rank rank);    
   std::vector<MeldInstance> getSameSuitMelds(Meld meld, GroupOfCards hand, Suit suit, Rank startingRank, int howManyCards);
   std::vector<MeldInstance> createMeldsFromEligibleCards(std::vector<std::vector<Card>> cards);
   // bool hasMeldBeenUsed();
   // bool moveCardToMeldPile(std::vector<Card>* handPile, std::vector<Card>* meldPile, Card card);
   // bool moveCardsToMeldPile(std::vector<Card>* handPile, std::vector<Card>* meldPile, std::vector<Card> cards);
   // int countSameSuitMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Suit suit, Rank startingRank, int howManyCards);
   // int countSameRankMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Rank rank);
   // int removeExtraneousChoices(Meld meld, std::vector<int> howManyOfEachCard, std::vector<int> howManyFromMeldPile);

   // void getMeldCoordinates(std::vector<Card>* handPile, std::vector<Card>* meldPile, Meld meld);

   // bool isDix(std::vector<Card> cards);
   // bool isAnyMarriage(std::vector<Card> cards);
   // Meld MeldServices::typeOfMarriage(std::vector<Card> cards);
   // // bool isRoyalMarriage(std::vector<Card> cards);
   // bool isPinochle(std::vector<Card> cards);
   // bool isFours(std::vector<Card> cards);
   // bool isFlush(std::vector<Card> cards);

};

#endif