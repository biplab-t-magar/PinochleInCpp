#include "MeldServices.h"
#include <algorithm>

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

bool MeldServices::playMeld(std::vector<Card> cardsToBePlayed, std::vector<Card>* handPile, std::vector<Card>* meldPile) {
   Meld whatMeld;
   if(!isValidMeld(cardsToBePlayed, &whatMeld)) {
      return false;
   }

   //if at least one meld of given meld is possible in the player's entire hand
   if(countMeldsFromHand(*handPile, *meldPile)[static_cast<int>(whatMeld)] <= 0) {
      return false;
   } 
   //marriages can be of 3 different types, for each 3 non-trump suits
   //check if that specific marriage provided is present in hand
   if(whatMeld == Meld::Marriage) {
      //count how many of the marraig
      if(howManyMarriages(cardsToBePlayed[0].getSuit(), *handPile, *meldPile) <= 0) {
         return false;
      }
   }

   //to keep track of the cards in handPile and meldPile
   std::vector<bool> isCardInMeldPile;
   std::vector<bool> isCardInHandPile;
   //initialize all indexes to false
   for(int i = 0; i < cardsToBePlayed.size(); i++) {
      isCardInMeldPile[i] = false;
      isCardInHandPile[i] = false;
   }

   //loop through all cards
   for(int i = 0; i < cardsToBePlayed.size(); i++) {
      //check if the card is in the meld pile
      for(int j = 0; j < (*meldPile).size(); j++) {
         if(cardsToBePlayed[i] == (*meldPile)[j]) {
            isCardInMeldPile[i] = true;
            break;
         }
      }
      //check if the card is in the hand pile
      for(int j = 0; j < (*handPile).size(); j++) {
         if(cardsToBePlayed[i] == (*handPile)[j]) {
            isCardInHandPile[i] = true;
            break;
         }
      }
   }

   //variable to indicate what cards from handPile should be transferred to meldPile to create the meld
   std::vector<Card> whatCardsToTransfer;

   //finding out what cards are not in meldPile 
   for(int i = 0; i < cardsToBePlayed.size(); i++) {
      //if the card is not in the meld pile, then we need to transfer that card from the handPile to meldpile to create the meld
      if(isCardInMeldPile[i] == false) {
          //keeping track of the card once not found in the meldPile
          //this is the card to be sought in the handPile 
         whatCardsToTransfer.push_back(cardsToBePlayed[i]);
      } 
   }
   //if by any chance all the needed cards were in the meld pile already:
   if(whatCardsToTransfer.size() == 0) {
      //first check if the meld has been played before in the round already.
      if(!meldTypePlayedFirstTime) {
         //if it has been played already, search for all the cards in the handPile and do not search for any in the meld pile
         //finding what index of the handPile is each card in
         for(int i = 0; i < cardsToBePlayed.size(); i++) {
            for(int j = 0; j < (*handPile).size(); j++) {
               if(cardsToBePlayed[i] == (*handPile)[j]) {
                  whatCardsToTransfer.push_back(cardsToBePlayed[i]);
                  break;
               }
            }
         }
      //if not all the cards are found in the handPile (although they should be found there
      //because we have checked that previously already), return false
         if(cardsToBePlayed.size() != whatCardsToTransfer.size()) {
            return false;
         }

      }
      //if the meld has not been played in the round before
      //we need to selectively look for one eligible card to bring from the handPile
      else {   
         //get a list of all the cards  in the handPile that are eligible for the meld
         std::vector<Card> eligibleHandCards;
         for(int i = 0; i < cardsToBePlayed.size(); i++) {
            if(isCardInHandPile[i]) {
               eligibleHandCards.push_back(cardsToBePlayed[i]);
            }
         }
         //create two mock handPiles and meldPiles so that we can know what card will bring about the
         //the better situation in future melds 
         std::vector<Card> idealHandPile = *handPile;
         std::vector<Card> idealMeldPile = *meldPile;
         std::vector<Card> contestantHandPile = *handPile;
         std::vector<Card> contestantMeldPile = *meldPile;

         //now, we hypothetically transfer each card in handPile to meldPile to create 
         //as many prospective handPile/meldPile pairs as there are and compare them to each other
         moveCardToMeldPile(&idealHandPile, &idealMeldPile, eligibleHandCards[0]);
         whatCardsToTransfer.push_back(eligibleHandCards[0]);
         for(int i = 1; i < eligibleHandCards.size(); i++) {
            moveCardToMeldPile(&contestantHandPile, &contestantMeldPile, eligibleHandCards[i]);
            //now compare prospective ideal hand and contestant hand
            //if the latter has better prospects, make that hand into ideal hand
            if(compareHands(idealHandPile, idealMeldPile, contestantHandPile, contestantMeldPile) == 2) {
               idealHandPile = contestantHandPile;
               idealMeldPile = contestantMeldPile;
               //push that card which, when pushed to the meld would make the ideal hand, into the 
               //emptied whatCardsToTransfer vector
               whatCardsToTransfer.clear();
               whatCardsToTransfer.push_back(eligibleHandCards[i]);
            } 
         }
      }
   }
   //now we have a list of all the cards we need to transfer from hand pile to meld pile to 
   //complete the playing of the meld
   return moveCardsToMeldPile(handPile, meldPile, whatCardsToTransfer);


}


bool MeldServices::moveCardToMeldPile(std::vector<Card>* handPile, std::vector<Card>* meldPile, Card card){
   std::vector<Card> cards;
   cards.push_back(card);
   return moveCardsToMeldPile(handPile, meldPile, cards);
}

bool MeldServices::moveCardsToMeldPile(std::vector<Card>* handPile, std::vector<Card>* meldPile, std::vector<Card> cards) {
   int initHandPileSize = (*handPile).size();
   //all cards are looped through 
   for(int i = 0; i < cards.size(); i++) {
      //look for that card in handPile
      for(int j = 0; j < (*handPile).size(); j++) {
         if((*handPile)[j] == cards[i]) {
            (*meldPile).push_back(cards[i]);
            (*handPile).erase((*handPile).begin() + j);
         } 
      }
   }
   //if all cards were found and moved to meld pile, return true
   if((initHandPileSize - (*handPile).size()) == cards.size()) {
      return true;
   } else {
      return false;
   }
}

int MeldServices::compareHands(std::vector<Card> handPile1, std::vector<Card>meldPile1, std::vector<Card> handPile2, std::vector<Card> meldPile2) {
   //get the points (from highest to lowest) for each possible meld in the two candidate hands

   std::vector<int> hand1Points = potentialPointsFromHand(handPile1, meldPile1);
   std::vector<int> hand2Points = potentialPointsFromHand(handPile2, meldPile2);
   
   //first, check which hand has the meld that provides the highest points
   if (hand1Points[0] > hand2Points[0]) {
      return 1;
   } else if (hand1Points[0] > hand2Points[0]) {
      return 2;
   } 
   //if the highest points of each is the same, compare the total points produced by all the melds of each hand
   int totalHand1Points, totalHand2Points = 0;
   for(int i = 0; i < hand1Points.size(); i++) {
      totalHand1Points+= hand1Points[i];
   }
   for(int i = 0; i < hand2Points.size(); i++) {
      totalHand1Points+= hand2Points[i];
   }
   if(totalHand1Points > totalHand2Points) {
      return 1;
   } else if(totalHand2Points > totalHand1Points) {
      return 2;
   }

   //if again the total possible points of each hand are the same, compare the 2nd highest, 3rd highest,...
   //and so on points of each meld until a winner is found
   int i = 0;
   while(i < hand1Points.size() && i < hand2Points.size()) {
      if (hand1Points[i] > hand2Points[i]) {
         return 1;
      } else if (hand1Points[i] > hand2Points[i]) {
         return 2;
      } 
   }

   //if no difference between the possible points from each hand was found, return 0
   return 0;
}

std::vector<int> MeldServices::potentialPointsFromHand(std::vector<Card> handPile, std::vector<Card> meldPile) {
   std::vector<int> handMelds = countMeldsFromHand(handPile, meldPile);
   //now, get the vector of points for each possible meld in the hand
   std::vector<int> points;
   for(int i = 0 ; i < handMelds.size(); i++) {
      //loop through each instance of all the potential melds (there may be 2 or more instances of the same meld in the hand)
      for(int  j = 0; j < handMelds[i]; j++) {
         points.push_back(getMeldPoints(static_cast<Meld>(handMelds[i])));
      }
   }
   //sort the vector
   std::sort(points.begin(), points.end(), std::greater<>()); 
   return points;
}

std::vector<int> MeldServices::countMeldsFromHand(std::vector<Card> handPile, std::vector<Card> meldPile) {
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified for this round. Use setTrumpSuit() to specify trump suit");
   }
   
   //vector to hold each meld type (9 in total)
   std::vector<int> numOfEachMeld;
   //initialize all values in vector to 0
   for(int i = 0; i < 9; i++) {
      numOfEachMeld.push_back(0);
   }

   //now push the number of possible instances of each meld
   numOfEachMeld[static_cast<int>(Meld::Dix)] = howManyDixes(handPile);
   numOfEachMeld[static_cast<int>(Meld::Flush)] = countSameSuitMelds(Meld::Flush, handPile, meldPile, trumpSuit, Rank::Ace, 5);
   //add marriages
   for(int i = 0; i < 4; i++) {
      if(static_cast<Suit>(i) == trumpSuit) {
         numOfEachMeld[static_cast<int>(Meld::RoyalMarriage)] = howManyMarriages(static_cast<Suit>(i), handPile, meldPile);
      } else {
         numOfEachMeld[static_cast<int>(Meld::Marriage)] += howManyMarriages(static_cast<Suit>(i), handPile, meldPile);
      }
   }
   numOfEachMeld[static_cast<int>(Meld::FourAces)] = countSameRankMelds(Meld::FourAces, handPile, meldPile, Rank::Ace);
   numOfEachMeld[static_cast<int>(Meld::FourJacks)] = countSameRankMelds(Meld::FourAces, handPile, meldPile, Rank::Jack);
   numOfEachMeld[static_cast<int>(Meld::FourKings)] = countSameRankMelds(Meld::FourAces, handPile, meldPile, Rank::King);
   numOfEachMeld[static_cast<int>(Meld::FourQueens)] = countSameRankMelds(Meld::FourAces, handPile, meldPile, Rank::Queen);
   numOfEachMeld[static_cast<int>(Meld::FourAces)] = howManyPinochles(handPile, meldPile);

   return numOfEachMeld;

}

int MeldServices::howManyDixes(std::vector<Card> handPile) {
    //since dix is a single card Meld, we need to only search for it in the handPile and not the meldPile
   //because any dix found in the meld pile has already been used and is therefore invalid

   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified for this round. Use setTrumpSuit() to specify trump suit");
   }

   int count = 0;
   //loop through the handPile 
   for(int i = 0; i < handPile.size(); i++) {
      //if any card is a 9 of Trump card, add it into the result
      if(handPile[i].getRank() == Rank::Nine && handPile[i].getSuit() == trumpSuit) {
         count++;
      }
   }
   return count;
}

int MeldServices::howManyPinochles(std::vector<Card> handPile, std::vector<Card> meldPile) {
   int queenOfSpadesCount = 0;
   int jackOfDiamondsCount = 0;

   //first, look through handPile
   for(int i = 0; i < handPile.size(); i++) {
      if(handPile[i].getRank() == Rank::Jack && handPile[i].getSuit() == Suit::Diamonds) {
         jackOfDiamondsCount++;
      } else if (handPile[i].getRank() == Rank::Queen && handPile[i].getSuit() == Suit::Spades) {
         queenOfSpadesCount++;
      }
   }
   //if card meld has not been played already, look at meldPile too
   if(meldTypePlayedFirstTime(Meld::Pinochle)) {
      for(int i = 0; i < meldPile.size(); i++) {
         if(meldPile[i].getRank() == Rank::Jack && meldPile[i].getSuit() == Suit::Diamonds) {
            jackOfDiamondsCount++;
         } else if (meldPile[i].getRank() == Rank::Queen && meldPile[i].getSuit() == Suit::Spades) {
            queenOfSpadesCount++;
         }
      }
   }
   if (jackOfDiamondsCount <= queenOfSpadesCount) {
      return jackOfDiamondsCount;
   } else {
      return queenOfSpadesCount;
   }
   
}

int MeldServices::howManyMarriages(Suit suit, std::vector<Card> handPile, std::vector<Card> meldPile) {
   if(suit == trumpSuit) {
      return countSameSuitMelds(Meld::RoyalMarriage, handPile, meldPile, suit, Rank::King, 2);
   } else {
      return countSameSuitMelds(Meld::Marriage, handPile, meldPile, suit, Rank::King, 2);
   }
   
}


int MeldServices::countSameSuitMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Suit suit, Rank startingRank, int howManyCards) {
   
   //if the number in howManyCards is more than the number of Ranks in the meld
   if((static_cast<int>(startingRank) - howManyCards + 1) <= 0 ) {
      throw PinochleException("Invalid starting rank or number of cards");
   }
   if(howManyCards <= 0) {
      throw PinochleException("There cannot be a meld with 0 cards");
   }
   //this vector counts the number of each rank of the specified suit
   //only ranks relevant to the meld are counted
   std::vector<int> howManyOfEachRank;
   //this vectors holds the count of each rank of the specified suit that was found in the meld pile
   // std::vector<int> howManyOfEachRankFromMeldPile;
   //holds the current rank (in int form) being counted 
   int rank = static_cast<int>(startingRank);

   bool meldHasBeenPlayedBefore = !meldTypePlayedFirstTime(meld);

   //loop through all the ranks in the meld
   for(int i = 0; i < howManyCards; i++) {
      //start counting number of current rank, with count initialized to 0
      howManyOfEachRank.push_back(0);
      //start counting number of current rank taken from meld pile, with count initialized to 0
      // howManyOfEachRankFromMeldPile.push_back(0);

      //loop through the hand pile
      for(int j = 0; j <  handPile.size(); j++) {
         //if both rank and suit match
         if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
            howManyOfEachRank[i]++;
         }
      }
      //loop through the meld pile only if this meld has not been previously created by player
      // if(!meldHasBeenPlayedBefore) {
      //    for(int j = 0; j <  meldPile.size(); j++) {
      //    //if both rank and suit match
      //       if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
      //          howManyOfEachRank[i]++;
      //          howManyOfEachRankFromMeldPile[i]++;
      //       }
      //    }
      // }
      //go to the next rank of the meld
      rank--;
   }
   for(int i = 0; i < howManyCards; i++) {
      if(!meldHasBeenPlayedBefore) {
         for(int j = 0; j <  meldPile.size(); j++) {
         //if both rank and suit match
            if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
               howManyOfEachRank[i]++;
               // howManyOfEachRankFromMeldPile[i]++;
            }
         }
      }
      rank--;
   }


   //now the number of *mutually exclusive* possible melds is calculated
   //the number of that constituent rank of the meld which occurs the least times in the meld is the actual 
   //number of mutually exclusive possible melds

   //finding minimum value in howManyOfEachRank
   int min = howManyOfEachRank[0];
   for(int i = 1; i < howManyOfEachRank.size(); i++) {
      if(min > howManyOfEachRank[i]) {
         min = howManyOfEachRank[i];
      } 
   }
   return min;
}

int MeldServices::countSameRankMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Rank rank) {
   //this vector counts the number of each suit of the specified rank
   std::vector<int> howManyOfEachSuit;
   //this vectors holds the count of each rank of the specified suit that was found in the meld pile
   // std::vector<int> howManyOfEachSuitFromMeldPile;
   //holds the current suit (in int form) being counted (always start with clubs and increase to other suits from there)
   int suit = 0;

   bool meldHasBeenPlayedBefore = !meldTypePlayedFirstTime(meld);


   //loop through all the suits in the meld
   for(int i = 0; i < 4; i++) {
      //start counting number of current rank, with count initialized to 0
      howManyOfEachSuit.push_back(0);
      //start counting number of current rank taken from meld pile, with count initialized to 0
      // howManyOfEachSuitFromMeldPile.push_back(0);
      //loop through the hand pile
      for(int j = 0; j <  handPile.size(); j++) {
         //if both rank and suit match
         if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
            howManyOfEachSuit[i]++;
         }
      }
      //go to the next rank of the meld
      suit++;
   }
   //now do the same for the meld pile
   suit = 0;
   for(int i = 0; i < 4; i++) {
      //loop through the meld pile only if this meld has not been played by player before
      if(!meldHasBeenPlayedBefore) {
         for(int j = 0; j <  meldPile.size(); j++) {
            //if both rank and suit match
            if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
               howManyOfEachSuit[i]++;
               // howManyOfEachSuitFromMeldPile[i]++;
            }
         }
      }
      suit++;
   }
   //finding minimum value in howManyOfEachSuit
   //any card with occurence more than that minimum value has extraneous cards
   //which represent choices we can make when we create melds
   //min represents number of mutually exclusive possible melds
   int min = howManyOfEachSuit[0];
   for(int i = 1; i < howManyOfEachSuit.size(); i++) {
      if(min > howManyOfEachSuit[i]) {
         min = howManyOfEachSuit[i];
      } 
   }
   

   return min;
}


// int MeldServices::removeExtraneousChoices(Meld meld, std::vector<int> howManyOfEachCard, std::vector<int> howManyFromMeldPile) {
//    if(howManyOfEachCard.size() != howManyFromMeldPile.size()) {
//       throw PinochleException("howManyOfEachCard and howManyFromMeldPile must have same size");
//    }
//    if(howManyOfEachCard.size() <= 0) {
//       throw PinochleException("Arguments cannot be empty vectors");
//    }
//    //finding minimum value in howManyOfEachCard
//    //any card with occurence more than that minimum value has extraneous cards
//    //which represent choices we can make when we create melds
//    //min represents number of mutually exclusive possible melds
//    int mutuallyExclusivePossibleMelds = howManyOfEachCard[0];
//    for(int i = 1; i < howManyOfEachCard.size(); i++) {
//       if(mutuallyExclusivePossibleMelds > howManyOfEachCard[i]) {
//          mutuallyExclusivePossibleMelds = howManyOfEachCard[i];
//       } 
//    }

//    //Despite choices, the possible number of mutually exclusive melds is not affected.
//    //So, we can eliminate these extraneous choices 
   
//    //If any extraneous choices are in meld pile we want to remove from the meldPile because choosing 
//    //from the meldPile might make an invalid meld
//    //This explains the decision below to subtract choices from the meldPile as we eliminate choices

//    //loop through all cards and delete extraneous occurrences (choices) of each card from vector 
//    //representing the meldPile and from the vector representing the entire hand
//    // for(int i = 0; i < howManyOfEachCard.size(); i++) {
//    //    howManyFromMeldPile[i] = howManyFromMeldPile[i] - (howManyOfEachCard[i] - mutuallyExclusivePossibleMelds);
//    //    howManyOfEachCard[i] = mutuallyExclusivePossibleMelds;
//    // }  
//    // //Now, for each possible occurence of the meld, we want to eliminate those that 
//    // //use cards from the meldPile if the meld has already been used by the user
//    // for(int i = 0; i < howManyOfEachCard.size(); i++) {
//    //    if(howManyFromMeldPile[i] > 0 && !meldTypePlayedFirstTime(meld)) {
//    //       //removing choices decreases the card with the least occurences
//    //       //and thus decreases the possible mutually exclusive melds
//    //       if(mutuallyExclusivePossibleMelds > (howManyOfEachCard[i] - howManyFromMeldPile[i])) {
//    //          mutuallyExclusivePossibleMelds = howManyOfEachCard[i] - howManyFromMeldPile[i];
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


bool MeldServices::meldTypePlayedFirstTime(Meld meld) {
   //loop through all melds that have been previously played by the player
   for(int i = 0; i < meldsPlayed.size(); i++) {
      //if matching meld found, return false
      if(meldsPlayed[i] == meld) {
         return false;
      }
   }
   return true;
}

bool MeldServices::meldCanNeglectMeldPile(std::vector<Card> handPile, std::vector<Card> meld) {
   //if any card in the given meld is not in hand pile, return false
   //Assumption: meld sent by user is a valid meld. Can use isValidMeld() to check
   //Given our assumption, we know that no card is repeated in the given meld

   bool cardIsPresent;
   //loop through each card in meld
   for(int i = 0; i < meld.size(); i++) {
      //loop through handPile to check if card is in the hand pile
      cardIsPresent = false; 
      for(int j = 0; j < handPile.size(); j++) {
         if (meld[i].getSuit() == handPile[j].getSuit() && meld[i].getRank() == handPile[j].getRank()) {
            cardIsPresent = true;
         }
         //if the card is missing, return false
         if(!cardIsPresent) {
            return false;
         }
      }
   }
   return true;
}

bool MeldServices::meldHasCardFromHandPile(std::vector<Card> handPile, std::vector<Card> meld) {
   //if any card in the given meld is in hand pile, return true

   //loop through each card in meld
   for(int i = 0; i < meld.size(); i++) {
      //loop through handPile to check if card is in the hand pile
      for(int j = 0; j < handPile.size(); j++) {
         if (meld[i].getSuit() == handPile[j].getSuit() && meld[i].getRank() == handPile[j].getRank()) {
            return true;
         }
      }
   }
   return false;

}

bool MeldServices::isValidMeld(std::vector<Card> cards, Meld* whatMeld) {
   //if the given cards do not create a valid meld, then whatMeld is returned with a NULL value
   whatMeld = NULL;
   //if the number of cards does not correspond to any possible meld, return false
   if(cards.size() < 1 || cards.size() == 3 || cards.size() > 5) {
      return false;
   }

   //checking melds from most common to least common

   //check if Dix
   if(isDix(cards)) {
      *whatMeld = Meld::Dix;
      return true;
   }

   //check if Marriage
   if(isMarriage(cards)) {
      *whatMeld = Meld::Marriage;
      return true;
   }

   //check if Royal Marriage
   if(isRoyalMarriage(cards)) {
      *whatMeld = Meld::RoyalMarriage;
      return true;
   }

   //check if Pinochle
   if(isPinochle(cards)) {
      *whatMeld = Meld::Pinochle;
      return true;
   }

   //check if Four Aces
   if(cards[0].getRank() == Rank::Ace && isFours(cards)) {
      *whatMeld = Meld::FourAces;
      return true;
   }

   //check if Four Kings
   if(cards[0].getRank() == Rank::King && isFours(cards)) {
      *whatMeld = Meld::FourKings;
      return true;
   }

   //check if Four Queens
   if(cards[0].getRank() == Rank::Queen && isFours(cards)) {
      *whatMeld = Meld::FourQueens;
      return true;
   }

   //check if Four Jacks
   if(cards[0].getRank() == Rank::Jack && isFours(cards)) {
      *whatMeld = Meld::FourJacks;
      return true;
   }

   //check if Flush
   if(isFlush(cards)) {
      *whatMeld = Meld::Flush;
      return true;
   }

   //if none of the Melds match
   return false;

}

bool MeldServices::isDix(std::vector<Card> cards) {
   //Note: A Dix is a card of rank Nine and a suit the same as the trump suit

   //check size
   if(cards.size() != 1) {
      return false;
   }

   //if card is not of rank Nine or not of trump suit, return false
   if(cards[0].getRank() != Rank::Nine || cards[0].getSuit() != trumpSuit) {
      return false;
   }
   return true;
}

bool MeldServices::isAnyMarriage(std::vector<Card> cards) {

   //check size of meld
   if(cards.size() != 2) {
      return false;
   }

   //If cards are not of the same suit, return false
   if(cards[0].getSuit() != cards[1].getSuit()) {
      return false;
   }

   //if one card is a King and the other a Queen, return true
   if(cards[0].getRank() == Rank::King && cards[1].getRank() == Rank::Queen) {
      return true;
   }
   if(cards[1].getRank() == Rank::King && cards[0].getRank() == Rank::Queen) {
      return true;
   }

   //if none of the conditions match return false
   return false;
}

bool MeldServices::isMarriage(std::vector<Card> cards) {
   //Note: A Marriage contains a King and Queen of any other suit besides the Trump suit

   if(!isAnyMarriage(cards)) {
      return false;
   }
   
   //If first card is trump suit, return false (only first card is checked because both cards are the same suit)
   //ensured by isAnyMarriage
   if(cards[0].getSuit() == trumpSuit) {
      return false;
   }

   return false;

}

bool MeldServices::isRoyalMarriage(std::vector<Card> cards) {
   //Note: A Royal Marriage contains a King and Queen of the Trump suit

   if(!isAnyMarriage(cards)) {
      return false;
   }
   
   //If first card is trump suit, return false (only first card is checked because both cards are the same suit)
   //ensured by isAnyMarriage   
   if(cards[0].getSuit() != trumpSuit) {
      return false;
   }

   return false;
}

bool MeldServices::isPinochle(std::vector<Card> cards) {
   //Note: A flush contains Queen of Spades and Jack of Diamonds 
   
   //check size
   if(cards.size() != 2) {
      return false;
   } 

   //if one card is queen of spades and the other is jack of diamonds, return true
   if(cards[0].getRank() == Rank::Queen && cards[0].getSuit() == Suit::Spades) {
      if(cards[1].getRank() == Rank::Jack && cards[1].getSuit() == Suit::Diamonds) {
         return true;
      }
   }
   //checking for vice versa 
   if(cards[1].getRank() == Rank::Queen && cards[1].getSuit() == Suit::Spades) {
      if(cards[0].getRank() == Rank::Jack && cards[0].getSuit() == Suit::Diamonds) {
         return true;
      }
   }
   return false;
}



bool MeldServices::isFours(std::vector<Card> cards) {
   //Note: Fours are types of melds consisting of Four melds of the same Rank but of different suits
   //These melds are: Four Aces, Four Kings, Four Queens, and Four Jacks
   //This general function checks only checks whether it is a Fours type of meld, 
   //it does not care what the specific meld is

   //check card size
   if(cards.size() != 4) {
      return false;
   }
   //we are comparing the ranks of the remaining three card with that of the first card
   Rank whatRank = cards[0].getRank();
   
   //this array of flags is used to keep track of the different suits encountered 
   //as we go through the vector of cards
   //it tells us if any suit has been repeated, in which case, it is not a Fours type meld 
   //and so, we can return false
   //flag[0] corresponds to Clubs, flag[1] to Diamonds, flag[2] to Hearts, and flag[3] to Spades (based
   //on the order of enums as listed in Card.h)
   //all flags set fo false initially
   bool flags[4] = {false, false, false, false};

   //loop once for each card
   for(int i = 0; i < 4; i++) {
      //if rank doesn't match in even a single card, return false
      if(cards[i].getRank() != whatRank) {
         return false;
      }

      //if the suit has already been encountered before, return false
      if(flags[static_cast<int>(cards[i].getSuit())] == true) {
         return false;
      } 

      //switch flag
      flags[static_cast<int>(cards[i].getSuit())] == !flags[static_cast<int>(cards[i].getSuit())];
   }
   //if all the cards are of the same rank and all their suits are unique, it is a Fours meld
   return true;

}

bool MeldServices::isFlush(std::vector<Card> cards) {
    //checking if the meld is a Flush
    //Note: A flush contains five cards: Ace, Ten, King, Queen, and Jack, all of Trump suit
    
    //check size of meld
    if(cards.size() != 5) {
        return false;
    }
    //This array of flags will keep track of the cards encountered in this 
    //group of cards. If a card of trump suit is encountered, corresponding flag is switched.
    //flag[0] corresponds to Nine, flag[1] to Joker, flag[2] to Queen, and 
    //so on in ascending order based on the order of enums as listed in Card.h
    //All flags except flag[0] set to false initially, because Nine (represented by flag[0]) is not part of the Flush meld
    //If a card of non-trump suit is encountered, directly return false
    
    bool flags[6] = {true, false, false, false, false, false};
    
    //loop once for each card
    for(int i = 0; i < 5; i++) {
        //if this card is not of the trump suit
        if(cards[i].getSuit() != trumpSuit) {
            return false;
        }
        //if the card has already been encountered before in this group of cards (or if this card is a Nine card)
        //then return false
        if (flags[static_cast<int>(cards[i].getRank())] == true) {
            return false;
        }

        //if this card is of the trump suit, hasn't been encountered before, and if it is not a Nine card, 
        //then switch flag
        flags[static_cast<int>(cards[i].getRank())] = !flags[static_cast<int>(cards[i].getRank())];
    }
    //if all the cards are unique, all are of trump suit, and none of them are of rank Nine, then  
    //the cards make up a Flush meld. 
    return true;
}