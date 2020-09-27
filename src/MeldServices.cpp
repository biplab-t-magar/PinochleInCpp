#include "MeldServices.h"
#include <algorithm>

#define numOfEachCard 2
#define numOfMeldTypes 9
#define numOfSuits 4

MeldServices::MeldServices() {
   this->trumpSuitSpecified = false;
}

MeldServices::MeldServices(MeldsStorage meldsPlayed, Suit trumpSuit) {
   this->trumpSuit = trumpSuit;
   this->trumpSuitSpecified = true;
   this->meldsPlayed = meldsPlayed;
}

MeldsStorage MeldServices::getMeldsPlayed() const {
   return meldsPlayed;
}

bool MeldServices::setTrumpSuit(Suit trumpSuit) {
   if(static_cast<int>(trumpSuit) >= numOfSuits || static_cast<int>(trumpSuit) < 0 ) {
      throw PinochleException("invalid trump suit");
   }
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

bool MeldServices::meldHasANewCard(MeldInstance meldInstance) {
   return !meldsPlayed.cardsUsedForSameMeld(meldInstance);
}

bool MeldServices::storeMeld(GroupOfCards hand, MeldInstance meldInstance) {
   if(!meldInstance.isValidMeld() || !allCardsPresentInHand(hand, meldInstance) || !meldIsNotARepeat(meldInstance)) {
      return false;
   }
   meldsPlayed.addMeld(meldInstance);
   return true;  
}

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
   int totalHand1Points = 0;
   int totalHand2Points = 0;
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
   //get the size of the smaller of the two vectors
   int size = hand1Points.size() < hand2Points.size() ? hand1Points.size() : hand2Points.size();
   while(i < size) {
      if (hand1Points[i] > hand2Points[i]) {
         return 1;
      } else if (hand1Points[i] < hand2Points[i]) {
         return 2;
      }
      i++;
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
   
   if(points.size() == 0) {
      points.push_back(0);
   }
   //sort the vector
   std::sort(points.rbegin(), points.rend()); 
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

   //0 index will store the vector of all JD's and 1 index will store the vector of all QS
   playableCards.push_back(std::vector<Card>());
   playableCards.push_back(std::vector<Card>());

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
         marriageStore = getSameSuitMelds(Meld::Marriage, hand, static_cast<Suit>(suit), Rank::King, 2);
         marriages.insert(marriages.begin(), marriageStore.begin(), marriageStore.end());
      }
   }
   
   return marriages;
}



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

   //since melds must contain at least one new card from hand and since 
   //Royal Marriage is the only possible meld that could violate this rule
   //we check that the any pair of King/Queen cards have not been used together to form a Flush
   if(meld == Meld::RoyalMarriage) {
      std::vector<Card> royalMarriagePair;
      if(cardsOfEachRank[0].size() > 0 && cardsOfEachRank[1].size() > 0) {
         royalMarriagePair.push_back(cardsOfEachRank[0][0]);
         royalMarriagePair.push_back(cardsOfEachRank[1][0]);

         //if both cards have been used for same meld, switch the cards out if there are other equivalent cards available
         if(meldsPlayed.cardsUsedForSameMeld(royalMarriagePair)) {
            if(cardsOfEachRank[0].size() > 1) {
               Card temp = cardsOfEachRank[0][0];
               cardsOfEachRank[0][0] = cardsOfEachRank[0][1];
               cardsOfEachRank[0][1] = temp;
            } else if(cardsOfEachRank[1].size() > 1) {
               Card temp = cardsOfEachRank[1][0];
               cardsOfEachRank[1][0] = cardsOfEachRank[1][1];
               cardsOfEachRank[1][1] = temp;
            } else {
               cardsOfEachRank[0].pop_back();
               cardsOfEachRank[1].pop_back();
            }
         }
      }
      
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
         meldStore.addCard(cards[j][i], trumpSuit);
      }
      allPossibleMelds.push_back(meldStore);
   }

   return allPossibleMelds;
}

   


int MeldServices::getMeldPoints(Meld meld) const{
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

