#include "Player.h"

Player::Player() { }

void Player::takeOneCard(Card card) {
   hand.addCard(card);
}

bool Player::setTrumpSuit(Suit trumpSuit) {
   this->trumpSuit = trumpSuit;
   return true;
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