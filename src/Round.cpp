#include "Round.h"
#include <iostream>
#include "StringUtilities.cpp"

Round::Round() {

}

void Round::startNewRound(int roundNumber, int &hGameScore, int &cGameScore) {
   std::cout << "Welcome to Pinochle. Let's start the game." << std::endl << std::endl << std::endl;
   //storing players for the game:
   Player players[2];

   //there is a reason for storing these players in this order
   //we can use the boolean variable humansTurn as the index for this array
   players[0] = Computer();
   players[1] = Human();
   

   std::cout << "Distributing cards:" << std::endl;
   for(int i = 0; i < 3; i++) {
      std::cout << "Giving human four cards..." << std::endl;
      for(int i = 0; i < 4; i++) {
         human.takeOneCard(stock.takeOneFromTop());
      }
      std::cout << "Giving computer four cards..." << std::endl;
      for(int i = 0; i < 4; i++) {
         computer.takeOneCard(stock.takeOneFromTop());
      }
   }

   trumpCard = stock.takeOneFromTop();
   trumpSuit = trumpCard.getSuit();
   std::cout << "The trump card for this round is " << trumpCard.getCardString() << std::endl << std::endl;
   
   if(roundNumber == 1 || hGameScore == cGameScore) {
      //if humansTurnIsNext is set to true, the human player plays the next turn
      humansTurn = coinToss();      
   } else {
      //check to see which player has higher score from previous rounds
      if(hGameScore > cGameScore) {
         std::cout << "Since you have a higher score from previous rounds, you go first!" << std::endl;
         humansTurn = true;
      } else {
         std::cout << "Since the computer has a higher score from previous rounds, the computer goes first." << std::endl;
         humansTurn = false;
      }
   }

   Card leadCard;
   Card chaseCard;
   int userAction;
   //loop for each turn
   while(human.numCardsInHand() != 0 || computer.numCardsInHand() != 0) {
      //prompt user action
      switch (promptUser()) {
      case 1:
         // saveGame();
         std::cout << "Game saved!" << std::endl;
         continue;
         break;
      case 2:
         //simply proceed beyond switch statement if user wants to make a move
         break;
      case 3:
         std::cout << players[1].getHelpForLeadCard();
         break;
      case 4: 
         std::cout << "Thank you for playing Pinochle! Exiting game..." << std::endl;
         exit(0);
         break;
      default:
         break;
      }
      //the player whose turn it is plays the lead card
      leadCard = players[humansTurn].playLeadCard();
      humansTurn = !humansTurn;

      //again, ask prompt user action
      promptUser();
      switch (promptUser()) {
      case 1:
         // saveGame();
         std::cout << "Game saved!" << std::endl;
         continue;
         break;
      case 2:
         //simply proceed beyond switch statement if user wants to make a move
         break;
      case 3:
         std::cout << players[1].getHelpForChaseCard(leadCard);
         break;
      case 4: 
         std::cout << "Thank you for playing Pinochle! Exiting game..." << std::endl;
         exit(0);
         break;
      default:
         break;
      }

      chaseCard = players[humansTurn].playChaseCard(leadCard);

      leadVsChase(leadCard, chaseCard);

      std::cout << "The winner takes both cards for their capture pile." << std::endl;
      players[humansTurn].addToCapturePile(leadCard, chaseCard);

   }

}

void Round::leadVsChase(Card leadCard, Card chaseCard) {
   //find out who wins
   if(leadCardWins(leadCard, chaseCard)) {
      //if it was the humans lead, then it would be the computer's turn during the playing of the chase card
      //that's why humansTurn == false if human played the lead card
      if(!humansTurn) {
         std::cout << "Your lead card has beaten the computer's chase card. You win this turn!" << std::endl;
         humansTurn = true;
      } else {
         std::cout << "Your chase card was beaten by the computer's lead card. You lose this turn." << std::endl;
         humansTurn = false;
      }
   } else {
      if(!humansTurn) {
         std::cout << "Your chase card was beaten by the computer's lead card. You lose this turn." << std::endl;
         humansTurn = false;
      } else {
         std::cout << "Your chase card has beaten the computer's lead card. You win this turn!" << std::endl;
         humansTurn = true;
      }
   }
}

bool Round::leadCardWins(Card leadCard, Card chaseCard) {
   if(leadCard.getSuit() == trumpSuit) {
      //if the chase card is of trump suit
      if(chaseCard.getSuit() == trumpSuit) {
         //if the rank of the lead card is less than that of chase card, lead card loses, else it wins
         return !(leadCard.hasLessRankThan(chaseCard));
      } else {
         //if the chase card is not of trump suit, lead card wins
         return true;
      }
   } else {
      //if the lead card is not of trump suit

      //if the chase card is of trump suit, chase card wins
      if(chaseCard.getSuit() == trumpSuit) {
         return false; 
      } else if(chaseCard.getSuit() == leadCard.getSuit()) {
         //if the chase card has the same suit as that of the lead card,
         //then the chase card only wins if it has higher rank than chase card
         return !(leadCard.hasLessRankThan(chaseCard));
      } else {
         //if the chase card is neither of trump suit nor does it have the same suit as the lead card, lead card wins
         return true;
      }
   }
}

int cardPoints(Card card){
   switch (card.getRank()) {
   case Rank::Ace:
      return 11;
      break;
   case Rank::Ten:
      return 10;
      break;
   case Rank::King:
      return 4;
      break;
   case Rank::Queen:
      return 3;
      break;
   case Rank::Jack:
      return 2;
      break;
   case Rank::Nine:
      return 0;
      break;
   default:
      return 0;
      break;
   }
}

bool coinToss() {
   srand(time(NULL));
   int toss = rand() % 2;
   std::string headsOrTails;
   if(toss == 0) {
      headsOrTails = "heads";
   } 
   if(toss == 1) {
      headsOrTails = "tails";
   }
   std::cout << "Deciding who goes first based on a coin toss. Enter your prediction (heads/tails): " << std::endl;
   std::string userResponse;
   while(true) {
      getline(std::cin, userResponse);
      userResponse = stripString(userResponse);
      if(userResponse != "heads" && userResponse != "tails" ) {
         std::cout << "You must enter either 'heads' or 'tails' exactly. Please enter your guess again:" << std::endl;
         continue;
      }
      break;
   }
   if(userResponse == "heads") {
      std::cout << "Human wins the coin toss! It's your turn to go first." << std::endl;
      return true;
   } else if(userResponse == "tails") {
      std::cout << "Computer wins. The computer goes first." << std::endl;
      return false;
   }
}



int Round::promptUser() {
   int numOfOptions;
   if(humansTurn) {
      std::cout << "It is your turn to play a card. What would you like to do?" << std::endl < std::endl;
      std::cout << "Pick an action:" << std::endl << std::endl;
      std::cout << "1.  Save the game" << std::endl;
      std::cout << "2.  Make a move" << std::endl;
      std::cout << "3.  Ask for help" << std::endl;
      std::cout << "4.  Quit the game" << std::endl;
      std::cout << std::endl;
      numOfOptions = 4;
   } else {
      std::cout << "It is the computers turn to play a card. What would you like to do?"<< std::endl << std::endl;;
      std::cout << "Pick an action:" << std::endl << std::endl;
      std::cout << "1.  Save the game" << std::endl;
      std::cout << "2.  Make a move" << std::endl;
      std::cout << "3.  Quit the game" << std::endl;
      std::cout << std::endl;
      numOfOptions = 3;
   }


   std::string userAction;
   int userActionInt;
   while(true) {
      std::getline(std::cin, userAction);
      userAction = removeWhiteSpace(userAction);
      if(userAction.length() != 1) {
         std::cout << "Invalid action. You must enter a number between 1 and " << numOfOptions << ". Please try again." << std::endl;
      }

      try {
         userActionInt = std::stoi(userAction);
      } catch(const std::invalid_argument &e) {
         std::cout << "You must enter a valid number. Please try again." << std::endl;
         continue;
      }
      
      if (userActionInt < 1 || userActionInt > numOfOptions) {
         std::cout << "You must enter a number between 1 and " << numOfOptions << ". Please try again." << std::endl; 
      } else {
         break;
      }
   }
   //adjust quit game option number
   if(!humansTurn && userActionInt == 3) {
      userActionInt = 4;
   }
   
   return userActionInt;
   
}

// //  std::cout << "Pick an action:" << std::endl << std::endl;
// //    std::cout << "1.  Save the game" << std::endl;
// //    std::cout << "2.  Make a move" << std::endl;
// //    std::cout << "3.  Ask for help" << std::endl;
// //    std::cout << "4.  Quit the game" << std::endl;


// std::cout << "Pick an action:" << std::endl << std::endl;
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