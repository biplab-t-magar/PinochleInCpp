#include "Human.h"
#include "StringUtilities.h"
#include <exception>
#include <iostream>
#include <stdexcept>
// #include "StringUtilities.cpp"

Human::Human() {

}

void Human::getHelpForLeadCard() {
   Card suggestedCard;
   std::string reasoning;
   suggestedCard = suggestLeadCard(reasoning);
   std::cout << "\nHint: I recomment that you present " << suggestedCard.getShortCardStr() 
         << "(" << getCardPositionInHand(suggestedCard) << ") as your lead card because " << reasoning << "." << std::endl << std::endl;
}
void Human::getHelpForChaseCard(Card opponentCard) {
   Card suggestedCard;
   std::string reasoning;
   suggestedCard = suggestChaseCard(reasoning, opponentCard);
   std::cout << "\nHint: I recomment that you present " << suggestedCard.getShortCardStr() 
   << "(" << getCardPositionInHand(suggestedCard) << ") as your chase card because " << reasoning << "." << std::endl << std::endl;
}
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



Card Human::playLeadCard() {
   // std::cout << "You are the lead player for this turn" << std::endl;
   int cardToPlay = promptCardThrow();
   Card card = playFromHand(cardToPlay);
   std::cout << "You chose to play " << card.getShortCardStr() << " as your lead card." << std::endl << std::endl;
   return card;
}

Card Human::playChaseCard(Card opponentCard) {
   // std::cout << "You are the chase player for this turn" << std::endl;
   std::cout << "Your opponent played " << opponentCard.getShortCardStr() << " as the lead card." << std::endl;
   int cardToPlay = promptCardThrow();
   Card card = playFromHand(cardToPlay);
   std::cout << "You chose to play " << card.getShortCardStr() << " as your chase card" << std::endl;
   return card;
}

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
