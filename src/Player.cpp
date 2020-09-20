#include "Player.h"

Player::Player() { }

void Player::takeOneCard(Card card) {
   hand.addCard(card);
}

bool Player::setTrumpSuit(Suit trumpSuit) {
   this->trumpSuit = trumpSuit;
   return true;
}

Card Player::playCard(int position) {
   //adjusting the string representation of the hand and meld 
   
}

void Player::playMeld(MeldInstance MeldInstance) {

}

