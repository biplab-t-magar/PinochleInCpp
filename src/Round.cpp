#include "Round.h"
#include <iostream>
#include "StringUtilities.h"

Round::Round() {
   
}

void Round::startNewRound(int roundNumber, int &hGameScore, int &cGameScore) {
   std::cout << "Welcome to Pinochle. Let's start the game." << std::endl << std::endl << std::endl;
   //storing players for the game:
   //there is a reason for storing these players in this order
   //we can use the boolean variable humansTurn as the index for this array
   
   hRoundScore = 0;
   cRoundScore = 0;
   

   std::cout << "Distributing cards:" << std::endl;
   for(int i = 0; i < 3; i++) {
      std::cout << "Giving human four cards..." << std::endl;
      for(int i = 0; i < 4; i++) {
         players[1].takeOneCard(stock.takeOneFromTop());
      }
      std::cout << "Giving computer four cards..." << std::endl;
      for(int i = 0; i < 4; i++) {
         players[0].takeOneCard(stock.takeOneFromTop());
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
   while(players[1].numCardsInHand() != 0 || players[0].numCardsInHand() != 0) {
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
         players[1].getHelpForLeadCard();
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
         //simply proceed beyond switch statement if user wants to make a move
         break;
      case 2:
         players[1].getHelpForChaseCard(leadCard);
         break;
      case 3: 
         std::cout << "Thank you for playing Pinochle! Exiting game..." << std::endl;
         exit(0);
         break;
      default:
         break;
      }

      chaseCard = players[humansTurn].playChaseCard(leadCard);

      //this function also updates the value of humansTurn to reflect who the winner of the turn is
      findWinnerAndGivePoints(leadCard, chaseCard);

      std::cout << "The winner takes both cards for their capture pile." << std::endl;

      players[humansTurn].addToCapturePile(leadCard, chaseCard);

      //if the human won, ask if he wants help for meld or if he wants to play a meld
      if(humansTurn) {
         if(promptUserForMeld(players[1]) == 2) {
            players[1].getHelpForMeld();
         }
      }
      

      //now ask the player to play a meld
      players[humansTurn].playMeld();

      //now each player takes one card from the stock
      if(stock.getNumRemaining() > 0) {
         std::cout << "Winner of the round picks a card from the stock pile first." << std::endl;
         players[humansTurn].takeOneCard(stock.takeOneFromTop());
         
         //if the stock pile has been exhausted, take the last card from stock 
         if(stock.getNumRemaining() == 0) {
            players[!humansTurn].takeOneCard(trumpCard);
         } else {
            std::cout << "The other players also picks a card from the stock pile." << std::endl;
            players[!humansTurn].takeOneCard(stock.takeOneFromTop());
         }  
      } else {
         std::cout << "No more cards left in stock. Play until hand cards are exhausted." << std::endl;
      }
   }
   std::cout << "Round ended. " << std::endl;
   if(hRoundScore > cRoundScore) {
      std::cout << "You won this round!" << std::endl << std::endl;
   } else if(cRoundScore > hRoundScore) {
      std::cout << "You lost this round." << std::endl << std::endl;
   } else {
      std::cout << "This round was a draw" << std::endl << std::endl;
   }
   hGameScore += hRoundScore;
   cGameScore += cRoundScore;
}

void Round::findWinnerAndGivePoints(Card leadCard, Card chaseCard) {
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
   //if the human player won
   if(humansTurn) {
      hRoundScore = hRoundScore + cardPoints(leadCard) + cardPoints(chaseCard);
   } else {
      cRoundScore = cRoundScore + cardPoints(leadCard) + cardPoints(chaseCard);
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

int Round::cardPoints(Card card){
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

bool Round::coinToss() {
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
   if(userResponse == headsOrTails) {
      std::cout << "Human wins the coin toss! It's your turn to go first." << std::endl;
      return true;
   } else {
      std::cout << "Computer wins. The computer goes first." << std::endl;
      return false;
   }
}



int Round::promptUser() {
   int numOfOptions;
   if(humansTurn) {
      std::cout << "It is your turn to play a card. What would you like to do?" << std::endl << std::endl;
      std::cout << "Pick an action:" << std::endl << std::endl;
      std::cout << "1.  Make a move" << std::endl;
      std::cout << "2.  Ask for help" << std::endl;
      std::cout << "3.  Quit the game" << std::endl;
      std::cout << std::endl;
      numOfOptions = 3;
   } else {
      std::cout << "It is the computers turn to play a card. What would you like to do?"<< std::endl << std::endl;;
      std::cout << "Pick an action:" << std::endl << std::endl;
      std::cout << "1.  Ask computer to make a move" << std::endl;
      std::cout << "2.  Quit the game" << std::endl;
      std::cout << std::endl;
      numOfOptions = 2;
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
   if(!humansTurn && userActionInt == 2) {
      userActionInt = 3;
   }
   
   return userActionInt;
}

int Round::promptUserForMeld(Player human) {
   if(!human.isMeldPossible()) {
      std::cout << "You won this turn, but you do not have any cards in your hand to create melds with." << std::endl << std::endl;
      return 0;
   }
   std::cout << "You won this turn, so you can create a meld." << std::endl;
   std::cout << "Pick an action: " << std::endl;
   std::cout << "1.     Play a meld" << std::endl;
   std::cout << "2.     Ask for help" << std::endl;

   std::string userAction;
   int userActionInt;
   while(true) {
      std::getline(std::cin, userAction);
      userAction = removeWhiteSpace(userAction);
      if(userAction.length() != 1) {
         std::cout << "Invalid action. You must enter a number between 1 and 2. Please try again." << std::endl;
      }

      try {
         userActionInt = std::stoi(userAction);
      } catch(const std::invalid_argument &e) {
         std::cout << "You must enter a valid number. Please try again." << std::endl;
         continue;
      }
      
      if (userActionInt < 1 || userActionInt > 2) {
         std::cout << "You must enter a number between 1 and 2. Please try again." << std::endl; 
      } else {
         break;
      }
   }
   return userActionInt;
}

void Round::displayTable(int roundNumber, int cGameScore, int hGameScore) {
   //display each players info
   std::string player;
   for(int i = 0; i < 2; i++) {
      std::cout << "Round: " << roundNumber << std::endl << std::endl;
      std::cout << (i == 0 ? "Computer:" : "Human:")  << roundNumber << std::endl;
      std::cout << "    Score: " <<  cGameScore << " / " << cRoundScore << std::endl;
      std::cout << "    Hand: " << getHandString(players[i]) << std::endl;
      std::cout << "    Capture Pile: " << getCaptureString(players[i]) << std::endl;
      std::cout << "    Melds: " << getMeldsString(players[i]) << std::endl;
      std::cout << std::endl;
   }

   if(stock.getNumRemaining() == 0) {
      std::cout << "Trump Card: " << trumpCard.getSuitString()[0] << std::endl;
   } else {
      std::cout << "Trump Card: " << trumpCard.getShortCardStr()  << std::endl;
   }
   

   std::vector<Card> stockCards = stock.getAllRemainingCards();
   std::cout << "Stock: ";
   //the card at the top of the stock is the card at the end of the vector
   for(int i = stockCards.size() - 1; i >= 0; i++) {
      std::cout << stockCards[i].getShortCardStr() << " ";
   }
   std::cout << std::endl << std::endl;
   std::cout << "Next Player: " << (humansTurn ? "Human" : "Computer") << std::endl << std::endl;
}

std::string Round::getHandString(Player player) {   
   std::string handString;
   GroupOfCards hand = player.getHand();
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      handString = hand.getCardByPosition(i).getShortCardStr() + "(" + std::to_string(i) + ") ";
   }
   return handString;
}

std::string Round::getCaptureString(Player player) {   
   std::string captureString;
   GroupOfCards capturePile = player.getCapturePile();
   for(int i = 0; i < capturePile.getNumOfCards(); i++) {
      captureString = capturePile.getCardByPosition(i).getShortCardStr() + " ";
   }
   return captureString;
}

std::string Round::getMeldsString(Player player) {   
   std::string meldsString;
   // MeldsStorage meldsPlayed = player.getMeldsPlayed();
   std::vector<std::vector<MeldInstance>> melds = player.getMeldsPlayed().getAllMelds();
   GroupOfCards hand = player.getHand();
   Card meldCard;
   int meldCardPos;
   //for all meld types
   for(int i = 0; i < melds.size(); i++) {
      //for all instances of a meld type
      for(int j = 0; j < melds[i].size(); j++) {
         std::cout << "          ";
         //for all cards in a meld instance
         for(int k = 0; k < melds[i][j].getNumOfCards(); k++) {
            meldCard = melds[i][j].getCardByPosition(0);
            meldCardPos = hand.getCardPosition(meldCard);
            //get the string representation of the card, along with its position in hand
            meldsString = meldCard.getShortCardStr() + "(" + (meldCardPos == -1 ? "" : std::to_string(meldCardPos)) + ") "; 
         }
         //once all the cards for the melds have been displayed, print name of meld type
         meldsString = meldsString + " -- " + melds[i][j].getMeldTypeString() + ",\n";
         
      }
   }
   return meldsString;
}
