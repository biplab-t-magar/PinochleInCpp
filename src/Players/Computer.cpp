#include "Computer.h"
#include <iostream>

Computer::Computer() {

}

Card Computer::playLeadCard() {
   std::string reasoning;
   Card cardToPlay = suggestLeadCard(reasoning);
   playFromHand(cardToPlay);
   std::cout << "The computer chose to play " << cardToPlay.getCardString() << " as its lead card" <<
               " because " << reasoning << "." << std::endl;
   return cardToPlay;
}


Card Computer::playChaseCard(Card opponentCard) {
   std::string reasoning;
   Card cardToPlay = suggestChaseCard(reasoning, opponentCard);
   playFromHand(cardToPlay);
   std::cout << "The computer chose to play " << cardToPlay.getCardString() << " as its chase card" <<
               " because " << reasoning << "." << std::endl;
   return cardToPlay;
}

void Computer::playMeld() {
   std::string reasoning;
   MeldInstance meldToPlay = suggestNextMeld(reasoning);
   if(meldToPlay.getNumOfCards() == 0) {
      return;
   }
   try {
      createMeld(meldToPlay);
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
      exit(1);
   }
   std::cout << "The computer chose to play a " << meldToPlay.getMeldTypeString() << " for its meld" << 
               " because " << reasoning << "." << std::endl;
}