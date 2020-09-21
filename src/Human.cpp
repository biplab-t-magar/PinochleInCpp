#include "Human.h"
#include <exception>
#include <iostream>
#include <stdexcept>

Human::Human() {

}




Card Human::playLeadCard() {
   std::cout << "You are the lead player for this turn" << std::endl;
   int userAction;
   Card suggestedCard;
   std::string reason;
   do {
      userAction = promptUser();
      if(userAction == 3) {
         suggestedCard = suggestChaseCard(reason);
         std::cout << reason << std::endl << std::endl;
      }
   } while(userAction != 2 );

   int cardToPlay = promptCardThrow();
   Card card = hand.getCardByPosition(cardToPlay);
   std::cout << "You chose to play " << card.getCardString() << " as your lead card" << std::endl << std::endl;
   return card;
}

Card Human::playChaseCard(Card card) {
   std::cout << "You are the chase player for this turn" << std::endl;
   std::cout << "Your opponent played " << card.getCardString() << " as the lead card." << std::endl;
   int userAction;
   Card suggestedCard;
   std::string reason;
   do {
      userAction = promptUser();
      if(userAction == 3) {
         suggestedCard = suggestChaseCard(reason);
         std::cout << reason << std::endl << std::endl;
      }
   } while(userAction != 2);

   int cardToPlay = promptCardThrow();
   Card card = hand.getCardByPosition(cardToPlay);
   std::cout << "You chose to play " << card.getCardString() << " as your lead card" << std::endl << std::endl;
   return card;
}

int Human::promptUser() {
   std::cout << "Pick an action:" << std::endl << std::endl;
   std::cout << "1.  Save the game" << std::endl;
   std::cout << "2.  Make a move" << std::endl;
   std::cout << "3.  Ask for help" << std::endl;
   std::cout << "4.  Quit the game" << std::endl;
   std::cout << std::endl;

   std::string userAction;
   int userActionInt;
   while(true) {
      std::getline(std::cin, userAction);
      userAction = removeWhiteSpace(userAction);
      if(userAction.length() != 1) {
         std::cout << "Invalid action. You must enter a number between 1 and 4. Please try again." << std::endl;
      }

      try {
         userActionInt = std::stoi(userAction);
      } catch(const std::invalid_argument &e) {
         std::cout << "You must enter a valid number. Please try again." << std::endl;
         continue;
      }
      
      if (userActionInt < 1 || userActionInt > 4) {
         std::cout << "You must enter a number between 1 and 4. Please try again." << std::endl; 
      } else {
         break;
      }
   }

   switch(userActionInt) {
      case 1:
         std::cout << "Saving game..." << std::endl;
         saveGameProgress();
         break;
      case 2:
         break;
      case 3:
         break;
      case 4:
         std::cout << "Thank you for playing Pinochle" << std::endl;
         exit(0);
         break;
      default:
         std::cout << "Error: invalid input from user" << std::endl;
         break;
   }
   return userActionInt;
   
}


int Human::promptCardThrow() {
   std::cout << "What card do you want to throw?" << std::endl;
   std::cout << "    Enter the card position (first card has position 0)" << std::endl;

   std::string cardToThrow;
   int cardToThrowInt;

   while(true) {
      std::getline(std::cin, cardToThrow);
      cardToThrow = removeWhiteSpace(cardToThrow);
      if(cardToThrow.length() < 1 || cardToThrow.length() > 2) {
         std::cout << "Invalid action. You must enter a valid card position. Please try again." << std::endl;
      }

      try {
         cardToThrowInt = std::stoi(cardToThrow);
      } catch(const std::invalid_argument &e) {
         std::cout << "You must enter a valid position number for the card and not non-numeric characters. Please try again." << std::endl;
         continue;
      }
      
      if (cardToThrowInt < 0 || cardToThrowInt >= numCardsInHand()) {
         std::cout << "You must enter a number between 0 and " << (numCardsInHand() - 1) << ". Please try again." << std::endl; 
      } else {
         break;
      }
   }
   return cardToThrowInt;

}

std::string Human::removeWhiteSpace(std::string str) {
   std::string cleanString = "";
   for(int i = 0; i < str.length(); i++) {
      if(str[i] != ' ') {
         cleanString += str[i];
      }
   }
   return cleanString;
}