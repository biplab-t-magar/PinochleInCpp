#include "Human.h"
#include <exception>
#include <iostream>
#include <stdexcept>
// #include "StringUtilities.cpp"

Human::Human() {

}

std::string Human::getHelpForLeadCard() {
   Card suggestedCard;
   std::string reasoning;
   suggestedCard = suggestLeadCard(reasoning);
   std::cout << "I recomment that you present " << suggestedCard.getCardString() << " as your lead card because " << reasoning << "." << std::endl << std::endl;
   return reasoning;
}
std::string Human::getHelpForChaseCard(Card opponentCard) {
   Card suggestedCard;
   std::string reasoning;
   suggestedCard = suggestChaseCard(reasoning, opponentCard);
   std::cout << "I recomment that you present " << suggestedCard.getCardString() << " as your chase card because " << reasoning << "." << std::endl << std::endl;
   return reasoning;
}
std::string Human::getHelpForMeld() {
   std::string reasoning;
   MeldInstance meldToPlay = suggestNextMeld(reasoning);
   return reasoning;
}



Card Human::playLeadCard() {
   // std::cout << "You are the lead player for this turn" << std::endl;
   int cardToPlay = promptCardThrow();
   Card card = playFromHand(cardToPlay);
   std::cout << "You chose to play " << card.getCardString() << " as your lead card" << std::endl << std::endl;
   return card;
}

Card Human::playChaseCard(Card opponentCard) {
   // std::cout << "You are the chase player for this turn" << std::endl;
   std::cout << "Your opponent played " << opponentCard.getCardString() << " as the lead card." << std::endl;
   int cardToPlay = promptCardThrow();
   Card card = playFromHand(cardToPlay);
   std::cout << "You chose to play " << card.getCardString() << " as your chase card" << std::endl << std::endl;
   return card;
}

MeldInstance Human::playMeld() {
   std::string meldString;
   std::vector<int> positions;
   MeldInstance meldInstance;
   while(true) {
      std::getline(std::cin, meldString);
      std::cout << "List out the positions (separated by spaces) of all the cards you would like to play for your meld:" << std::endl << std::endl;
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


// int Human::promptUser() {
//    std::cout << "Pick an action:" << std::endl << std::endl;
//    std::cout << "1.  Save the game" << std::endl;
//    std::cout << "2.  Make a move" << std::endl;
//    std::cout << "3.  Ask for help" << std::endl;
//    std::cout << "4.  Quit the game" << std::endl;
//    std::cout << std::endl;

//    std::string userAction;
//    int userActionInt;
//    while(true) {
//       std::getline(std::cin, userAction);
//       userAction = removeWhiteSpace(userAction);
//       if(userAction.length() != 1) {
//          std::cout << "Invalid action. You must enter a number between 1 and 4. Please try again." << std::endl;
//       }

//       try {
//          userActionInt = std::stoi(userAction);
//       } catch(const std::invalid_argument &e) {
//          std::cout << "You must enter a valid number. Please try again." << std::endl;
//          continue;
//       }
      
//       if (userActionInt < 1 || userActionInt > 4) {
//          std::cout << "You must enter a number between 1 and 4. Please try again." << std::endl; 
//       } else {
//          break;
//       }
//    }

//    switch(userActionInt) {
//       case 1:
//          std::cout << "Saving game..." << std::endl;
//          saveGameProgress();
//          break;
//       case 2:
//          break;
//       case 3:
//          break;
//       case 4:
//          std::cout << "Thank you for playing Pinochle" << std::endl;
//          exit(0);
//          break;
//       default:
//          std::cout << "Error: invalid input from user" << std::endl;
//          break;
//    }
//    return userActionInt;
   
// }


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

std::vector<int> Human::parseMeldPositions(std::string str) {
   std::vector<int> positions;
   std::string numInString = "";
   for(int i = 0; i < str.length(); i++) {
      //if the character is a number
      if(str[i] >= 48 && str[i] <= 57) {
         numInString += str[i];
      }
      //if the character is a space
      else if(str[i] == ' '){
         try {
            positions.push_back(stoi(numInString));
            numInString = "";
         } catch(std::invalid_argument &e) {
            throw PinochleException("You must specify the position of the cards in the meld. Do not enter characters other than numbers and spaces.");
         }
      } else {
         throw PinochleException("You must specify the position of the cards in the meld. Do not enter characters other than numbers and spaces.");
      }
   }
   //if there are still characters left to be converted
   if(numInString != "") {
      try {
            positions.push_back(stoi(numInString));
         } catch(std::invalid_argument &e) {
            throw PinochleException("You must specify the position of the cards in the meld. Do not enter characters other than numbers and spaces.");
      }
   }
   return positions;
}

// std::string stripString(std::string str) {
//    std::string strippedString;
//    //strip from front
//    for(int i = 0; i < str.length(); i++) {
//       if(str[i] != ' ') {
//          strippedString = str.substr(i);
//          break;
//       }
//    }
//    //strip from back
//    str = strippedString;
//    for(int i = str.length() - 1; i >= 0; i--) {
//       if(str[i] != ' ') {
//          strippedString = str.substr(0, i + 1);
//          break;
//       }
//    }
//    return strippedString;
// }

// std::string removeWhiteSpace(std::string str) {
//    std::string cleanString = "";
//    for(int i = 0; i < str.length(); i++) {
//       if(str[i] != ' ') {
//          cleanString += str[i];
//       }
//    }
//    return cleanString;
// }