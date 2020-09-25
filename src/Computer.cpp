#include "Computer.h"
#include <iostream>

Computer::Computer() {

}

Card Computer::playLeadCard() {
   std::string reasoning;
   Card cardToPlay = suggestLeadCard(reasoning);
   playFromHand(cardToPlay);
   std::cout << "The computer chose to play " << cardToPlay.getShortCardStr() << " as its lead card" <<
               " because " << reasoning << "." << std::endl;
   return cardToPlay;
}


Card Computer::playChaseCard(Card opponentCard) {
   std::string reasoning;
   Card cardToPlay = suggestChaseCard(reasoning, opponentCard);
   playFromHand(cardToPlay);
   std::cout << "The computer chose to play " << cardToPlay.getShortCardStr() << " as its chase card" <<
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
   std::string reason = "The computer chose to play ";

   for(int i = 0; i < meldToPlay.getNumOfCards() - 1; i++) {
         reason += meldToPlay.getCardByPosition(i).getShortCardStr();
         reason += "(";
         reason += getCardPositionInHand(meldToPlay.getCardByPosition(i));
         reason += "), ";
      }
   reason += "and ";
   reason += meldToPlay.getCardByPosition(meldToPlay.getNumOfCards() - 1).getShortCardStr();
   reason += "(";
   reason += getCardPositionInHand(meldToPlay.getCardByPosition(meldToPlay.getNumOfCards() - 1));
   reason = reason + ") to create a " + meldToPlay.getMeldTypeString() + " meld because " + reasoning + ".";

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
