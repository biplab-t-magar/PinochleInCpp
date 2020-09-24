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

MeldInstance Computer::playMeld() {
   std::string reasoning;
   MeldInstance meldToPlay = suggestMeld(reasoning);
   if(meldToPlay.getNumOfCards() == 0) {
      std::cout << "The computer has no meld to play." << std::endl;
      return meldToPlay;
   }
   try {
      createMeld(meldToPlay);
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
      exit(1);
   }
   std::cout << "The computer chose to play a " << meldToPlay.getMeldTypeString() << " for its meld" << 
               " because " << reasoning << "." << std::endl;
   return meldToPlay;
}

void Computer::getHelpForLeadCard() {
   throw PinochleException("Non-human players cannot ask for help for moves.");
}
void Computer::getHelpForChaseCard(Card opponentCard) {
   throw PinochleException("Non-human players cannot ask for help for moves.");
}
void Computer::getHelpForMeld() {
   throw PinochleException("Non-human players cannot ask for help for moves.");
}
