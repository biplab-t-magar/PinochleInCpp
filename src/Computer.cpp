/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#include "Computer.h"
#include <iostream>


/* *********************************************************************
Function Name: Computer
Purpose: Default constructor for Card class that initializes a computer object.
Parameters: 
Return Value: 
Assistance Received: None
********************************************************************* */
Computer::Computer() {

}

/* *********************************************************************
Function Name: Computer
Purpose: Constructor for Computer class that initializes a computer object (and Player base class object) with the given parameters
Parameters: 
      hand, the Computer player's hand,
      capturePile, the Computer player's capture pile
      meldPlayed, the record of all the melds that the computer player has played
      trumpSuit, the trump suit of the current round
Return Value: 
Assistance Received: None
********************************************************************* */
Computer::Computer(GroupOfCards hand, GroupOfCards capturePile, MeldsStorage meldsPlayed, Suit trumpSuit) : Player(hand, capturePile, meldsPlayed, trumpSuit) {

}


/* *********************************************************************
Function Name: playLeadCard
Purpose: To play a lead card during a turn
Parameters: 
Return Value: The card that is to be played as the lead card, of type Card
Assistance Received: None
********************************************************************* */
Card Computer::playLeadCard() {
   std::string reasoning;
   Card cardToPlay = suggestLeadCard(reasoning);
   playFromHand(cardToPlay);
   std::cout << "The computer chose to play " << cardToPlay.getShortCardStr() << " as its lead card" <<
               " because " << reasoning << "." << std::endl;
   return cardToPlay;
}

/* *********************************************************************
Function Name: playChaseCard
Purpose: To play a chase card during a turn
Parameters: 
      opponentCard, the card of the opponent player, of type Card
Return Value: 
      The chase card to be played by the computer player during the turn, of type Card
Assistance Received: None
********************************************************************* */
Card Computer::playChaseCard(Card opponentCard) {
   std::string reasoning;
   Card cardToPlay = suggestChaseCard(reasoning, opponentCard);
   playFromHand(cardToPlay);
   std::cout << "The computer chose to play " << cardToPlay.getShortCardStr() << " as its chase card" <<
               " because " << reasoning << "." << std::endl;
   return cardToPlay;
}

/* *********************************************************************
Function Name: playMeld
Purpose: Plays a meld by picking a meld
Parameters: 
Return Value: 
      meldToPlay, the meld to be played; is of type MeldInstance
Local Variables: 
      reasoning, to store the reasoning for picking the card
Algorithm: 
      1)Get the suggested chase card and the reasoning for suggestion and store them
      2)Output reasoning
      3)return suggested chase card
Assistance Received: None
********************************************************************* */
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

   //creating the string that forms the console output that explains reasoning
   std::string reason = "The computer chose to play ";

   for(int i = 0; i < meldToPlay.getNumOfCards() - 1; i++) {
         reason += meldToPlay.getCardByPosition(i).getShortCardStr();
         reason += "(";
         reason += std::to_string(getCardPositionInHand(meldToPlay.getCardByPosition(i)));
         reason += "), ";
      }
   reason += "and ";
   reason += meldToPlay.getCardByPosition(meldToPlay.getNumOfCards() - 1).getShortCardStr();
   reason += "(";
   reason += std::to_string(getCardPositionInHand(meldToPlay.getCardByPosition(meldToPlay.getNumOfCards() - 1)));
   reason = reason + ") to create a " + meldToPlay.getMeldTypeString() + " meld because " + reasoning + ".";

   return meldToPlay;
}

/* *********************************************************************
Function Name: getHelpForCard()
Purpose: Implementations for the virtual function from base class
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
      1)Throw exception because this function is not applicable to Computer player
Assistance Received: None
********************************************************************* */
void Computer::getHelpForLeadCard() {
   throw PinochleException("Non-human players cannot ask for help for moves.");
}

/* *********************************************************************
Function Name: getHelpForChaseCard()
Purpose: Implementations for the virtual function from base class
Parameters: 
      opponentCard, the lead card thrown by the opponent during a turn, of type Card
Return Value: 
Local Variables: 
Algorithm: 
      1)Throw exception because this function is not applicable to Computer player
Assistance Received: None
********************************************************************* */
void Computer::getHelpForChaseCard(Card opponentCard) {
   throw PinochleException("Non-human players cannot ask for help for moves.");
}

/* *********************************************************************
Function Name: getHelpForMeld()
Purpose: Implementations for the virtual function from base class
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
      1)Throw exception because this function is not applicable to Computer player
Assistance Received: None
********************************************************************* */
void Computer::getHelpForMeld() {
   throw PinochleException("Non-human players cannot ask for help for moves.");
}
