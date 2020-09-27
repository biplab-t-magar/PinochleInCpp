#include "Player.h"
#include <iostream>

#define numOfMeldTypes 9
#define numOfSuits 4

// Player::Player() { 

// }

// Player::~Player() {

// }

Player::Player(GroupOfCards hand, GroupOfCards capturePile, MeldsStorage meldsPlayed, Suit trumpSuit) {
   this->hand = hand;
   this->capturePile = capturePile;
   this->trumpSuit = trumpSuit;
   this->meldServices = MeldServices(meldsPlayed, trumpSuit);
}

void Player::takeOneCard(Card card) {
   hand.addCard(card);
}

// void Player::takeCards(std::vector<Card> cards) {
//    for(int i = 0; i < cards.size(); i++) {
//       hand.addCard(cards[i]);
//    }
// }

GroupOfCards Player::getHand() const {
   return hand;
} 

MeldsStorage Player::getMeldsPlayed() const {
   return meldServices.getMeldsPlayed();
}

GroupOfCards Player::getCapturePile() const {
   return capturePile;
}

bool Player::setTrumpSuit(Suit trumpSuit) {
   if(static_cast<int>(trumpSuit) >= numOfSuits || static_cast<int>(trumpSuit) < 0 ) {
      throw PinochleException("invalid trump suit");
   }
   this->trumpSuit = trumpSuit;
   meldServices.setTrumpSuit(trumpSuit);
   return true;
}


Card Player::playFromHand(int position) {
   if(position >= numCardsInHand() || position < 0) {
      throw PinochleException("Position is out of range.");
   } 
   Card card = hand.getCardByPosition(position);
   hand.removeCardByPosition(position);
   return card;
}

Card Player::playFromHand(Card card) {
   if(!hand.searchCardById(card.getId())) {
      throw PinochleException("This card is not in the player's hand.");
   } 
   hand.removeCardById(card.getId());
   return card;
}

void Player::addToCapturePile(Card card1, Card card2) {
   capturePile.addCard(card1);
   capturePile.addCard(card2);
}

std::vector<Card> Player::bestCardsForLeadThrow() {
   
   //make sure hand is not empty
   if(hand.getNumOfCards() == 0) {
      throw PinochleException("Cannot suggest card on an empty hand");
   }
   //making copies of the original hand so we can make comparisons on the copies
   GroupOfCards bestHand;
   GroupOfCards competeHand;

   //these are all the cards that yield the best hand when thrown
   std::vector<Card> bestCardsToThrow;

   bestHand = hand;
   bestHand.removeCardByPosition(0);
   bestCardsToThrow.push_back(hand.getCardByPosition(0));
   //iterate through all cards to see what hand would look like if we remove each card
   for(int i = 1; i < hand.getNumOfCards(); i++) {
      competeHand = hand;
      competeHand.removeCardByPosition(i);

      //if the competing hand is better
      if(meldServices.compareHandsForMelds(bestHand, competeHand) == 2) {
         bestHand = competeHand;
         //there is a hand that is better than all the hands encountered before, so we empty 
         //out bestCardsToThrow and add the new best card
         bestCardsToThrow.clear();
         bestCardsToThrow.push_back(hand.getCardByPosition(i));  
      } 
      //if its a draw between the two hands, then we add the other best card to the vector of best cards
      else if(meldServices.compareHandsForMelds(bestHand, competeHand) == 0) {
         bestCardsToThrow.push_back(hand.getCardByPosition(i));
      }
   }
   return bestCardsToThrow;
}

Card Player::suggestLeadCard(std::string &reasoning) {
   //use the compareHandsForMelds to check what card, when thrown, will preserve the best melds
   //we simulate throwing every single card in the hand to create as many possible hands as there are cards in the hand
   std::vector<Card> bestCardsToThrow;
   try {
      bestCardsToThrow = bestCardsForLeadThrow();
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
   }

   assert(bestCardsToThrow.size() >= 1);

   //give the reasoning for choosing this set of cards
   reasoning = "throwing this card would preserve the most favorable hand, which has the most high-value melds";

   //now, check the size of bestCardsToThrow. if it is more than one, then we need to further compare
   if(bestCardsToThrow.size() > 1) {
      //if any of them is of suit , throw the trump suit card since that will increase the chance of winning the round
      //without sacrificing melds
      std::vector<Card> trumpSuitCardsToThrow;
      for(int i = 0; i < bestCardsToThrow.size(); i++) {
         if(bestCardsToThrow[i].getSuit() == trumpSuit) {
            trumpSuitCardsToThrow.push_back(bestCardsToThrow[i]);
         }
      }

      //if there is at least one trump card, then that card(s) is the new best card(s)
      if(trumpSuitCardsToThrow.size() != 0) {
         bestCardsToThrow = trumpSuitCardsToThrow;
      }
      //if still there are more than one cards, get the card with the greater Rank
      //if even ranks are the same, it does not matter what card we throw
      if(bestCardsToThrow.size() >= 1) {
         //here, we store the best card in the first index of the vector
         for(int i = 1; i < bestCardsToThrow.size(); i++) {
            if(bestCardsToThrow[i].hasGreaterRankThan(bestCardsToThrow[0])) {
               bestCardsToThrow[0] = bestCardsToThrow[i];
            }
         }
         if(bestCardsToThrow[0].getSuit() == trumpSuit) {
            reasoning = "throwing this card would preserve the most favorable hand (meld-wise) and it would increase the chance of winning because it is a trump card";
         } else {
            reasoning = "throwing this card would result in a good balance between a favorable hand (meld-wise) and the chance of winning the turn";
         }
         
      }
   } 
   return bestCardsToThrow[0];
}

Card Player::getLeastRankedCard() {
   if(hand.getNumOfCards() == 0) {
      throw PinochleException("Cannot get least ranked card from an empty hand.");
   }
   Card leastRankedCard;
   //first check to see if there are any non-trump suit cards in hand
   if(hand.getCardsBySuit(trumpSuit).size() == hand.getNumOfCards()) {
      //if there are no non-trump suit cards, simply find the card with the least rank
      leastRankedCard = hand.getCardByPosition(0);
      for(int i = 1; i < hand.getNumOfCards(); i++) {
         if(hand.getCardByPosition(1).hasLessRankThan(leastRankedCard)) {
            leastRankedCard = hand.getCardByPosition(i);
         }
      }
   } else {
      //if there are non-trump suit cards, find the least ranked among them   
      //first, get the first card in hand that's not a trump suit card
      int index;
      for(index = 0; index < hand.getNumOfCards(); index++) {
         if(hand.getCardByPosition(index).getSuit() != trumpSuit) {
            leastRankedCard = hand.getCardByPosition(index);
            break;
         }
      }
      //now find the least ranked card that's not of trump suit
      for(int i = index + 1; i < hand.getNumOfCards(); i++) {
         if(hand.getCardByPosition(i).getSuit() != trumpSuit && hand.getCardByPosition(i).hasLessRankThan(leastRankedCard)) {
            leastRankedCard = hand.getCardByPosition(i);
         }
      }
   }
   
   return leastRankedCard;
}

Card Player::getLeastRankedFrom(std::vector<Card> cards) {
   if(cards.size() == 0) {
      throw PinochleException("Cannot get least ranked card from an empty vector of cards");
   }
   Card leastRankedCard;
   leastRankedCard = cards[0];
   for(int i = 1; i < cards.size(); i++) {
      if(cards[i].hasLessRankThan(leastRankedCard)) {
         leastRankedCard = cards[i];
      }
   }
   return leastRankedCard;
}


Card Player::suggestChaseCard(std::string &reasoning, Card opponentCard) {
   Card cardToThrow;
   //first, see if the opponent's card is of trump suit
   if(opponentCard.getSuit() == trumpSuit) {
      //get the list of all the trump suits in hand
      std::vector<Card> trumpSuitCards = hand.getCardsBySuit(trumpSuit);
      if(trumpSuitCards.size() != 0) {
         //if we do have a trump suit, we throw the card that's just greater than the opponent's card

         //first, find all the trump suit cards greater than the opponent card
         std::vector<Card> greaterTrumpSuitCards;
         for(int i = 0; i < trumpSuitCards.size(); i++) {
            if(trumpSuitCards[i].hasGreaterRankThan(opponentCard)) {
               greaterTrumpSuitCards.push_back(trumpSuitCards[i]);
            }
         }
         //if there are no trump suit cards that have higher rank than the opponent's card, there is no way to win
         //So, throw the least ranked card
         if(greaterTrumpSuitCards.size() == 0) {
            cardToThrow = getLeastRankedCard();
            reasoning = "there is no way to win this turn, so throwing the least ranked card will increase chances of winning next turn";
         } else {
            //if there are trump suit cards that have higher rank than the opponent's card, find the smallest one among them
            cardToThrow = getLeastRankedFrom(greaterTrumpSuitCards);
            reasoning = "throwing a trump-suit card that is higher than but still closest to the opponent's trump-suit card will be the least expensive winning move";
         }
      } else {
         //if no trump suits in hand when the opponent's card is a trump suit, we cannot win the turn
         //throw the card with the lowest rank
         cardToThrow = getLeastRankedCard();
         reasoning = "there is no way to win this turn, so throwing the least ranked card will increase chances of winning next turn";
      }
   } else {
      //if opponents card is not a trump suit, we try our best not to use trump suit
      //first, get a vector of all cards from hand having the same suit as the opponent's card and having a greater rank than the opponent's card
      std::vector<Card> sameSuitCards;
      for(int i = 0; i < hand.getNumOfCards(); i++) {
         if(hand.getCardByPosition(i).getSuit() == opponentCard.getSuit() && hand.getCardByPosition(i).hasGreaterRankThan(opponentCard)) {
            sameSuitCards.push_back(hand.getCardByPosition(i));
         }
      }
      //if same suit cards with ranks greater than that of opponent card were found, then return the card with the smallest rank among them
      if(sameSuitCards.size() > 0) {
         cardToThrow = getLeastRankedFrom(sameSuitCards);
         reasoning = "throwing the card of the same suit as the opponent but with a rank greater than but closest to that of the opponent's card is the least expensive winning move";
      } else {
         //if the there are no cards in hand having the same suit as that of the opponent card, and having a higher rank,
         //then check if there is a trump-suit card in hand

         if(hand.getCardsBySuit(trumpSuit).size() == 0) {
            //if there are no trumps suits in hand either, there is no chance of winning; simply return the lowest ranked card in hand
            cardToThrow = getLeastRankedCard();
            reasoning = "there is no way to win this turn, so throwing the least ranked card will increase chances of winning next turn";
         } else {
            //if there is at least one trump-suit card, return the lowest-ranked of the trump-suit cards 
            cardToThrow = getLeastRankedFrom(hand.getCardsBySuit(trumpSuit));
            reasoning = "there is no way of winning without using a trump suit, so throwing the least ranked trump suit is the least expensive way of winning over the opponent's non-trump-suit card";
         }
      }
   }
   return cardToThrow;
}

MeldInstance Player::suggestMeld(std::string &reasoning) {
   //first, get all possible melds from the hand
   MeldsStorage allPossibleMelds = meldServices.getMeldsFromHand(hand);

   if(allPossibleMelds.getNumOfMelds() == 0) {
      reasoning = "there are no possible melds to play with";
      return MeldInstance();
   }

   //this stores all the meld instances that yield the highest points
   std::vector<MeldInstance> highScoringMelds;

   int highestPointsPossible = 0;
   std::vector<MeldInstance> allMeldsOfGivenType;
   int meldPoints;
   //loop through all 9 meld types
   for(int i = 0; i < numOfMeldTypes; i++) {
      //if a particular meld yields the highest points so far, keep track of it
      allMeldsOfGivenType = allPossibleMelds.getAllMeldsByType(static_cast<Meld>(i));
      if(allMeldsOfGivenType.size() == 0) {
         meldPoints = 0;
      } else {
         meldPoints = meldServices.getMeldPoints(static_cast<Meld>(i));
      }
      
      if(meldPoints > highestPointsPossible) {
         highestPointsPossible = meldPoints;
         highScoringMelds.clear();
         //append all the instances of this meld type to highScoringMelds
         highScoringMelds.insert(highScoringMelds.end(), allMeldsOfGivenType.begin(), allMeldsOfGivenType.end());
      } else if (meldPoints == highestPointsPossible) {
         //append all the instances of this meld type to highScoringMelds
         highScoringMelds.insert(highScoringMelds.end(), allMeldsOfGivenType.begin(), allMeldsOfGivenType.end());
      }
   }

   if(highScoringMelds.size() == 1) {
      reasoning = "playing this meld will yield the highest possible points";
      return highScoringMelds[0];
   } else {
      reasoning = "playing this meld will yield the highest possible points and will also ensure that the best melds are preserved for the next turn.";
      return findBestMeldToPlay(highScoringMelds);
   }

}

MeldInstance Player::findBestMeldToPlay(std::vector<MeldInstance> meldsToCompare) {
   MeldServices mockServices1 = meldServices;
   MeldServices mockServices2 = meldServices;

   //simulate playing the first meld in meldsToCompare
   MeldInstance bestMeld = meldsToCompare[0];
   mockServices1.storeMeld(hand, bestMeld);

   //now, loop through the rest of the melds and compare the potential points from hand after playing each meld
   for(int i = 1; i < meldsToCompare.size(); i++) {
      mockServices2.storeMeld(hand, meldsToCompare[i]);
      //if melds stored in mockServices2 results in higher potential points from the hand
      if(mockServices1.potentialPointsFromHand(hand) < mockServices2.potentialPointsFromHand(hand)) {
         bestMeld = meldsToCompare[i];
         mockServices1 = mockServices2;
      }
      //reset mockServices2
      mockServices2 = meldServices;
   }
   
   return bestMeld;

}


int Player::numCardsInHand() {
   return hand.getNumOfCards();
}

bool Player::isMeldPossible() {
   MeldsStorage allPossibleMelds = meldServices.getMeldsFromHand(hand);
   if(allPossibleMelds.getNumOfMelds() == 0) {
      return false;
   } 
   return true;
}

int Player::getCardPositionInHand(Card card) {
   return hand.getCardPosition(card);
}



MeldInstance Player::createMeld(std::vector<int> positions) {
   // be sure   to check if all positions are valid
   for(int i = 0; i < positions.size(); i++) {
      if (positions[i] < 0 || positions[i] >= numCardsInHand()) {
         std::string errorMessage = "All positions must be a valid position between 0 and " + std::to_string(numCardsInHand() - 1) + ". Please try again.";
         throw PinochleException(errorMessage); 
      }
   }

   MeldInstance meldInstance;
   //next, combine all cards from each position to create a MeldInstance object
   for(int i = 0; i < positions.size(); i++) {
      meldInstance.addCard(hand.getCardByPosition(positions[i]), trumpSuit);
   }
   try {
      createMeld(meldInstance);
   } catch(PinochleException &e) {
      throw e;
   }
   
   return meldInstance;
}


MeldInstance Player::createMeld(MeldInstance meldInstance) {
   if(meldInstance.isValidMeld() == false) {
      throw PinochleException("The cards you specified do not combine to make up a meld. Please try again.");
   }   

   if(!meldServices.allCardsPresentInHand(hand, meldInstance)) {
      throw PinochleException("All the cards in the meld are not present in the player's hand.");
   }

   if(!meldServices.meldIsNotARepeat(meldInstance)) {
      throw PinochleException("This meld is not valid because at least one card in it has been used to create an instance of this same meld type before.");
   }

   if(!meldServices.meldHasANewCard(meldInstance)) {
      throw PinochleException("The meld must contain at least one new card from hand. Please try again.");
   }

   if (meldServices.storeMeld(hand, meldInstance) == false) {
      throw PinochleException("Unable to store meld.");
   }
   return meldInstance;
}

