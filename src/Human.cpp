#include "Human.h"
#include "StringUtilities.h"
#include <exception>
#include <iostream>
#include <stdexcept>
// #include "StringUtilities.cpp"

/* *********************************************************************
Function Name: Human
Purpose: constructor for Human class
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
Human::Human() {

}

/* *********************************************************************
Function Name: getHelpForLeadCard
Purpose: Gets suggestion for lead card and outputs the reasoning behind it for user to see
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
void Human::getHelpForLeadCard() {
   Card suggestedCard;
   std::string reasoning;
   suggestedCard = suggestLeadCard(reasoning);
   std::cout << "\nHint: I recomment that you present " << suggestedCard.getShortCardStr() 
         << "(" << getCardPositionInHand(suggestedCard) << ") as your lead card because " << reasoning << "." << std::endl << std::endl;
}

/* *********************************************************************
Function Name: getHelpForChaseCard
Purpose: get suggestion for what chase card to play and outputs the reasoning behind the choice for user to see
Parameters: 
      opponentCard, the lead card played by the opponent
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
void Human::getHelpForChaseCard(Card opponentCard) {
   Card suggestedCard;
   std::string reasoning;
   suggestedCard = suggestChaseCard(reasoning, opponentCard);
   std::cout << "\nHint: I recomment that you present " << suggestedCard.getShortCardStr() 
   << "(" << getCardPositionInHand(suggestedCard) << ") as your chase card because " << reasoning << "." << std::endl << std::endl;
}

/* *********************************************************************
Function Name: getHelpForMeld
Purpose: get suggestion for what meld to play and outputs the reasoning behind the choice for user to see
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
void Human::getHelpForMeld() {
   std::string reasoning;
   MeldInstance meldToPlay = suggestMeld(reasoning);

    std::string hint = "\nHint: I recomment that you present "; 

   //create reasoning string
   for(int i = 0; i < meldToPlay.getNumOfCards() - 1; i++) {
      hint += meldToPlay.getCardByPosition(i).getShortCardStr();
      hint += "(";
      hint += std::to_string(getCardPositionInHand(meldToPlay.getCardByPosition(i)));
      hint += "), ";
   }
   hint += "and ";
   hint += meldToPlay.getCardByPosition(meldToPlay.getNumOfCards() - 1).getShortCardStr();
   hint += "(";
   hint += std::to_string(getCardPositionInHand(meldToPlay.getCardByPosition(meldToPlay.getNumOfCards() - 1)));
   hint = hint + ") to create a " + meldToPlay.getMeldTypeString() + " meld because " + reasoning + ".";
   std::cout << hint << std::endl << std::endl;
}


/* *********************************************************************
Function Name: playLeadCard
Purpose: Plays a lead card, returning the card
Parameters: 
Return Value: 
      the card played as lead card
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
Card Human::playLeadCard() {
   // std::cout << "You are the lead player for this turn" << std::endl;
   int cardToPlay = promptCardThrow();
   Card card = playFromHand(cardToPlay);
   std::cout << "You chose to play " << card.getShortCardStr() << " as your lead card." << std::endl << std::endl;
   return card;
}

/* *********************************************************************
Function Name: playChaseCard
Purpose: Plays a chase card, returning the card in the process
Parameters: 
Return Value: 
      the card played as chase card
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
Card Human::playChaseCard(Card opponentCard) {
   // std::cout << "You are the chase player for this turn" << std::endl;
   std::cout << "Your opponent played " << opponentCard.getShortCardStr() << " as the lead card." << std::endl;
   int cardToPlay = promptCardThrow();
   Card card = playFromHand(cardToPlay);
   std::cout << "You chose to play " << card.getShortCardStr() << " as your chase card" << std::endl;
   return card;
}


/* *********************************************************************
Function Name: playMeld
Purpose: Prompts the user to specify meld and plays the meld from the player's hand
Parameters: 
Return Value: 
      the meld the user played, of MeldInstnace type
Local Variables: 
Algorithm: 
      1) Get the list of positions of cards from hand to use to create the meld from user
      2) Parse the positions 
      3) Create meld from the cards in those positinos
Assistance Received: None
********************************************************************* */
MeldInstance Human::playMeld() {
   std::string meldString;
   std::vector<int> positions;
   MeldInstance meldInstance;
   while(true) {
      std::cin.clear();
      std::cout << "List out the positions (separated by spaces) of all the cards you would like to play for your meld: ";
      std::getline(std::cin, meldString);
      try {   
         positions = parseMeldPositions(meldString);
      } catch(PinochleException &e) {
         std::cout << e.what() << std::endl;
         continue;
      }
      try {
         meldInstance = createMeld(positions);
         std::cout << "You played a " << meldInstance.getMeldTypeString() << " for your meld." << std::endl;
      } catch (PinochleException &e) {
         std::cout << e.what() << std::endl;
         continue;
      }
      break;
   }
   return meldInstance;
}


/* *********************************************************************
Function Name: promptCardThrow
Purpose: Prompts the user to specify what card to throw
Parameters: 
Return Value: 
      Returns the position of the card to throw, as specified by the user
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
int Human::promptCardThrow() {
   std::cout << "What card do you want to throw?" << std::endl;
   std::cout << "    Enter the card position (first card has position 0): ";

   std::string cardToThrow;
   int cardToThrowInt;

   while(true) {
      std::cin.clear();
      std::getline(std::cin, cardToThrow);
      cardToThrow = StringUtilities::stripString(cardToThrow);
      try {
         cardToThrowInt = parsePosition(cardToThrow);
         break;
      } catch(PinochleException &e) {
         std::cout << e.what();
      }
   }
   return cardToThrowInt;

}

/* *********************************************************************
Function Name: parsePosition
Purpose: parses the positin of a card specified by the user and converts that into a readable int
Parameters: 
      str, the user's input, of type string
Return Value: 
      the position that was parsed from the user's input
Local Variables: 
      num, to hold the position after it's converted to int
Algorithm: 
      1) check that str length is less than 1 or more than 2
      2) check that the str contains only numeric characters
      3) convert str to int
      4) check that the position is valid given the number of cards in hand
      5) return the position in int form
Assistance Received: None
********************************************************************* */
int Human::parsePosition(std::string str) {
   int num;
   if(str.length() < 1 || str.length() > 2) {
         throw PinochleException("Invalid action. You must enter a valid card position. Please try again: ");
      }
      
   if(str[0] < 48 || str[0] > 57) {
      throw PinochleException("You must enter a valid number. Please try again: ");
   }
   if (str.size() > 1) {
      if(str[1] < 48 || str[1] > 57) {
         throw PinochleException("You must enter a valid number. Please try again: ");
      }
   }  
   
   try {
      num = std::stoi(str);
   } catch(const std::invalid_argument &e) {
      throw PinochleException("You must enter a valid position number for the card and not non-numeric characters. Please try again: ");
   }
   
   if (num < 0 || num >= numCardsInHand()) {
      throw PinochleException("You must enter a number between 0 and " + std::to_string((numCardsInHand() - 1)) + ". Please try again: "); 
   } else {
      return num;
   }
}

/* *********************************************************************
Function Name: parseMeldPosition
Purpose: parses the list of all card positions from the string entered by user
Parameters: 
      str, the string containing card positions for the meld
Return Value: 
      vector of integers, each integer representing one card position 
Local Variables: 
      positions, the vector of all positions, in string format
      numInSring, a temporary strintg variable to store each individual position in string format
      num, a temporary int variable to store each individual position in int form
Algorithm: 
      1) Strip the string of white spaces on both ends
      2) for each characterin the string:
      3)      check if the character represents a numerical value
      4)      if it does, append that character to the end of numInString
      5)      if the character is a white space character, then convert numInString to a position and store it
      6)       if the character is not a number or a white space, throw error
      7)       returned all the stored numbers
Assistance Received: None
********************************************************************* */
std::vector<int> Human::parseMeldPositions(std::string str) {
   std::vector<int> positions;
   std::string numInString = "";
   str = StringUtilities::stripString(str);
   int num;
   for(int i = 0; i < str.length(); i++) {
      //if the character is a number
      if(str[i] >= 48 && str[i] <= 57) {
         numInString += str[i];
      }
      //if the character is a space
      else if(str[i] == ' '){
         try {
            if(numInString != "") {
               num = parsePosition(numInString);
               positions.push_back(num);
               numInString = "";
            }
         } catch(PinochleException &e) {
            throw e;
         }
      } else {
         throw PinochleException("You must specify the position of the cards in the meld. Do not enter characters other than numbers and spaces. Try again: ");
      }
   }
   //if there are still characters left to be converted
   if(numInString != "") {
      try {
         num = parsePosition(numInString);
         positions.push_back(num);
         numInString = "";
      } catch(PinochleException &e) {
         throw e;
      }
   }
   return positions;
}
