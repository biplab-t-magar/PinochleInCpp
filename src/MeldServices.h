/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
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
   MeldServices(MeldsStorage meldsPlayed, Suit trumpSuit);

   MeldsStorage getMeldsPlayed() const;
   int getMeldPoints(Meld meld) const;
   MeldsStorage getMeldsFromHand(GroupOfCards hand);
   std::vector<int> potentialPointsFromHand(GroupOfCards hand);
   bool allCardsPresentInHand(GroupOfCards hand, MeldInstance meldInstance);
   bool meldIsNotARepeat(MeldInstance meldInstance);
   bool meldHasANewCard(MeldInstance meldInstance);
   int compareHandsForMelds(GroupOfCards hand1, GroupOfCards hand2);

   bool setTrumpSuit(Suit trumpSuit);
   bool storeMeld(GroupOfCards hand, MeldInstance meldInstance);
private:
   MeldsStorage meldsPlayed;
   Suit trumpSuit;
   bool trumpSuitSpecified;   
   std::vector<int> countMeldsFromHand(GroupOfCards hand);
   std::vector<MeldInstance> getDixes(GroupOfCards hand);
   std::vector<MeldInstance> getPinochles(GroupOfCards hand);
   std::vector<MeldInstance> getMarriages(GroupOfCards hand);
   std::vector<MeldInstance> getSameRankMelds(Meld meld, GroupOfCards hand, Rank rank);    
   std::vector<MeldInstance> getSameSuitMelds(Meld meld, GroupOfCards hand, Suit suit, Rank startingRank, int howManyCards);
   std::vector<MeldInstance> createMeldsFromEligibleCards(std::vector<std::vector<Card>> cards);

};

#endif