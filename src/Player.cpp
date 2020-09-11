#include "Player.h"

Player::Player() { }

void Player::takeOneCard(Card card) {
    hand.push_back(card);
}

bool Player::setTrumpSuit(Suit trumpSuit) {
    this->trumpSuit = trumpSuit;
    return true;
}


bool Player::isValidMeld(std::vector<Card> cards, Meld* whatMeld) {
    //if the given cards do not create a valid meld, then whatMeld is return with a NULL value
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

bool Player::isDix(std::vector<Card> cards) {
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

bool Player::isMarriage(std::vector<Card> cards) {
    //Note: A Marriage contains a King and Queen of any other suit besides the Trump suit

    //check size of meld
    if(cards.size() != 2) {
        return false;
    }

    //If both cards are trump suit, return false
    if(cards[0].getSuit() == trumpSuit && cards[1].getSuit() == trumpSuit) {
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

bool Player::isRoyalMarriage(std::vector<Card> cards) {
     //Note: A Royal Marriage contains a King and Queen of the Trump suit

    //check size of meld
    if(cards.size() != 2) {
        return false;
    }

    //check if both are trump suit
    if(cards[0].getSuit() == trumpSuit && cards[1].getSuit() == trumpSuit) {
        //if one card is a King and the other a Queen, return true
        if(cards[0].getRank() == Rank::King && cards[1].getRank() == Rank::Queen) {
            return true;
        }
        if(cards[1].getRank() == Rank::King && cards[0].getRank() == Rank::Queen) {
            return true;
        }
    }

    //if none of the conditions match return false
    return false;
}

bool Player::isPinochle(std::vector<Card> cards) {
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



bool Player::isFours(std::vector<Card> cards) {
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

bool Player::isFlush(std::vector<Card> cards) {
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