#include "MeldServices.h"

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

std::vector<int> MeldServices::findAllPlayableMeldsFromHand(std::vector<Card> handPile, std::vector<Card> meldPile, Suit trumpSuit){
   if(!trumpSuitSpecified) {
      throw PinochleException("Trump Suit has not been specified for this round. Use setTrumpSuit() to specify trump suit");
   }
   
   //vector to hold each 
   std::vector<int> numOfEachMeld;
   //initialize all values in vector to 0

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


int MeldServices::howManyMarriages(Suit suit, std::vector<Card> handPile, std::vector<Card> meldPile, std::vector<Card>) {
   return countSameSuitMelds(Meld::Marriage, handPile, meldPile, suit, Rank::King, 2);
}

int MeldServices::howMany

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
   std::vector<int> howManyOfEachRankFromMeldPile;
   //holds the current rank (in int form) being counted 
   int rank = static_cast<int>(startingRank);

   //loop through all the ranks in the meld
   for(int i = 0; i < howManyCards; i++) {
      //start counting number of current rank, with count initialized to 0
      howManyOfEachRank.push_back(0);
      //start counting number of current rank taken from meld pile, with count initialized to 0
      howManyOfEachRankFromMeldPile.push_back(0);

      //loop through the hand pile
      for(int j = 0; j <  handPile.size(); j++) {
         //if both rank and suit match
         if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
            howManyOfEachRank[i]++;
         }
      }
      //loop through the meld pile
      for(int j = 0; j <  meldPile.size(); j++) {
         //if both rank and suit match
         if(handPile[j].getSuit() == suit && static_cast<int>(handPile[j].getRank()) == rank) {
            howManyOfEachRank[i]++;
            howManyOfEachRankFromMeldPile[i]++;
         }
      }
      //go to the next rank of the meld
      rank--;
   }
   //now the number of *mutually exclusive* possible melds is calculated
   //the number of that constituent rank of the meld which occurs the least times in the meld is the actual 
   //number of mutually exclusive possible melds
   
   
   return removeExtraneousChoices(meld, howManyOfEachRank, howManyOfEachRankFromMeldPile);
}

int MeldServices::countSameRankMelds(Meld meld, std::vector<Card> handPile, std::vector<Card> meldPile, Rank rank) {
   //this vector counts the number of each suit of the specified rank
   std::vector<int> howManyOfEachSuit;
   //this vectors holds the count of each rank of the specified suit that was found in the meld pile
   std::vector<int> howManyOfEachSuitFromMeldPile;
   //holds the current suit (in int form) being counted (always start with clubs and increase to other suits from there)
   int suit = static_cast<int>(Suit::Clubs);

   //loop through all the suits in the meld
   for(int i = 0; i < 4; i++) {
      //start counting number of current rank, with count initialized to 0
      howManyOfEachSuit.push_back(0);
      //start counting number of current rank taken from meld pile, with count initialized to 0
      howManyOfEachSuitFromMeldPile.push_back(0);
      //loop through the hand pile
      for(int j = 0; j <  handPile.size(); j++) {
         //if both rank and suit match
         if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
            howManyOfEachSuit[i]++;
         }
      }
      //loop through the meld pile
      for(int j = 0; j <  meldPile.size(); j++) {
         //if both rank and suit match
         if(handPile[j].getRank() == rank && static_cast<int>(handPile[j].getSuit()) == suit) {
            howManyOfEachSuit[i]++;
            howManyOfEachSuitFromMeldPile[i]++;
         }
      }
      //go to the next rank of the meld
      suit++;
   }

   return removeExtraneousChoices(meld, howManyOfEachSuit, howManyOfEachSuitFromMeldPile);
}


int MeldServices::removeExtraneousChoices(Meld meld, std::vector<int> howManyOfEachCard, std::vector<int> howManyFromMeldPile) {
   if(howManyOfEachCard.size() != howManyFromMeldPile.size()) {
      throw PinochleException("howManyOfEachCard and howManyFromMeldPile must have same size");
   }
   if(howManyOfEachCard.size() <= 0) {
      throw PinochleException("Arguments cannot be empty vectors");
   }
   //finding minimum value in howManyOfEachCard
   //any card with occurence more than that minimum value has extraneous cards
   //which represent choices we can make when we create melds
   //min represents number of mutually exclusive possible melds
   int mutuallyExclusivePossibleMelds = howManyOfEachCard[0];
   for(int i = 1; i < howManyOfEachCard.size(); i++) {
      if(mutuallyExclusivePossibleMelds > howManyOfEachCard[i]) {
         mutuallyExclusivePossibleMelds = howManyOfEachCard[i];
      } 
   }

   //Despite choices, the possible number of mutually exclusive melds is not affected.
   //So, we can eliminate these extraneous choices 
   
   //If any extraneous choices are in meld pile we want to remove from the meldPile because choosing 
   //from the meldPile might make an invalid meld
   //This explains the decision below to subtract choices from the meldPile as we eliminate choices

   //loop through all cards and delete extraneous occurrences (choices) of each card from vector 
   //representing the meldPile and from the vector representing the entire hand
   for(int i = 0; i < howManyOfEachCard.size(); i++) {
      howManyFromMeldPile[i] = howManyFromMeldPile[i] - (howManyOfEachCard[i] - mutuallyExclusivePossibleMelds);
      howManyOfEachCard[i] = mutuallyExclusivePossibleMelds;
   }  
   //Now, for each possible occurence of the meld, we want to eliminate those that 
   //use cards from the meldPile if the meld has already been used by the user
   for(int i = 0; i < howManyOfEachCard.size(); i++) {
      if(howManyFromMeldPile[i] > 0 && !meldTypePlayedFirstTime(meld)) {
         //removing choices decreases the card with the least occurences
         //and thus decreases the possible mutually exclusive melds
         if(mutuallyExclusivePossibleMelds > (howManyOfEachCard[i] - howManyFromMeldPile[i])) {
            mutuallyExclusivePossibleMelds = howManyOfEachCard[i] - howManyFromMeldPile[i];
         }
      }
   } 
   //min represents the 
   return mutuallyExclusivePossibleMelds;
   
}




void MeldServices::getMeldCoordinates(std::vector<Card>* handPile, std::vector<Card>* meldPile, Meld meld) {

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

bool MeldServices::meldUsesOnlyHandPile(std::vector<Card> handPile, std::vector<Card> meld) {
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