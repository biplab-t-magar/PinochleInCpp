#include "Player.h"
#include <iostream>

Player::Player() { }

void Player::takeOneCard(Card card) {
   hand.addCard(card);
}

bool Player::setTrumpSuit(Suit trumpSuit) {
   this->trumpSuit = trumpSuit;
   return true;
}

std::vector<Card> Player::findBestCardsToThrow() {
   
   //make sure hand is not empty
   if(hand.getNumOfCards() == 0) {
      throw PinochleException("Cannot suggest card on an empty hand");
   }
   //making copies of the original hand so we can make comparisons on the copies
   GroupOfCards bestHand;
   GroupOfCards competeHand;

   //these are all the cards that yield the best hand when thrown
   std::vector<Card> bestCardsToThrow;
   
   //set the initial bestHand to the hand produced by removing the card in the first position
   int index = 0;

   // //if we are preserving trump suits, first find the first card in hand that is not of trump suit
   // if(preserveTrumpSuits) {
   //    for(int index = 0; index < hand.getNumOfCards(); index++) {
   //       if(hand.getCardByPosition(index).getSuit() != trumpSuit) {
   //          break;
   //       }
   //    }
   // }
   
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
      bestCardsToThrow = findBestCardsToThrow();
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
   }

   assert(bestCardsToThrow.size() >= 1);

   //give the reasoning for choosing this set of cards
   reasoning = "throwing this card would result in the most favorable hand, which has the most high-value melds";

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
         //give the reasoning for choosing this set of cards
         reasoning = "throwing this card would result in the most favorable hand (meld-wise) and it would increase the chance of winning because it is a trump card";
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
         reasoning = "throwing this card would result in a good balance between a favorable hand (meld-wise) and the chance of winning the turn";
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
   }
   //if there are non-trump suit cards, find the least ranked among them   
   //first, get the first card in hand that's not a trump suit card
   int index;
   for(int index = 0; index < hand.getNumOfCards(); index++) {
      if(hand.getCardByPosition(index).getSuit() != trumpSuit) {
         leastRankedCard = hand.getCardByPosition(index);
      }
   }

   //now find the least ranked card that's not of trump suit
   for(int i = index + 1; i < hand.getNumOfCards(); i++) {
      if(hand.getCardByPosition(i).getSuit() != trumpSuit && hand.getCardByPosition(i).hasLessRankThan(leastRankedCard)) {
         leastRankedCard = hand.getCardByPosition(i);
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
         reasoning = "throwing the card of the same suit as the opponent but with a greater rank is the least expensive winning move";
      } else {
         //if the there are no cards in hand having the same suit as that of the opponent card, and having a higher rank, then simply
         //then check if there is a trump-suit card in hand
         if(hand.getCardsBySuit(trumpSuit).size() == 0) {
            //if there are no trumps suits in hand either, there is no chance of winning; simply return the lowest ranked card in hand
            cardToThrow = getLeastRankedCard();
            reasoning = "there is no way to win this turn, so throwing the least ranked card will increase chances of winning next turn";
         } else {
            //if there is at least one trump-suit card, return the lowest-ranked of the trump-suit cards 
            cardToThrow = getLeastRankedFrom(hand.getCardsBySuit(trumpSuit));
            reasoning = "there is no way of winning without using a trump suit, so throwing the least ranked trump suit is the least expensive way of winning over the opponent's non-trump suit";
         }
      }
   }
   return cardToThrow;
}


// Card Player::playCard(int position) {
//    //adjusting the string representation of the hand and meld 
   
// }

// void Player::playMeld(MeldInstance MeldInstance) {

// }

int Player::numCardsInHand() {
   return hand.getNumOfCards();
}


MeldInstance Player::createMeld(std::vector<int> positions) {
   //be sure to check if all positions are valid
// for(int i = 0; i < positions.size(); i++) {
//    if (positions[i] < 0 || positions[i] >= numCardsInHand()) {
//       std::cout << "All positions must be a valid position between 0 and " << (numCardsInHand() - 1) << ". Please try again." << std::endl; 
//    } else {
//       break;
//    }
// }
}