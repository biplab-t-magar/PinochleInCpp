#include "Round.h"
#include <iostream>
#include "StringUtilities.h"

#define numOfPlayers 2

Round::Round() {
   for(int i = 0; i < numOfPlayers; i++) {
      players[i] = NULL;
   }
}

Round::~Round() {

   for(int i = 0; i < numOfPlayers; i++) {
      delete players[i];
   }
}

void Round::startNewRound(int roundNumber, int &hGameScore, int &cGameScore) {
   std::cout << "Welcome to Pinochle. Let's start the game." << std::endl << std::endl << std::endl;
   //storing players and scores for the round:
   //there is a reason for storing these players in this order
   //we can use the boolean variable humansTurn as the index for these array

   roundScores[0] = 0;
   roundScores[1] = 0;
   players[0] = new Computer();
   players[1] = new Human();
   

   std::cout << "Distributing cards:" << std::endl;
   for(int i = 0; i < 3; i++) {
      std::cout << "Giving human four cards..." << std::endl;
      for(int i = 0; i < 4; i++) {
         players[1]->takeOneCard(stock.takeOneFromTop());
      }
      std::cout << "Giving computer four cards..." << std::endl;
      for(int i = 0; i < 4; i++) {
         players[0]->takeOneCard(stock.takeOneFromTop());
      }
   }

   trumpCard = stock.takeOneFromTop();
   trumpSuit = trumpCard.getSuit();
   players[0]->setTrumpSuit(trumpSuit);
   players[1]->setTrumpSuit(trumpSuit);
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
   MeldInstance meld;
   int promptResponse;
   //loop for each turn
   while(players[1]->numCardsInHand() != 0 || players[0]->numCardsInHand() != 0) {
      //display the game table
      displayTable(roundNumber, hGameScore, cGameScore);
      //prompt user action
      switch (promptUser()) {
      case 1:
         //simply proceed beyond switch statement if user wants to make a move
         break;
      case 2:
         players[1]->getHelpForLeadCard();
         break;
      case 3: 
         std::cout << "Thank you for playing Pinochle! Exiting game..." << std::endl;
         exit(0);
         break;
      default:
         break;
      }
      //the player whose turn it is plays the lead card
      leadCard = players[humansTurn]->playLeadCard();
      humansTurn = !humansTurn;

      //again, ask prompt user action
      switch (promptUser()) {
      case 1:
         //simply proceed beyond switch statement if user wants to make a move
         break;
      case 2:
         players[1]->getHelpForChaseCard(leadCard);
         break;
      case 3: 
         std::cout << "Thank you for playing Pinochle! Exiting game..." << std::endl;
         exit(0);
         break;
      default:
         break;
      }

      chaseCard = players[humansTurn]->playChaseCard(leadCard);

      //this function also updates the value of humansTurn to reflect who the winner of the turn is
      findWinnerAndGivePoints(leadCard, chaseCard);

      players[humansTurn]->addToCapturePile(leadCard, chaseCard);

      if(!(players[humansTurn]->isMeldPossible())) {
         std::cout << "The winner of this turn does not have any cards in hand to create melds with. Moving on to the next turn..." << std::endl << std::endl;
      } else {
         std::cout <<"Now, the winner creates a meld." << std::endl;
         //if the human won, ask if he wants help for meld or if he wants to play a meld
         if(humansTurn) {
            if(promptUserForMeld(players[1]) == 2) {
               players[1]->getHelpForMeld();
            } 
         }
         //now ask the player to play a meld
         meld = players[humansTurn]->playMeld();
         std::cout << (humansTurn ? "You" : "The computer") << " win " << meld.getMeldPoints() << " points for playing a " << meld.getMeldTypeString() << " meld.\n\n";
         roundScores[humansTurn] += meld.getMeldPoints();
      }
      

      //now each player takes one card from the stock
      if(stock.getNumRemaining() > 0) {
         std::cout << "Winner of the round picks a card from the stock pile first." << std::endl;
         players[humansTurn]->takeOneCard(stock.takeOneFromTop());
         
         //if the stock pile has been exhausted, take the trump card
         if(stock.getNumRemaining() == 0) {
            players[!humansTurn]->takeOneCard(trumpCard);
         } else {
            std::cout << "The other players also picks a card from the stock pile." << std::endl;
            players[!humansTurn]->takeOneCard(stock.takeOneFromTop());
         }  
      } else {
         std::cout << "No more cards left in stock. Play until hand cards are exhausted." << std::endl;
      }
   }
   std::cout << "Round ended. " << std::endl;
   if(roundScores[1] > roundScores[0]) {
      std::cout << "You won this round!" << std::endl << std::endl;
   } else if(roundScores[0] > roundScores[1]) {
      std::cout << "You lost this round." << std::endl << std::endl;
   } else {
      std::cout << "This round was a draw" << std::endl << std::endl;
   }
   hGameScore += roundScores[1];
   cGameScore += roundScores[0];
}

void Round::findWinnerAndGivePoints(Card leadCard, Card chaseCard) {
   //find out who wins
   if(leadCardWins(leadCard, chaseCard)) {
      //if it was the humans lead, then it would be the computer's turn during the playing of the chase card
      //that's why humansTurn == false if human played the lead card
      if(!humansTurn) {
         std::cout << "\nYour lead card has beaten the computer's chase card. You win this turn!" << std::endl;
         humansTurn = true;
      } else {
         std::cout << "\nYour chase card was beaten by the computer's lead card. You lose this turn." << std::endl;
         humansTurn = false;
      }
   } else {
      if(!humansTurn) {
         std::cout << "\nYour lead card was beaten by the computer's chase card. You lose this turn." << std::endl;
         humansTurn = false;
      } else {
         std::cout << "\nYour chase card has beaten the computer's lead card. You win this turn!" << std::endl;
         humansTurn = true;
      }
   }
   int pointsWon = cardPoints(leadCard) + cardPoints(chaseCard);
   //if the human player won
   if(humansTurn) {
      std::cout << "You take both cards for your capture pile." << std::endl;
      std::cout << "You won " << pointsWon << " points.\n\n";
      roundScores[1] = roundScores[1] + pointsWon;
   } else {
      std::cout << "The computer takes both cards for their capture pile." << std::endl;
      std::cout << "The computer won " << pointsWon << " points.\n\n";
      roundScores[0] = roundScores[0] + pointsWon;
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
   std::cout << "Deciding who goes first based on a coin toss. Enter your prediction (heads/tails): ";
   std::string userResponse;
   while(true) {
      std::cin.clear();
      getline(std::cin, userResponse);
      userResponse = stripString(userResponse);
      if(userResponse != "heads" && userResponse != "tails" ) {
         std::cout << "You must enter either 'heads' or 'tails' exactly. Please enter your guess again:";
         continue;
      }
      break;
   }

   std::cout << "The toss resulted in a " << headsOrTails << "." <<std::endl;
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
      std::cout << "It is the computer's turn to play a card. What would you like to do?"<< std::endl << std::endl;;
      std::cout << "Pick an action:" << std::endl << std::endl;
      std::cout << "1.  Ask computer to make a move" << std::endl;
      std::cout << "2.  Quit the game" << std::endl;
      std::cout << std::endl;
      numOfOptions = 2;
   }

   std::string userAction;
   int userActionInt;
   while(true) {
      std::cin.clear();
      std::getline(std::cin, userAction);
      userAction = removeWhiteSpace(userAction);
      if(userAction.length() != 1) {
         std::cout << "Invalid action. You must enter a number between 1 and " << numOfOptions << ". Please try again." << std::endl;
         continue;
      }

      
      if(userAction[0] < 48 || userAction[0] > 57) {
         std::cout << "You must enter a valid number. Please try again." << std::endl;
         continue;
      }

      try {
         userActionInt = std::stoi(userAction);
      } catch(const std::invalid_argument &e) {
         std::cout << "You must enter a valid number. Please try again." << std::endl;
         continue;
      }
      
      if (userActionInt < 1 || userActionInt > numOfOptions) {
         std::cout << "You must enter a number between 1 and " << numOfOptions << ". Please try again." << std::endl; 
         continue;
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

int Round::promptUserForMeld(Player *human) {
   std::cout << "You won this turn, so you can create a meld." << std::endl;
   std::cout << "Pick cards from your hand to create a meld: " << std::endl;
   std::cout << "-------------------------------------------------" << std::endl;
   std::cout << "Hand: " << getHandString(human) <<std::endl;
   std::cout << "Trump Suit: " << trumpCard.getSuitString() << std::endl;
   std::cout << "Melds Played: " << getMeldsString(human) << std::endl;
   std::cout << "-------------------------------------------------" << std::endl  << std::endl;
   std::cout << "Pick an action: " << std::endl;
   std::cout << "1.     Play a meld" << std::endl;
   std::cout << "2.     Ask for help" << std::endl;

   std::string userAction;
   int userActionInt;
   while(true) {
      std::cin.clear();
      std::getline(std::cin, userAction);
      userAction = removeWhiteSpace(userAction);
      if(userAction.length() != 1) {
         std::cout << "Invalid action. You must enter a number between 1 and 2. Please try again." << std::endl;
         continue;
      }

      try {
         userActionInt = std::stoi(userAction);
      } catch(const std::invalid_argument &e) {
         std::cout << "You must enter a valid number. Please try again." << std::endl;
         continue;
      }
      
      if (userActionInt < 1 || userActionInt > 2) {
         std::cout << "You must enter a number between 1 and 2. Please try again." << std::endl; 
         continue;
      } else {
         break;
      }
   }
   return userActionInt;
}

void Round::displayTable(int roundNumber, int hGameScore, int cGameScore) {
   std::cout << "\n\n---------------------------------------------------------------------------------------\n";
   std::cout << "Current Game Table: \n\n";
   //display each players info
   std::cout << "Round: " << roundNumber << std::endl << std::endl;
   for(int i = 0; i < 2; i++) {
      std::cout << (i == 0 ? "Computer:" : "Human:") << std::endl;
      std::cout << "    Score: " <<  (i == 0 ? cGameScore : hGameScore) << " / " << roundScores[i] << std::endl;
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
   for(int i = stockCards.size() - 1; i >= 0; i--) {
      std::cout << stockCards[i].getShortCardStr() << " ";
   }
   std::cout << std::endl << std::endl;
   std::cout << "Next Player: " << (humansTurn ? "Human" : "Computer") << std::endl;
   std::cout << "---------------------------------------------------------------------------------------\n\n";
}

std::string Round::getHandString(Player* player) {   
   std::string handString ="";
   GroupOfCards hand = player->getHand();
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      handString = handString + hand.getCardByPosition(i).getShortCardStr() + "(" + std::to_string(i) + ") ";
   }
   return handString;
}

std::string Round::getCaptureString(Player* player) {   
   std::string captureString ="";
   GroupOfCards capturePile = player->getCapturePile();
   for(int i = 0; i < capturePile.getNumOfCards(); i++) {
      captureString = captureString + capturePile.getCardByPosition(i).getShortCardStr() + " ";
   }
   return captureString;
}

std::string Round::getMeldsString(Player* player) {   
   std::string meldsString ="";
   // MeldsStorage meldsPlayed = player.getMeldsPlayed();
   std::vector<std::vector<MeldInstance>> melds = player->getMeldsPlayed().getAllMelds();
   GroupOfCards hand = player->getHand();
   Card meldCard;
   int meldCardPos;
   //for all meld types
   for(int i = 0; i < melds.size(); i++) {
      //for all instances of a meld type
      for(int j = 0; j < melds[i].size(); j++) {
         std::cout << " ";
         //for all cards in a meld instance
         for(int k = 0; k < melds[i][j].getNumOfCards(); k++) {
            meldCard = melds[i][j].getCardByPosition(k);
            meldCardPos = hand.getCardPosition(meldCard);
            //get the string representation of the card, along with its position in hand
            meldsString = meldsString +  meldCard.getShortCardStr() + "(" + (meldCardPos == -1 ? "" : std::to_string(meldCardPos)) + ") "; 
         }
         //once all the cards for the melds have been displayed, print name of meld type
         meldsString = meldsString + "[" + melds[i][j].getMeldTypeString() + "],";
      }
   }
   return meldsString;
}
