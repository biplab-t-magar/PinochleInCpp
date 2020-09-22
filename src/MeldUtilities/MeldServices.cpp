#include "MeldServices.h"
#include <algorithm>

#define numOfEachCard 2
#define numOfMeldTypes 9

MeldServices::MeldServices() {
   this->trumpSuitSpecified = false;
}

MeldServices::MeldServices(Suit trumpSuit) {
   this->trumpSuit = trumpSuit;
   this->trumpSuitSpecified = true;
}

bool MeldServices::setTrumpSuit(Suit trumpSuit) {
   this->trumpSuit = trumpSuit;
   this->trumpSuitSpecified = true;
   return true;
}

bool MeldServices::allCardsPresentInHand(GroupOfCards hand, MeldInstance meldInstance) {
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      //check if each card in the candidate meld is present in the hand
      if(hand.searchCardById(meldInstance.getCardByPosition(i).getId()) == false) {
         return false;
      }
   }
   return true;
}

bool MeldServices::meldIsNotARepeat(MeldInstance meldInstance) {
   for(int i = 0; i < meldInstance.getNumOfCards(); i++) {
      if(meldsPlayed.isCardUsedByMeld(meldInstance.getCardByPosition(i), meldInstance.getMeldType())) {
         return false;
      }
   }
   return true;
}

bool MeldServices::storeMeld(GroupOfCards hand, MeldInstance meldInstance) {
   if(!meldInstance.isValidMeld() || !allCardsPresentInHand(hand, meldInstance) || !meldIsNotARepeat(meldInstance)) {
      return false;
   }
   meldsPlayed.addMeld(meldInstance);
   return true;  
}

// bool MeldServices::storeMeld(std::vector<Card> cardsToBePlayed, std::vector<Card>* handPile, std::vector<Card>* meldPile) {
//    Meld whatMeld;
//    if(!isValidMeld(cardsToBePlayed, &whatMeld)) {
//       return false;
//    }

//    //if at least one meld of given meld is possible in the player's entire hand
//    if(countMeldsFromHand(*handPile, *meldPile)[static_cast<int>(whatMeld)] <= 0) {
//       return false;
//    } 
//    //marriages can be of 3 different types, for each 3 non-trump suits
//    //check if that specific marriage provided is present in hand
//    if(whatMeld == Meld::Marriage) {
//       //count how many of the marraig
//       if(countMarriages(cardsToBePlayed[0].getSuit(), *handPile, *meldPile) <= 0) {
//          return false;
//       }
//    }

//    //to keep track of the cards in handPile and meldPile
//    std::vector<bool> isCardInMeldPile;
//    std::vector<bool> isCardInHandPile;
//    //initialize all indexes to false
//    for(int i = 0; i < cardsToBePlayed.size(); i++) {
//       isCardInMeldPile[i] = false;
//       isCardInHandPile[i] = false;
//    }

//    //loop through all cards
//    for(int i = 0; i < cardsToBePlayed.size(); i++) {
//       //check if the card is in the meld pile
//       for(int j = 0; j < (*meldPile).size(); j++) {
//          if(cardsToBePlayed[i] == (*meldPile)[j]) {
//             isCardInMeldPile[i] = true;
//             break;
//          }
//       }
//       //check if the card is in the hand pile
//       for(int j = 0; j < (*handPile).size(); j++) {
//          if(cardsToBePlayed[i] == (*handPile)[j]) {
//             isCardInHandPile[i] = true;
//             break;
//          }
//       }
//    }

//    //variable to indicate what cards from handPile should be transferred to meldPile to create the meld
//    std::vector<Card> whatCardsToTransfer;

//    //finding out what cards are not in meldPile 
//    for(int i = 0; i < cardsToBePlayed.size(); i++) {
//       //if the card is not in the meld pile, then we need to transfer that card from the handPile to meldpile to create the meld
//       if(isCardInMeldPile[i] == false) {
//           //keeping track of the card once not found in the meldPile
//           //this is the card to be sought in the handPile 
//          whatCardsToTransfer.push_back(cardsToBePlayed[i]);
//       } 
//    }
//    //if by any chance all the needed cards were in the meld pile already:
//    if(whatCardsToTransfer.size() == 0) {
//       //first check if the meld has been played before in the round already.
//       if(!meldTypePlayedFirstTime) {
//          //if it has been played already, search for all the cards in the handPile and do not search for any in the meld pile
//          //finding what index of the handPile is each card in
//          for(int i = 0; i < cardsToBePlayed.size(); i++) {
//             for(int j = 0; j < (*handPile).size(); j++) {
//                if(cardsToBePlayed[i] == (*handPile)[j]) {
//                   whatCardsToTransfer.push_back(cardsToBePlayed[i]);
//                   break;
//                }
//             }
//          }
//       //if not all the cards are found in the handPile (although they should be found there
//       //because we have checked that previously already), return false
//          if(cardsToBePlayed.size() != whatCardsToTransfer.size()) {
//             return false;
//          }

//       }
//       //if the meld has not been played in the round before
//       //we need to selectively look for one eligible card to bring from the handPile
//       else {   
//          //get a list of all the cards  in the handPile that are eligible for the meld
//          std::vector<Card> eligibleHandCards;
//          for(int i = 0; i < cardsToBePlayed.size(); i++) {
//             if(isCardInHandPile[i]) {
//                eligibleHandCards.push_back(cardsToBePlayed[i]);
//             }
//          }
//          //create two mock handPiles and meldPiles so that we can know what card will bring about the
//          //the better situation in future melds 
//          std::vector<Card> idealHandPile = *handPile;
//          std::vector<Card> idealMeldPile = *meldPile;
//          std::vector<Card> contestantHandPile = *handPile;
//          std::vector<Card> contestantMeldPile = *meldPile;

//          //now, we hypothetically transfer each card in handPile to meldPile to create 
//          //as many prospective handPile/meldPile pairs as there are and compare them to each other
//          moveCardToMeldPile(&idealHandPile, &idealMeldPile, eligibleHandCards[0]);
//          whatCardsToTransfer.push_back(eligibleHandCards[0]);
//          for(int i = 1; i < eligibleHandCards.size(); i++) {
//             moveCardToMeldPile(&contestantHandPile, &contestantMeldPile, eligibleHandCards[i]);
//             //now compare prospective ideal hand and contestant hand
//             //if the latter has better prospects, make that hand into ideal hand
//             if(compareHands(idealHandPile, idealMeldPile, contestantHandPile, contestantMeldPile) == 2) {
//                idealHandPile = contestantHandPile;
//                idealMeldPile = contestantMeldPile;
//                //push that card which, when pushed to the meld would make the ideal hand, into the 
//                //emptied whatCardsToTransfer vector
//                whatCardsToTransfer.clear();
//                whatCardsToTransfer.push_back(eligibleHandCards[i]);
//             } 
//          }
//       }
//    }
//    //now we have a list of all the cards we need to transfer from hand pile to meld pile to 
//    //complete the playing of the meld
//    return moveCardsToMeldPile(handPile, meldPile, whatCardsToTransfer);
// }




// bool MeldServices::moveCardToMeldPile(std::vector<Card>* handPile, std::vector<Card>* meldPile, Card card){
//    std::vector<Card> cards;
//    cards.push_back(card);
//    return moveCardsToMeldPile(handPile, meldPile, cards);
// }

// bool MeldServices::moveCardsToMeldPile(std::vector<Card>* handPile, std::vector<Card>* meldPile, std::vector<Card> cards) {
//    int initHandPileSize = (*handPile).size();
//    //all cards needed to be transferred are looped through 
//    for(int i = 0; i < cards.size(); i++) {
//       //look for each card in handPile
//       for(int j = 0; j < (*handPile).size(); j++) {
//          if((*handPile)[j] == cards[i]) {
//             (*meldPile).push_back(cards[i]);
//             (*handPile).erase((*handPile).begin() + j);
//          } 
//       }
//    }
//    //if all cards were found and moved to meld pile, return true
//    if((initHandPileSize - (*handPile).size()) == cards.size()) {
//       return true;
//    } else {
//       return false;
//    }
// }

int MeldServices::compareHandsForMelds(GroupOfCards hand1, GroupOfCards hand2) {
   //get the points (from highest to lowest) for each possible meld in the two candidate hands

   std::vector<int> hand1Points = potentialPointsFromHand(hand1);
   std::vector<int> hand2Points = potentialPointsFromHand(hand2);
   
   //first, check which hand has the meld that provides the highest points
   if (hand1Points[0] > hand2Points[0]) {
      return 1;
   } else if (hand1Points[0] < hand2Points[0]) {
      return 2;
   } 
   //if the highest points of each is the same, compare the total potential points produced by all the melds of each hand
   int totalHand1Points, totalHand2Points = 0;
   for(int i = 0; i < hand1Points.size(); i++) {
      totalHand1Points+= hand1Points[i];
   }
   for(int i = 0; i < hand2Points.size(); i++) {
      totalHand2Points+= hand2Points[i];
   }
   if(totalHand1Points > totalHand2Points) {
      return 1;
   } else if(totalHand2Points > totalHand1Points) {
      return 2;
   }

   //if again the total possible points of each hand are the same, compare the 2nd highest, 3rd highest,...
   //and so on points of each meld until a winner is found
   int i = 0;
   while(i < hand1Points.size()) {
      if (hand1Points[i] > hand2Points[i]) {
         return 1;
      } else if (hand1Points[i] < hand2Points[i]) {
         return 2;
      } 
   }

   //if no difference between the possible points from each hand was found, return 0, standing for draw
   return 0;
}

std::vector<int> MeldServices::potentialPointsFromHand(GroupOfCards hand) {
   std::vector<int> countsOfEachMeldType = countMeldsFromHand(hand);
   //now, get the vector containing the points for each possible meld in the hand
   std::vector<int> points;
   //loop through all the meld types
   for(int i = 0 ; i < countsOfEachMeldType.size(); i++) {
      //add separate entry for each occurence of a meld type (what is entered into the vector should be the points a meld gives)
      for(int j = 0; j < countsOfEachMeldType[i]; j++) {
         points.push_back(getMeldPoints(static_cast<Meld>(i)));
      }
   }
   //sort the vector
   std::sort(points.begin(), points.end(), std::greater<>()); 
   return points;
}

std::vector<int> MeldServices::countMeldsFromHand(GroupOfCards hand) {
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified yet. Use MeldServices.setTrumpSuit() to specify trump suit");
   }
   
   //vector to hold each meld type
   std::vector<int> numOfEachMeld;
   //initialize all values in vector to 0
   for(int i = 0; i < numOfMeldTypes; i++) {
      numOfEachMeld.push_back(0);
   }

   //now assign the number of possible instances for each meld
   
   //flush
   numOfEachMeld[static_cast<int>(Meld::Flush)] = getSameSuitMelds(Meld::Flush, hand, trumpSuit, Rank::Ace, 5).size();
   //royal marriage
   numOfEachMeld[static_cast<int>(Meld::RoyalMarriage)] = getSameSuitMelds(Meld::RoyalMarriage, hand, trumpSuit, Rank::King, 2).size();
   //marriage
   numOfEachMeld[static_cast<int>(Meld::Marriage)] = getMarriages(hand).size();
   //dix
   numOfEachMeld[static_cast<int>(Meld::Dix)] = getDixes(hand).size();
   //FourAces
   numOfEachMeld[static_cast<int>(Meld::FourAces)] = getSameRankMelds(Meld::FourAces, hand, Rank::Ace).size();
   //Four Kings
   numOfEachMeld[static_cast<int>(Meld::FourKings)] = getSameRankMelds(Meld::FourKings, hand, Rank::King).size();
   //Four Queens
   numOfEachMeld[static_cast<int>(Meld::FourQueens)] = getSameRankMelds(Meld::FourQueens, hand, Rank::Queen).size();
   //Four Jacks
   numOfEachMeld[static_cast<int>(Meld::FourJacks)] = getSameRankMelds(Meld::FourJacks, hand, Rank::Jack).size();
   //Pinochle
   numOfEachMeld[static_cast<int>(Meld::Pinochle)] = getPinochles(hand).size();

   return numOfEachMeld;
}

MeldsStorage MeldServices::getMeldsFromHand(GroupOfCards hand) {
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified yet. Use MeldServices.setTrumpSuit() to specify trump suit");
   }
   
   MeldsStorage allPossibleMelds;

   //now push the number of possible instances of each meld
   
   //flush
   allPossibleMelds.addMelds(getSameSuitMelds(Meld::Flush, hand, trumpSuit, Rank::Ace, 5));
   //royal marriage
   allPossibleMelds.addMelds(getSameSuitMelds(Meld::RoyalMarriage, hand, trumpSuit, Rank::King, 2));
   //marriage
   allPossibleMelds.addMelds(getMarriages(hand));
   //dix
   allPossibleMelds.addMelds(getDixes(hand));
   //FourAces
   allPossibleMelds.addMelds(getSameRankMelds(Meld::FourAces, hand, Rank::Ace));
   //Four Kings
   allPossibleMelds.addMelds(getSameRankMelds(Meld::FourKings, hand, Rank::King));
   //Four Queens
   allPossibleMelds.addMelds(getSameRankMelds(Meld::FourQueens, hand, Rank::Queen));
   //Four Jacks
   allPossibleMelds.addMelds(getSameRankMelds(Meld::FourJacks, hand, Rank::Jack));
   //Pinochle
   allPossibleMelds.addMelds(getPinochles(hand));

   return allPossibleMelds;
}

std::vector<MeldInstance> MeldServices::getDixes(GroupOfCards hand) {
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified yet. Use setTrumpSuit() to specify trump suit");
   }

   std::vector<MeldInstance> dixes;
   //get all the 9 of Trumps from the hand
   std::vector<Card> allDixes = hand.getCardsByRankAndSuit(Rank::Nine, trumpSuit);

   //go through each of these 9 of Trumps and discount those which have been used already to make a Dix
   for(int i = 0; i < allDixes.size(); i++) {
      if(meldsPlayed.isCardUsedByMeld(allDixes[i], Meld::Dix) == false) {
         //create a meld instance out of the eligible card and push it to dixes
         dixes.push_back(MeldInstance(std::vector<Card>(allDixes.begin(), allDixes.begin() + 1), trumpSuit));
      }
   }


   return dixes;
}

std::vector<MeldInstance> MeldServices::getPinochles(GroupOfCards hand) {
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified yet. Use setTrumpSuit() to specify trump suit");
   }
   std::vector<std::vector<Card>> playableCards;

   std::vector<Card> allJackOfDiamonds = hand.getCardsByRankAndSuit(Rank::Jack, Suit::Diamonds);
   std::vector<Card> allQueenOfSpades = hand.getCardsByRankAndSuit(Rank::Queen, Suit::Spades);

   //go through each Jack of Diamonds and discount those which have been used to make a Pinochle before
   for(int i = 0; i < allJackOfDiamonds.size(); i++) {
      if(meldsPlayed.isCardUsedByMeld(allJackOfDiamonds[i], Meld::Pinochle) == false) {
         //store all Jack of Diamonds in the first position
         playableCards[0].push_back(allJackOfDiamonds[i]);
      }
   }
   //do the same for Queen of Spades
   for(int i = 0; i < allQueenOfSpades.size(); i++) {
      if(meldsPlayed.isCardUsedByMeld(allQueenOfSpades[i], Meld::Pinochle) == false) {
         //store all Queen of Spades in the second position
         playableCards[1].push_back(allQueenOfSpades[i]);
      }
   }

   return createMeldsFromEligibleCards(playableCards);
   
}

std::vector<MeldInstance> MeldServices::getMarriages(GroupOfCards hand) {
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified yet. Use setTrumpSuit() to specify trump suit");
   }
   std::vector<MeldInstance> marriages;
   std::vector<MeldInstance> marriageStore;
   //loop through each suit
   for(int suit = 0; suit < 4; suit++) {
      if(static_cast<Suit>(suit) != trumpSuit) {
         marriageStore = getSameSuitMelds(Meld::RoyalMarriage, hand, static_cast<Suit>(suit), Rank::King, 2);
         marriages.insert(marriages.begin(), marriageStore.begin(), marriageStore.end());
      }
   }
   
   return marriages;
}
// int MeldServices::countSameSuitMelds(Meld meld, GroupOfCards hand, Suit suit, Rank startingRank, int howManyCards) {
   
//    //if the number in howManyCards is more than the number of Ranks in the meld
//    if((static_cast<int>(startingRank) - howManyCards + 1) <= 0 ) {
//       throw PinochleException("Invalid starting rank or number of cards");
//    }
//    if(howManyCards <= 0) {
//       throw PinochleException("There cannot be a meld with 0 cards");
//    }

//    //this vector counts the number of each rank of the specified suit in the hand
//    //only ranks relevant to the meld are counted
//    std::vector<int> howManyOfEachRank;

//    int rank = static_cast<int>(startingRank);

//    //for temporarily holding groups of candidate cards
//    std::vector<Card> cardHolder;

//    //for keeping count of the number of instances of a particular card eligible for the meld
//    int eligibleCardsCount;

//    //loop through all the ranks in the meld
//    for(int i = 0; i < howManyCards; i++) {
//       eligibleCardsCount = 0;
      
//       //get all the cards with the given rank and with the suit of the current loop
//       cardHolder = hand.getCardsByRankAndSuit(static_cast<Rank>(rank), suit);
      
//       //now check how many of these cards have already been used to create this meld
//       for(int i = 0; i < cardHolder.size(); i++) {
//          //only add to count if the card has not already been used for this meld
//          if(meldsPlayed.isCardUsedByMeld(cardHolder[i], meld) == false) {
//             eligibleCardsCount++;
//          }
//       }

//       //keep count of the number of the count for each suit of the given rank
//       howManyOfEachRank.push_back(eligibleCardsCount);

//       //go to the next rank in the meld
//       rank--;
      
//    }

//    //now the number of *mutually exclusive* possible melds is calculated
//    //the number of that constituent rank of the meld which occurs the least times in the meld is the actual 
//    //number of mutually exclusive possible melds

//    //finding minimum value in howManyOfEachRank, which represents number of mutually exclusive possible melds
//    int min = howManyOfEachRank[0];
//    for(int i = 1; i < howManyOfEachRank.size(); i++) {
//       if(min > howManyOfEachRank[i]) {
//          min = howManyOfEachRank[i];
//       } 
//    }
//    return min;
// }

// int MeldServices::countSameRankMelds(Meld meld, GroupOfCards hand, Rank rank) {
//    //this vector counts the number of each suit of the specified rank
//    std::vector<int> howManyOfEachSuit;
//    //holds the current suit (in int form) being counted (always start with clubs and increase to other suits from there)
//    int suit = 0;

//    //for temporarily holding groups of candidate cards
//    std::vector<Card> cardHolder;

//    //for keeping count of the number of instances of a particular card eligible for the meld
//    int eligibleCardsCount;
//    //loop through all the suits
//    for(int i = 0; i < 4; i++) {
//       eligibleCardsCount = 0;
      
//       //get all the cards with the given rank and with the suit of the current loop
//       cardHolder = hand.getCardsByRankAndSuit(rank, static_cast<Suit>(suit));
      
//       //now check how many of these cards have already been used to create this meld
//       for(int i = 0; i < cardHolder.size(); i++) {
//          //only add to count if the card has not already been used for this meld
//          if(meldsPlayed.isCardUsedByMeld(cardHolder[i], meld) == false) {
//             eligibleCardsCount++;
//          }
//       }

//       //keep count of the number of the count for each suit of the given rank
//       howManyOfEachSuit.push_back(eligibleCardsCount);

//       //go to the next suit of the meld
//       suit++;
//    }
//    //finding minimum value in howManyOfEachSuit (which card in the potential meld occurs the minimum number of times)
//    //any cards with occurence more than that minimum value have extraneous instances
//    //that represent choices we can make when we create melds
//    //min represents number of mutually exclusive possible melds
//    int min = howManyOfEachSuit[0];
//    for(int i = 1; i < howManyOfEachSuit.size(); i++) {
//       if(min > howManyOfEachSuit[i]) {
//          min = howManyOfEachSuit[i];
//       } 
//    }

//    return min;
// }

std::vector<MeldInstance> MeldServices::getSameRankMelds(Meld meld, GroupOfCards hand, Rank rank) {
   //this vector holds all possible cards of each suit of the specified rank in the hand
   std::vector<std::vector<Card>> cardsOfEachSuit;
   //holds the current suit (in int form) being counted (always start with clubs and increase to other suits from there)
   int suit = 0;

   //for temporarily holding groups of candidate cards
   std::vector<Card> cardHolder;

   //for keeping track of the instances of a particular card eligible for the meld
   std::vector<Card> eligibleCards;
   //loop through all the suits
   for(int i = 0; i < 4; i++) {
      eligibleCards.clear();
      
      //get all the cards with the given rank and with the suit of the current loop
      cardHolder = hand.getCardsByRankAndSuit(rank, static_cast<Suit>(suit));
      
      //now check how many of these cards have already been used to create this meld
      for(int i = 0; i < cardHolder.size(); i++) {
         //only add card to eligibleCards if the card has not already been used for this meld
         if(meldsPlayed.isCardUsedByMeld(cardHolder[i], meld) == false) {
            eligibleCards.push_back(cardHolder[i]);
         }
      }

      //keep track of each instance of the suit of the given rank
      cardsOfEachSuit.push_back(eligibleCards);

      //go to the next suit of the meld
      suit++;
   }

   return createMeldsFromEligibleCards(cardsOfEachSuit);
   
}

std::vector<MeldInstance> MeldServices::getSameSuitMelds(Meld meld, GroupOfCards hand, Suit suit, Rank startingRank, int howManyCards) {
   
   //if the number in howManyCards is more than the number of Ranks in the meld
   if((static_cast<int>(startingRank) - howManyCards + 1) <= 0 ) {
      throw PinochleException("Invalid starting rank or number of cards");
   }
   if(howManyCards <= 0) {
      throw PinochleException("There cannot be a meld with 0 cards");
   }

   //this vector holds all possible cards of each rank of the specified suit in the hand
   //only ranks relevant to the meld are stored
   std::vector<std::vector<Card>> cardsOfEachRank;

   //holds the rank to be counted in each iteration
   int rank = static_cast<int>(startingRank);

   //for temporarily holding groups of candidate cards
   std::vector<Card> cardHolder;

   //for keeping count of the number of instances of a particular card eligible for the meld
   std::vector<Card> eligibleCards;

   //loop through all the ranks in the meld
   for(int i = 0; i < howManyCards; i++) {
      eligibleCards.clear();
      
      //get all the cards with the given rank and with the suit of the current loop
      cardHolder = hand.getCardsByRankAndSuit(static_cast<Rank>(rank), suit);
      
      //now check how many of these cards have already been used to create this meld
      for(int i = 0; i < cardHolder.size(); i++) {
         //only add card to eligibleCards if the card has not already been used for this meld
         if(meldsPlayed.isCardUsedByMeld(cardHolder[i], meld) == false) {
            eligibleCards.push_back(cardHolder[i]);
         }
      }

      //keep track of each instance of the rank of the given suit
      cardsOfEachRank.push_back(eligibleCards);

      //go to the next rank in the meld
      rank--;
      
   }

   return createMeldsFromEligibleCards(cardsOfEachRank);

}

std::vector<MeldInstance> MeldServices::createMeldsFromEligibleCards(std::vector<std::vector<Card>> cards) {
   //finding the card type that has the least instances in cards (i.e. which card in the potential meld occurs the least number of times)
   //Any cards with occurence more than that minimum value have extraneous instances that represent choices we can make when we create melds
   //min represents number of mutually exclusive possible melds
   int min = cards[0].size();
   for(int i = 1; i < cards.size(); i++) {
      if(min > cards[i].size()) {
         min = cards[i].size();
      } 
   }
   
   if(min == 0) {
      //if any card needed to create the meld is missing, return an empty vector
      cards.clear();
   } else {
      //now, we simply disregard the extra instances of a given card type because it makes no difference which instance of the card 
      //we use to create the meld
      for(int i = 0; i < cards.size(); i++) {
         //reduce size of each vector to be the same as min
         cards[i] = std::vector<Card>(cards[i].begin(), cards[i].begin() + min); 
      }
   }

   //now make a meld instance by combining cards
   std::vector<MeldInstance> allPossibleMelds;
   MeldInstance meldStore;
   //loop through all the instances of a card type
   for(int i = 0; i < min; i++) {
      //loop through each card type needed to create the meld
      for(int j = 0; j < cards.size(); j++) {
         meldStore.addCard(cards[j][i]);
      }
   }

   return allPossibleMelds;
}



// int MeldServices::countSameSuitMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Suit suit, Rank startingRank, int howManyCards) {
   
//    //if the number in howManyCards is more than the number of Ranks in the meld
//    if((static_cast<int>(startingRank) - howManyCards + 1) <= 0 ) {
//       throw PinochleException("Invalid starting rank or number of cards");
//    }
//    if(howManyCards <= 0) {
//       throw PinochleException("There cannot be a meld with 0 cards");
//    }
//    //this vector counts the number of each rank of the specified suit
//    //only ranks relevant to the meld are counted
//    std::vector<int> howManyOfEachRank;
//    //this vectors holds the count of each rank of the specified suit that was found in the meld pile
//    // std::vector<int> howManyOfEachRankFromMeldPile;
//    //holds the current rank (in int form) being counted 
//    int rank = static_cast<int>(startingRank);

//    // bool meldHasBeenPlayedBefore = !meldTypePlayedFirstTime(meld);

//    //loop through all the ranks in the meld
//    for(int i = 0; i < howManyCards; i++) {
//       //start counting number of current rank, with count initialized to 0
//       howManyOfEachRank.push_back(0);
//       //start counting number of current rank taken from meld pile, with count initialized to 0
//       // howManyOfEachRankFromMeldPile.push_back(0);

//       //loop through the hand pile
//       for(int j = 0; j <  handPile.size(); j++) {
//          //if both rank and suit match
//          if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
//             howManyOfEachRank[i]++;
//          }
//       }
//       //loop through the meld pile only if this meld has not been previously created by player
//       // if(!meldHasBeenPlayedBefore) {
//       //    for(int j = 0; j <  meldPile.size(); j++) {
//       //    //if both rank and suit match
//       //       if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
//       //          howManyOfEachRank[i]++;
//       //          howManyOfEachRankFromMeldPile[i]++;
//       //       }
//       //    }
//       // }
//       //go to the next rank of the meld
      
//       //loop through meld pile
//       for(int j = 0; j <  meldPile.size(); j++) {
//          //if both rank and suit match
//          if(static_cast<int>(handPile[j].getRank()) == rank && handPile[j].getSuit() == suit) {
//             howManyOfEachRank[i]++;
//             // howManyOfEachSuitFromMeldPile[i]++;
//          }
//       }
//       //since a card cannot be part of the same meld more than once, however many times the given meld 
//       //has already been declared, that many instances of the given card needs to be discounted
//       howManyOfEachRank[i] = howManyOfEachRank[i] - meldsPlayed[static_cast<int>(meld)];
//       rank--;
      
//    }
//    // for(int i = 0; i < howManyCards; i++) {
//    //    if(!meldHasBeenPlayedBefore) {
//    //       for(int j = 0; j <  meldPile.size(); j++) {
//    //       //if both rank and suit match
//    //          if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
//    //             howManyOfEachRank[i]++;
//    //             // howManyOfEachRankFromMeldPile[i]++;
//    //          }
//    //       }
//    //    }
//    //    rank--;
//    // }


//    //now the number of *mutually exclusive* possible melds is calculated
//    //the number of that constituent rank of the meld which occurs the least times in the meld is the actual 
//    //number of mutually exclusive possible melds

//    //finding minimum value in howManyOfEachRank
//    int min = howManyOfEachRank[0];
//    for(int i = 1; i < howManyOfEachRank.size(); i++) {
//       if(min > howManyOfEachRank[i]) {
//          min = howManyOfEachRank[i];
//       } 
//    }
//    return min;
// }

// int MeldServices::countSameRankMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Rank rank) {
//    //this vector counts the number of each suit of the specified rank
//    std::vector<int> howManyOfEachSuit;
//    //this vectors holds the count of each rank of the specified suit that was found in the meld pile
//    // std::vector<int> howManyOfEachSuitFromMeldPile;
//    //holds the current suit (in int form) being counted (always start with clubs and increase to other suits from there)
//    int suit = 0;

//    // bool meldHasBeenPlayedBefore = !meldTypePlayedFirstTime(meld);


//    //loop through all the suits in the meld
//    for(int i = 0; i < 4; i++) {
//       //start counting number of current rank, with count initialized to 0
//       howManyOfEachSuit.push_back(0);
//       //start counting number of current rank taken from meld pile, with count initialized to 0
//       // howManyOfEachSuitFromMeldPile.push_back(0);
//       //loop through the hand pile
//       for(int j = 0; j <  handPile.size(); j++) {
//          //if both rank and suit match
//          if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
//             howManyOfEachSuit[i]++;
//          }
//       }
//       //loop through meld pile
//       for(int j = 0; j <  meldPile.size(); j++) {
//          //if both rank and suit match
//          if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
//             howManyOfEachSuit[i]++;
//             // howManyOfEachSuitFromMeldPile[i]++;
//          }
//       }
//       //since a card cannot be part of the same meld more than once, however many times the given meld 
//       //has already been declared, that many instances of the given card needs to be discounted
//       howManyOfEachSuit[i] = howManyOfEachSuit[i] - meldsPlayed[static_cast<int>(meld)];

//       //go to the next rank of the meld
//       suit++;
//    }
//    //now do the same for the meld pile
//    // suit = 0;
//    // for(int i = 0; i < 4; i++) {
//    //    for(int j = 0; j <  meldPile.size(); j++) {
//    //       //if both rank and suit match
//    //       if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
//    //          howManyOfEachSuit[i]++;
//    //          // howManyOfEachSuitFromMeldPile[i]++;
//    //       }

//    //       //since a card cannot be part of the same meld more than once, however many times the given meld 
//    //       //has already been declared, that many instances of the given card needs to be discounted
//    //       howManyOfEachSuit[i] = howManyOfEachSuit[i] - meldsPlayed[static_cast<int>(meld)];
//    //    }
//    //    // }
//    //    suit++;
//    // }
//    //finding minimum value in howManyOfEachSuit.
//    //any cards with occurence more than that minimum value have extraneous instances
//    //which represent choices we can make when we create melds
//    //min represents number of mutually exclusive possible melds
//    int min = howManyOfEachSuit[0];
//    for(int i = 1; i < howManyOfEachSuit.size(); i++) {
//       if(min > howManyOfEachSuit[i]) {
//          min = howManyOfEachSuit[i];
//       } 
//    }
   

//    return min;
// }


// int MeldServices::removeExtraneousChoices(Meld meld, std::vector<int> numOfEachCard, std::vector<int> howManyFromMeldPile) {
//    if(numOfEachCard.size() != howManyFromMeldPile.size()) {
//       throw PinochleException("numOfEachCard and howManyFromMeldPile must have same size");
//    }
//    if(numOfEachCard.size() <= 0) {
//       throw PinochleException("Arguments cannot be empty vectors");
//    }
//    //finding minimum value in numOfEachCard
//    //any card with occurence more than that minimum value has extraneous cards
//    //which represent choices we can make when we create melds
//    //min represents number of mutually exclusive possible melds
//    int mutuallyExclusivePossibleMelds = numOfEachCard[0];
//    for(int i = 1; i < numOfEachCard.size(); i++) {
//       if(mutuallyExclusivePossibleMelds > numOfEachCard[i]) {
//          mutuallyExclusivePossibleMelds = numOfEachCard[i];
//       } 
//    }

//    //Despite choices, the possible number of mutually exclusive melds is not affected.
//    //So, we can eliminate these extraneous choices 
   
//    //If any extraneous choices are in meld pile we want to remove from the meldPile because choosing 
//    //from the meldPile might make an invalid meld
//    //This explains the decision below to subtract choices from the meldPile as we eliminate choices

//    //loop through all cards and delete extraneous occurrences (choices) of each card from vector 
//    //representing the meldPile and from the vector representing the entire hand
//    // for(int i = 0; i < numOfEachCard.size(); i++) {
//    //    howManyFromMeldPile[i] = howManyFromMeldPile[i] - (numOfEachCard[i] - mutuallyExclusivePossibleMelds);
//    //    numOfEachCard[i] = mutuallyExclusivePossibleMelds;
//    // }  
//    // //Now, for each possible occurence of the meld, we want to eliminate those that 
//    // //use cards from the meldPile if the meld has already been used by the user
//    // for(int i = 0; i < numOfEachCard.size(); i++) {
//    //    if(howManyFromMeldPile[i] > 0 && !meldTypePlayedFirstTime(meld)) {
//    //       //removing choices decreases the card with the least occurences
//    //       //and thus decreases the possible mutually exclusive melds
//    //       if(mutuallyExclusivePossibleMelds > (numOfEachCard[i] - howManyFromMeldPile[i])) {
//    //          mutuallyExclusivePossibleMelds = numOfEachCard[i] - howManyFromMeldPile[i];
//    //       }
//    //    }
//    // } 
//    return mutuallyExclusivePossibleMelds;
   
// }


int MeldServices::getMeldPoints(Meld meld) {
   switch(meld) {
      case Meld::Flush:
         return 150;
         break;
      case Meld::RoyalMarriage:
         return 40;
         break;
      case Meld::Marriage:
         return 20;
         break;
      case Meld::Dix:
         return 10;
         break;
      case Meld::FourAces:
         return 100;
         break;
      case Meld::FourKings:
         return 80;
         break;
      case Meld::FourQueens:
         return 60;
         break;
      case Meld::FourJacks:
         return 40;
         break;
      case Meld::Pinochle:
         return 40;
         break;
      default:
         return 0;
         break;
   }
}

// void MeldServices::addToMeldsPlayed(Meld meld) {
   
// }


// bool MeldServices::meldTypePlayedFirstTime(Meld meld) {
//    if(meldsPlayed[static_cast<int>(meld)] == 0) {
//       return true;
//    } else {
//       return false;
//    }
// }

// bool MeldServices::meldCanNeglectMeldPile(std::vector<Card> handPile, std::vector<Card> meldCards) {
//    //if any card in the given meld is not in hand pile, return false
//    //Assumption: meld sent by user is a valid meld. Can use isValidMeld() to check
//    //Given our assumption, we know that no card is repeated in the given meld

//    bool cardIsInHandPile;
//    //loop through each card in meld
//    for(int i = 0; i < meldCards.size(); i++) {
//       //loop through handPile to check if card is in the hand pile
//       cardIsInHandPile = false; 
//       for(int j = 0; j < handPile.size(); j++) {
//          if (meldCards[i] == handPile[j]) {
//             cardIsInHandPile = true;
//          }
//       }
//       //if the card is missing, return false
//       if(!cardIsInHandPile) {
//          return false;
//       }
//    }
//    return true;
// }

// bool MeldServices::meldHasCardFromHandPile(std::vector<Card> handPile, std::vector<Card> meldCards) {
//    //if any card in the given meld is in hand pile, return true

//    //loop through each card in meld
//    for(int i = 0; i < meldCards.size(); i++) {
//       //loop through handPile to check if card is in the hand pile
//       for(int j = 0; j < handPile.size(); j++) {
//          if (meldCards[i] == handPile[j]) {
//             return true;
//          }
//       }
//    }
//    return false;

// }

// bool MeldServices::isValidMeld(std::vector<Card> cards, Meld* whatMeld) {
//    //if the given cards do not create a valid meld, then whatMeld is returned with a NULL value
//    whatMeld = NULL;
//    //if the number of cards does not correspond to any possible meld, return false
//    if(cards.size() < 1 || cards.size() == 3 || cards.size() > 5) {
//       return false;
//    }

//    if(!trumpSuitSpecified)
//    {
//       throw PinochleException("Trump suit must be specified before checking valiity of meld");
//    }

//    //checking melds from most common to least common

//    //check if Dix
//    if(isDix(cards)) {
//       *whatMeld = Meld::Dix;
//       return true;
//    }

//    //check if Marriage
//    if(isAnyMarriage(cards)) {
//       *whatMeld = typeOfMarriage(cards);
//       return true;
//    }

//    //check if Pinochle
//    if(isPinochle(cards)) {
//       *whatMeld = Meld::Pinochle;
//       return true;
//    }

//    //check if Four Aces
//    if(cards[0].getRank() == Rank::Ace && isFours(cards)) {
//       *whatMeld = Meld::FourAces;
//       return true;
//    }

//    //check if Four Kings
//    if(cards[0].getRank() == Rank::King && isFours(cards)) {
//       *whatMeld = Meld::FourKings;
//       return true;
//    }

//    //check if Four Queens
//    if(cards[0].getRank() == Rank::Queen && isFours(cards)) {
//       *whatMeld = Meld::FourQueens;
//       return true;
//    }

//    //check if Four Jacks
//    if(cards[0].getRank() == Rank::Jack && isFours(cards)) {
//       *whatMeld = Meld::FourJacks;
//       return true;
//    }

//    //check if Flush
//    if(isFlush(cards)) {
//       *whatMeld = Meld::Flush;
//       return true;
//    }

//    //if none of the Melds match
//    return false;

// }

// bool MeldServices::isDix(std::vector<Card> cards) {
//    //Note: A Dix is a card of rank Nine and a suit the same as the trump suit

//    //check size
//    if(cards.size() != 1) {
//       return false;
//    }

//    //if card is not of rank Nine or not of trump suit, return false
//    if(cards[0].getRank() != Rank::Nine || cards[0].getSuit() != trumpSuit) {
//       return false;
//    }
//    return true;
// }

// bool MeldServices::isAnyMarriage(std::vector<Card> cards) {

//    //check size of meld
//    if(cards.size() != 2) {
//       return false;
//    }

//    //If cards are not of the same suit, return false
//    if(cards[0].getSuit() != cards[1].getSuit()) {
//       return false;
//    }

//    //if one card is a King and the other a Queen, return true
//    if(cards[0].getRank() == Rank::King && cards[1].getRank() == Rank::Queen) {
//       return true;
//    }
//    if(cards[1].getRank() == Rank::King && cards[0].getRank() == Rank::Queen) {
//       return true;
//    }

//    //if none of the conditions match return false
//    return false;
// }

// Meld MeldServices::typeOfMarriage(std::vector<Card> cards) {
//    //Note: A Marriage contains a King and Queen of any other suit besides the Trump suit

//    if(!isAnyMarriage(cards)) {
//       throw PinochleException("Cards must be a type of marriage");
//    }
   
//    //If first card is trump suit, return false (only first card is checked because both cards are the same suit)
//    //ensured by isAnyMarriage
//    if(cards[0].getSuit() == trumpSuit) {
//       return Meld::RoyalMarriage;
//    } else {
//       //return appropriate marriage
//       switch(cards[0].getSuit()) {
//          case(Suit::Clubs):
//             return Meld::MarriageClubs;
//             break;
//          case(Suit::Diamonds):
//             return Meld::MarriageHearts;
//             break;
//          case(Suit::Hearts):
//             return Meld::MarriageHearts;
//             break;
//          case(Suit::Spades):
//             return Meld::MarriageSpades;
//             break;
//          default:
//             throw PinochleException("Undefined suit type for card");
//             break;
//       } 
//    }

// }

// // bool MeldServices::isRoyalMarriage(std::vector<Card> cards) {
// //    //Note: A Royal Marriage contains a King and Queen of the Trump suit

// //    if(!isAnyMarriage(cards)) {
// //       return false;
// //    }
   
// //    //If first card is trump suit, return false (only first card is checked because both cards are the same suit)
// //    //ensured by isAnyMarriage   
// //    if(cards[0].getSuit() != trumpSuit) {
// //       return false;
// //    }

// //    return false;
// // }

// bool MeldServices::isPinochle(std::vector<Card> cards) {
//    //Note: A flush contains Queen of Spades and Jack of Diamonds 
   
//    //check size
//    if(cards.size() != 2) {
//       return false;
//    } 

//    //if one card is queen of spades and the other is jack of diamonds, return true
//    if(cards[0].getRank() == Rank::Queen && cards[0].getSuit() == Suit::Spades) {
//       if(cards[1].getRank() == Rank::Jack && cards[1].getSuit() == Suit::Diamonds) {
//          return true;
//       }
//    }
//    //checking for vice versa 
//    if(cards[1].getRank() == Rank::Queen && cards[1].getSuit() == Suit::Spades) {
//       if(cards[0].getRank() == Rank::Jack && cards[0].getSuit() == Suit::Diamonds) {
//          return true;
//       }
//    }
//    return false;
// }



// bool MeldServices::isFours(std::vector<Card> cards) {
//    //Note: Fours are types of melds consisting of Four melds of the same Rank but of different suits
//    //These melds are: Four Aces, Four Kings, Four Queens, and Four Jacks
//    //This general function checks only checks whether it is a Fours type of meld, 
//    //it does not care what the specific meld is

//    //check card size
//    if(cards.size() != 4) {
//       return false;
//    }
//    //we are comparing the ranks of the remaining three card with that of the first card
//    Rank whatRank = cards[0].getRank();
   
//    //this array of flags is used to keep track of the different suits encountered 
//    //as we go through the vector of cards
//    //it tells us if any suit has been repeated, in which case, it is not a Fours type meld 
//    //and so, we can return false
//    //flag[0] corresponds to Clubs, flag[1] to Diamonds, flag[2] to Hearts, and flag[3] to Spades (based
//    //on the order of enums as listed in Card.h)
//    //all flags set fo false initially
//    bool flags[4] = {false, false, false, false};

//    //loop once for each card
//    for(int i = 0; i < 4; i++) {
//       //if rank doesn't match in a card, return false
//       if(cards[i].getRank() != whatRank) {
//          return false;
//       }

//       //if the suit has already been encountered before, return false
//       if(flags[static_cast<int>(cards[i].getSuit())] == true) {
//          return false;
//       } 

//       //switch flag
//       flags[static_cast<int>(cards[i].getSuit())] == !flags[static_cast<int>(cards[i].getSuit())];
//    }
//    //if all the cards are of the same rank and all their suits are unique, it is a Fours meld
//    return true;

// }

// bool MeldServices::isFlush(std::vector<Card> cards) {
//    //checking if the meld is a Flush
//    //Note: A flush contains five cards: Ace, Ten, King, Queen, and Jack, all of Trump suit
   
//    //check size of meld
//    if(cards.size() != 5) {
//       return false;
//    }
//    //This array of flags will keep track of the cards encountered in this 
//    //group of cards. If a card of trump suit is encountered, corresponding flag is switched.
//    //flag[0] corresponds to Nine, flag[1] to Joker, flag[2] to Queen, and 
//    //so on in ascending order based on the order of enums as listed in Card.h
//    //All flags except flag[0] set to false initially, because Nine (represented by flag[0]) is not part of the Flush meld
//    //If a card of non-trump suit is encountered, directly return false
   
//    bool flags[6] = {true, false, false, false, false, false};
   
//    //loop once for each card
//    for(int i = 0; i < 5; i++) {
//       //if this card is not of the trump suit
//       if(cards[i].getSuit() != trumpSuit) {
//          return false;
//       }
//       //if the card has already been encountered before in this group of cards (or if this card is a Nine card)
//       //then return false
//       if (flags[static_cast<int>(cards[i].getRank())] == true) {
//          return false;
//       }

//       //if this card is of the trump suit, hasn't been encountered before, and if it is not a Nine card, 
//       //then switch flag
//       flags[static_cast<int>(cards[i].getRank())] = !flags[static_cast<int>(cards[i].getRank())];
//    }
//    //if all the cards are unique, all are of trump suit, and none of them are of rank Nine, then  
//    //the cards make up a Flush meld. 
//    return true;
// }