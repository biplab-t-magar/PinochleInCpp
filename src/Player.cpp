#include "Player.h"

Player::Player() { }

void Player::takeOneCard(Card card) {
    handPile.push_back(card);
}

bool Player::setTrumpSuit(Suit trumpSuit) {
    this->trumpSuit = trumpSuit;
    return true;
}



