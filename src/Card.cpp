#include <Card.h>

Card::Card() {
    rankInitialized = false;
    suitInitialized = false;
}

Card::Card(Rank rank, Suit suit) {
    this->rank = rank;
    this->suit = suit;
    rankInitialized = true;
    suitInitialized = true;
}

Rank Card::getRank() const {
    if(rankInitialized) {
        return rank;
    } else {
        throw PinochleException("Rank for this card has not been specified");
    }
}

Suit Card::getSuit() const {
    if(suitInitialized) {
        return suit;
    } else {
        throw PinochleException("Suit for this card has not been specified");
    }
}

std::string Card::getRankString() const {
    if(rankInitialized) {
        switch(rank) {
            case Rank::Nine:
                return "Nine";
                break;
            case Rank::Jack:
                return "Jack";
                break;
            case Rank::Queen:
                return "Queen";
                break;
            case Rank::King:
                return "King";
                break;
            case Rank::Ten:
                return "Ten";
                break;
            case Rank::Ace:
                return "Ace";
                break;
            default:
                throw PinochleException("Unrecognized Rank");
                break;
        }
    } else {
        throw PinochleException("Rank for this card has not been specified");
    }
}

std::string Card::getSuitString() const {
    if(suitInitialized) {
        switch(suit) {
            case Suit::Clubs:
                return "Clubs";
                break;
            case Suit::Diamonds:
                return "Diamonds";
                break;
            case Suit::Hearts:
                return "Hearts";
                break;
            case Suit::Spades:
                return "Spades";
                break;
            default:
                throw PinochleException("Unrecognized Suit");
                break;
        }
    } else {
        throw PinochleException("Suit for this card has not been specified");
    }
}


std::string Card::getCardString() const {
    if(!suitInitialized || !rankInitialized) {
        throw PinochleException("Suit and rank for this card have not been specified");
    } else {
        std::string cardString;
        try {
            cardString = getRankString() + " of " + getSuitString();
        } catch (PinochleException &e) {
            throw e;
        }
        return cardString;
    }
}

bool Card::setRank(Rank rank) {
    this->rank = rank;
    rankInitialized = true;
    return true;
}

bool Card::setSuit(Suit suit) {
    this->suit = suit;
    suitInitialized = true;
    return true;
}