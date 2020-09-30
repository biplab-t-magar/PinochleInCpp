/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#include "Round.h"
#include <iostream>
#include "StringUtilities.h"
#include <fstream>

#define numOfPlayers 2

/* *********************************************************************
Function Name: Round
Purpose: The default constructor for the Round Class
Parameters: 
Return Value:
Assistance Received: None
********************************************************************* */
Round::Round() {
   for(int i = 0; i < numOfPlayers; i++) {
      players[i] = NULL;
   }
}

/* *********************************************************************
Function Name: ~Round
Purpose: The destructor for the Round class
Parameters: 
Return Value:
Assistance Received: None
********************************************************************* */
Round::~Round() {

   for(int i = 0; i < numOfPlayers; i++) {
      delete players[i];
   }
}

/* *********************************************************************
Function Name: startNewRound
Purpose: Starts a fresh round of Pinochle
Parameters: 
      roundNumber, which is the current round number; passed by reference
      hGameScore, the human player's current game score; passed by reference
      cGameScore, the computer player's current game score, passed by reference
Return Value:
Local Variables: 
Algorithm: 
      1) set round scores and game scores
      2) set up human and computer players
      3) distribute cards to both players
      4) if the game scores for computer and human are the same, ask human for coin toss call
      5) assign the player who has the higher game score or who wins the coin toss as the player who goes next 
      6) begin the round loop
      7) after round ends, display points and winner
      8) update the human game score, computer game score, and round number
Assistance Received: None
********************************************************************* */
void Round::startNewRound(int &roundNumber, int &hGameScore, int &cGameScore) {
   std::cout << "Let's begin the round." << std::endl << std::endl << std::endl;
   //storing players and scores for the round:
   //there is a reason for storing these players in this order
   //we can use the boolean variable humansTurn as the index for these array

   roundScores[0] = 0;
   roundScores[1] = 0;
   gameScores[1] = hGameScore;
   gameScores[0] = cGameScore;
   players[0] = new Computer();
   players[1] = new Human();
   this->roundNumber = roundNumber;
   

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

   beginRoundLoop();
   
   std::cout << "Round ended. " << std::endl;
   if(roundScores[1] > roundScores[0]) {
      std::cout << "You won this round!\n\n" << std::endl << std::endl;
   } else if(roundScores[0] > roundScores[1]) {
      std::cout << "You lost this round.\n\n" << std::endl << std::endl;
   } else {
      std::cout << "This round was a draw\n\n" << std::endl << std::endl;
   }
   hGameScore += roundScores[1];
   cGameScore += roundScores[0];

   std::cout << "Your score this round: " << roundScores[1];
   std::cout << "\nComputer's score this round: " << roundScores[0];
   std::cout << "\n\nYour total score so far: " << hGameScore;
   std::cout << "\nComputer's total score so far: " << cGameScore;
   std::cout << "\n\n";
   
   roundNumber++;
}

/* *********************************************************************
Function Name: beginRoundLoop()
Purpose: Runs the round loop until all player cards are exhausted
Parameters: 
Return Value:
Local Variables: 
      leadCard, to hold the leadCard thrown by a player
      chaseCard, to hold the chaseCard thrown by a player
      meld, to hold the meld instance played by a player
      promptResponse, to hold the responses made by a user
Algorithm: 
      1)loop until both players' cards are exhausted
      2)    display round table and prompt user for next step to take
      3)    player whose turn it is plays the lead card
      4)    ask the user what action to take
      5)    the other player plays the chase card, if the user decides to proceed with the round
      6)    find the winner between the lead and chase cards, and assign points to the winner of the turn
      7)    give the winner the lead and chase card to put in their capture pile
      8)    if a meld is possible from the winner's hand, ask them to play a meld
      9)    Give points for meld, if one was played
      10)   Each player takes card from stock. It no card in stock, player takes Trump Card.
                If no trump card, player's play until their cards are exhausted
      11)   if there are still cards left in the players' hands, go to step 2
Assistance Received: None
********************************************************************* */
void Round::beginRoundLoop() {
   Card leadCard;
   Card chaseCard;
   MeldInstance meld;
   int promptResponse;
   //loop for each turn
   while(players[1]->numCardsInHand() != 0 || players[0]->numCardsInHand() != 0) {
      //display the game table
      displayTable();
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
         std::cout << (humansTurn ? "You" : "The computer") << " won " << meld.getMeldPoints() << " points for playing a " << meld.getMeldTypeString() << " meld.\n\n";
         roundScores[humansTurn] += meld.getMeldPoints();
      }
      

      //now each player takes one card from the stock
      if(stock.getNumRemaining() > 0) {
         std::cout << "Winner of the round picks a card from the stock pile first." << std::endl;
         players[humansTurn]->takeOneCard(stock.takeOneFromTop());
         
         //if the stock pile has been exhausted, take the trump card
         if(stock.getNumRemaining() == 0) {
            players[!humansTurn]->takeOneCard(trumpCard);
            std::cout << "Loser of this round picks up the trump card." << std::endl;
         } else {
            std::cout << "The other players also picks a card from the stock pile." << std::endl;
            players[!humansTurn]->takeOneCard(stock.takeOneFromTop());
         }  
      } else {
         std::cout << "No more cards left in stock. Play until hand cards are exhausted." << std::endl;
      }
      promptSaveGame();

   }
}

/* *********************************************************************
Function Name: findWinnerAndGivePoints
Purpose: finds the winner between the lead card and chase card and assigns 
            points to the winner
Parameters: 
      leadCard, the lead card played in the turn
      chaseCard, the chase card played in the turn
Return Value: 
Local Variables:
      pointsWon, the points won by the winner of the turn
Algorithm: 
      1)Find out whether the chase or the lead card wins and display winner in console accordingly
      2)Add the points each card is worth, display it, and add that to the winner's round score
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: leadCardWins
Purpose: finds out whether the lead card is the winner of the turn or not
Parameters: 
      leadCard, the lead card played in the turn
      chaseCard, the chase card played in the turn
Return Value:
      true if lead card wins, false otherwise
Local Variables: 
Algorithm: 
      1) if the lead card is of trump suit
      2)       check if the chase card is of trump suit and has higher rank
      3)          if not, return true
      4)          else, return false
      5) if lead card is not of trump suit
      6)       check if the chase card is of trump suit
      7)           if yes, return false
      8)           if no, then check if chase card is of same suit as lead card
      9)                  if no, return true
      10)                 if yes, check if chase card has higher rank than lead card
      11)                     if no return false, else return true
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: cardPoints
Purpose: returns the points a card yields during a turn
Parameters: 
      card, the card whose point is to be returned
Return Value:
      the points a card is worth
Assistance Received: None
********************************************************************* */
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


/* *********************************************************************
Function Name: coinToss
Purpose: Tosses a coin and asks for user prediction
Parameters: 
Return Value:
      return true if user prediction was correct, false if not
Local Variables: 
      toss, which hold the coin toss result, in int form (0 for heads, 1 for tails)
      userResponse, holds the user response
Algorithm: 
      1) generate 0 or 1 randomly
      2) prompt user for prediciton
      3) if user prediction was correct, return true, else return false
Assistance Received: None
********************************************************************* */
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
      userResponse = StringUtilities::stripString(userResponse);
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


/* *********************************************************************
Function Name: promptUser
Purpose: Prompts user for next action to take during a turn
Parameters: 
Return Value:
      the number corresponding to the option the user picked 
Local Variables: 
      numOfOptions, the number of options to display to the user
Algorithm: 
Assistance Received: None
********************************************************************* */
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
      userAction = StringUtilities::stripString(userAction);
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

/* *********************************************************************
Function Name: promptUserForMeld
Purpose: asks the user to play a meld or ask for help
Parameters: 
      human, the pointer to the human player
Return Value:
      the number (i.e. 1 or 2) representing what action the user wants to take
Local Variables: 
      userAction, to hold user's answer to prompt
Algorithm: 
Assistance Received: None
********************************************************************* */
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
      userAction = StringUtilities::stripString(userAction);
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

/* *********************************************************************
Function Name: displayTable
Purpose: to display the current state of the round 
Parameters: 
Return Value:
Local Variables: 
      stockCards, to hold all the cards left in the stock
Algorithm: 
Assistance Received: None
********************************************************************* */
void Round::displayTable() {
   std::cout << "\n\n---------------------------------------------------------------------------------------\n";
   std::cout << "Current Game Table: \n\n";
   //display each players info
   std::cout << "Round: " << roundNumber << std::endl << std::endl;
   for(int i = 0; i < numOfPlayers; i++) {
      std::cout << (i == 0 ? "Computer:" : "Human:") << std::endl;
      std::cout << "    Score: " <<  gameScores[i] << " / " << roundScores[i] << std::endl;
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

/* *********************************************************************
Function Name: getHandString
Purpose: returns the string representation of a player's hand
Parameters: 
      player, the pointer to the player object whose hand's string representation is to be returned
Return Value:
      the string representation of the player's hand
Assistance Received: None
********************************************************************* */
std::string Round::getHandString(Player* player) {   
   std::string handString ="";
   GroupOfCards hand = player->getHand();
   for(int i = 0; i < hand.getNumOfCards(); i++) {
      handString = handString + hand.getCardByPosition(i).getShortCardStr() + "(" + std::to_string(i) + ") ";
   }
   return handString;
}

/* *********************************************************************
Function Name: getCaptureString
Purpose: returns the string representation of a player's capture pile
Parameters: 
      player, the pointer to the player object whose capture pile's string representation is to be returned
Return Value:
      the string representation of the player's capture pile
Assistance Received: None
********************************************************************* */
std::string Round::getCaptureString(Player* player) {   
   std::string captureString ="";
   GroupOfCards capturePile = player->getCapturePile();
   for(int i = 0; i < capturePile.getNumOfCards(); i++) {
      captureString = captureString + capturePile.getCardByPosition(i).getShortCardStr() + " ";
   }
   return captureString;
}

/* *********************************************************************
Function Name: getMeldsString
Purpose: to return the string representation of the player's melds
Parameters: 
      player, the pointer to the player object whose melds' string representation is to be returned
Return Value:
      the string representation of the player's melds 
Assistance Received: None
********************************************************************* */
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
         meldsString = meldsString + "[" + melds[i][j].getMeldTypeString() + "], ";
      }
   }
   return meldsString;
}

/* *********************************************************************
Function Name: promptSaveGame
Purpose: Prompts the user if they want to save the game progress 
Parameters: 
Return Value:
Assistance Received: None
********************************************************************* */
void Round::promptSaveGame() {
   std::cout << "Would you like to save your progress so far? (y/n): ";
   std::string userResponse;
   while(true) {
      getline(std::cin, userResponse);
      userResponse = StringUtilities::stripString(userResponse);
      if(userResponse != "y" && userResponse != "n" ) {
         std::cout << "You must enter either y or n: ";
         continue;
      }
      break;
   }
   std::string saveData;
   if(userResponse == "n") {
      return;
   } else {
      saveData = saveGame();
   }
   
   std::cout << "Name your save file: ";
   getline(std::cin, userResponse);
   std::ofstream saveFile(userResponse.c_str());
   saveFile << saveData;
   saveFile.close();
   std::cout << "\nGame successfully save.";
   std::cout << "\n\nThank you for playing Pinochle!\n\n";
   exit(0);
}

/* *********************************************************************
Function Name: saveGame
Purpose: Generates the string representation of the current game state
Parameters: 
Return Value:
      the string representation of the current game state
Assistance Received: None
********************************************************************* */
std::string Round::saveGame() {
   //prepare data to save to file
   std::string saveData = "";

   Serialization serializations[numOfPlayers];
   for(int i = 0; i < numOfPlayers; i++) {
      serializations[i].setPlayerObjects(players[i]->getHand(), players[i]->getMeldsPlayed(), players[i]->getCapturePile());
   }

   //round number
   saveData = saveData + "Round: " + std::to_string(roundNumber) + "\n\n";
   //player data
   for(int i = 0; i < numOfPlayers; i++) {
      saveData = saveData + (i == 0 ? "Computer:" : "Human:") + "\n";
      saveData = saveData + "   Score: " + std::to_string(gameScores[i]) + " / " + std::to_string(roundScores[i]) + "\n";
      saveData = saveData + "   Hand: " + serializations[i].getHandString() + "\n";
      saveData = saveData + "   Capture Pile: " + serializations[i].getCaptureString() + "\n";
      saveData = saveData + "   Melds: " + serializations[i].getMeldString() + "\n\n";
   }

   if(stock.getNumRemaining() == 0) {
      saveData = saveData + "Trump Card: " + trumpCard.getSuitString()[0] + "\n";
   } else {
      saveData = saveData + "Trump Card: " + trumpCard.getShortCardStr() + "\n";
   }
   
   std::vector<Card> stockCards = stock.getAllRemainingCards();
   saveData = saveData + "Stock: ";
   //the card at the top of the stock is the card at the end of the vector
   for(int i = stockCards.size() - 1; i >= 0; i--) {
      saveData = saveData + stockCards[i].getShortCardStr() + " ";
   }
   saveData = saveData + "\n\n";
   saveData = saveData + "Next Player: " + (humansTurn ? "Human" : "Computer");

   return saveData;

}

/* *********************************************************************
Function Name: contintueRound
Purpose: Continues a round by loading game state from a state file
Parameters: 
      roundNumber, the round number, passed by reference, which is to be populated by the value loaded from the save file
      hGameScore, the human game score, passed by reference, which is to be populated by the value loaded from the save file
      cGameScore, the computer game score, passed by reference, which is to be populated by the value loaded from the save file
Return Value: 
Local Variables: 
      deck, to generate all the cards in the game
      allCards, to store all the cards in the game
      cardsHolder, to temporarily hold cards
Algorithm: 
      1) get save file data
      2) set the round numbers and game scores 
      3) generate all the cards to be played in the round
      4) generate the stock cards from the stock string representation
      5) generate players' hands, capture piles, and meldsPlayed from save file string
      6) intialize players with the loaded data
      7) begin the round loop
      8) after round ends, display points and winner
      9) update the human game score, computer game score, and round number
Assistance Received: None
********************************************************************* */
void Round::continueRound(int &roundNumber, int &hGameScore, int &cGameScore) {
   try {
      loadGameData();
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
      return;
   } catch(std::exception &e) {
      std::cout << e.what() << std::endl;
      return;
   }
   roundNumber = this->roundNumber;
   hGameScore = this->gameScores[1];
   cGameScore = this->gameScores[0];


   //once game data has been loaded, we can start forming objects from our serializations

   //first, create all the cards in the game by creating a dummy deck, which automatically generates all cards
   Deck deck;
   //empty out the deck into a GroupOfCards object
   GroupOfCards allCards;
   //to hold cards temporarily
   std::vector<Card> cardsHolder;
   while(deck.getNumRemaining() > 0) {
      allCards.addCard(deck.takeOneFromTop());
   }
   //now, get a vector of all the stock pile cards from the string
   //this only creates "ghost" cards, with no ids, only rank and suit defined
   std::vector<Card> stockCards = StringUtilities::strToVectorOfCards(loadedStockStr);

   stock.clear();

   //create the final stock pile by transferring over matching cards from allCards
   //it is very important here to add the cards in reverse order from how they were listed (so we need to go from right to left)
   //this is beause stock::putCardAtTop() adds the card to the "top" of the pile each time
   for(int i = stockCards.size() - 1; i >= 0; i--) {
      //take the first instance of a matching card returned from allCards
      cardsHolder = allCards.getCardsByRankAndSuit(stockCards[i].getRank(), stockCards[i].getSuit());
      if(cardsHolder.size() == 0) {
         std::cout << "The card " << stockCards[i].getShortCardStr() << " could not be extracted. There may one or more "
            << "extra instances of this card in the serialization file. Make sure there are only two instances of this card.\n\n";
         exit(1);
      }
      stockCards[i] = cardsHolder[0];
      //remove the card from allCards
      allCards.removeCardById(stockCards[i].getId());
      stock.putCardAtTop(stockCards[i]);
   }

   //now, we must create the player objects
   //first, deserialize the strings
   //important to receive the version of allCards returned by the Serialization::setPlayerStrings in order to reflect true stock pile
   Serialization szs[numOfPlayers];
   for(int i = 0; i < numOfPlayers; i++) {
      try {
         allCards = szs[i].setPlayerStrings(loadedHandStrs[i], loadedMeldStrs[i], loadedCaptureStrs[i], allCards, trumpSuit);
      } catch(PinochleException &e) {
         std::cout << e.what() << std::endl;
         exit(1);
      }
      
   }

   //now create the player objects
   try {
      players[0] = new Computer(szs[0].getHand(), szs[0].getCapturePile(), szs[0].getMeldsPlayed(), trumpSuit);
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
   }

   try {
      players[1] = new Human(szs[1].getHand(), szs[1].getCapturePile(), szs[1].getMeldsPlayed(), trumpSuit);
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
   }
   
   std::cout << "Game successfully loaded!\n";

   std::cout << "Loading round " << roundNumber << " from the load file...\n\n";

   beginRoundLoop();
   
   std::cout << "Round ended. " << std::endl;
   
   if(roundScores[1] > roundScores[0]) {
      std::cout << "You won this round!\n\n" << std::endl << std::endl;
   } else if(roundScores[0] > roundScores[1]) {
      std::cout << "You lost this round.\n\n" << std::endl << std::endl;
   } else {
      std::cout << "This round was a draw\n\n" << std::endl << std::endl;
   }

   hGameScore += roundScores[1];
   cGameScore += roundScores[0];

   std::cout << "Your score this round: " << roundScores[1];
   std::cout << "\nComputer's score this round: " << roundScores[0];
   std::cout << "\n\nYour total score so far: " << hGameScore;
   std::cout << "\nComputer's total score so far: " << cGameScore;
   std::cout << "\n\n";

   roundNumber++;

}

/* *********************************************************************
Function Name: loadGameData
Purpose: Goes through each line in the save file and parses relevant information from it
Parameters: 
Return Value:
Assistance Received: None
********************************************************************* */
void Round::loadGameData() {
   std::vector<std::string> lines = getSaveFileContent();
   std::string data;
   int index;
   int lineNumber = 0;
   //first get round number
   
   if(lines[lineNumber].substr(0, 6) != "Round:") {
      throw PinochleException("This is not a valid save file. 'Round' label is missing.");
   }
   //get round number
   lines[lineNumber] = lines[lineNumber].substr(6);
   lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
   try {
      roundNumber = std::stoi(lines[lineNumber]);
   } catch (std::invalid_argument &e) {
      throw PinochleException("This is not a valid save file. The round number is not readable.");
   }

   //next line
   lineNumber++;

   //lines 1 to 10 (1- 5 in first iteration, 2-6 in second iteration)
   //one iteration for each player
   for(int i = 0; i < numOfPlayers; i++) {
      //skip the "Computer:" / "Human:" lines
      if(lineNumber == 1 || lineNumber == (1 + 5)) {
         if(StringUtilities::stripString(lines[lineNumber]) != "Computer:" && StringUtilities::stripString(lines[lineNumber]) != "Human:") {
            throw("This is not a valid save file. Player's label (computer/human) is missing from save file");
         }
         
      }
      //next line
      lineNumber++;
      //get scores
      if(lines[lineNumber].substr(0, 6) != "Score:") {
         throw PinochleException("This is not a valid save file. 'Score:' label is missing.");
      }
      lines[lineNumber] = lines[lineNumber].substr(6);
      lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
      index = lines[lineNumber].find("/");
      data = lines[lineNumber].substr(0, index);
      //game score
      try {
         gameScores[i] = std::stoi(data);
      } catch (std::invalid_argument &e) {
         throw PinochleException("This is not a valid save file. Game score is invalid");
      }
      lines[lineNumber] = lines[lineNumber].substr(index + 1);
      lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]); 
      //round score
      try {
         roundScores[i] = std::stoi(lines[lineNumber]);
      } catch (std::invalid_argument &e) {
         throw PinochleException("This is not a valid save file. Round score is invalid");
      }

      //next line
      lineNumber++;
      //get Hand 
      if(lines[lineNumber].substr(0, 5) != "Hand:") {
         throw PinochleException("This is not a valid save file. 'Hand' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(5);
      lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
      loadedHandStrs[i] = lines[lineNumber];


      //next line
      lineNumber++;
      //get Capture Pile
      if(lines[lineNumber].substr(0, 13) != "Capture Pile:") {
         throw PinochleException("This is not a valid save file. 'Capture' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(13);
      lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
      loadedCaptureStrs[i] = lines[lineNumber];
      
      //next line
      lineNumber++;
      //get Melds
      if(lines[lineNumber].substr(0, 6) != "Melds:") {
         throw PinochleException("This is not a valid save file. 'Melds' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(6);
      lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
      loadedMeldStrs[i] = lines[lineNumber];
      //next line
      lineNumber++;
   }

   //trump card
   if(lines[lineNumber].substr(0, 11) != "Trump Card:") {
      throw PinochleException("This is not a valid save file. 'Trump Card: label is missing" );
   }
   lines[lineNumber] = lines[lineNumber].substr(11);
   lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
   if(lines[lineNumber].size() == 1) {
      if(!StringUtilities::isAValidSuitStr(lines[lineNumber][0])) {
         throw PinochleException("This is not a valid save file. The trump suit specified is not valid."); 
      }
      trumpCard.setSuit(StringUtilities::strToSuit(lines[lineNumber][0]));
      trumpSuit = StringUtilities::strToSuit(lines[lineNumber][0]);
   } else if(lines[lineNumber].size() == 2) {
      if(!StringUtilities::isAValidCardStr(lines[lineNumber])) {
         throw PinochleException("This is not a valid save file. The trump card specified is invalid."); 
      }
      trumpCard.setRank(StringUtilities::strToRank(lines[lineNumber][0]));
      trumpCard.setSuit(StringUtilities::strToSuit(lines[lineNumber][1]));
      trumpSuit = StringUtilities::strToSuit(lines[lineNumber][1]);
   } else {
      throw PinochleException("This is not a valid save file. The Trump card specified is invalid."); 
   }

   //next line
   lineNumber++;

   //stock
   if(lines[lineNumber].substr(0, 6) != "Stock:") {
      throw PinochleException("This is not a valid save file. 'Stock' label is missing.");
   }
   lines[lineNumber] = lines[lineNumber].substr(6);
   lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
   loadedStockStr = lines[lineNumber];

   //next line
   lineNumber++;

   //next player
   if(lines[lineNumber].substr(0, 12) != "Next Player:") {
      throw PinochleException("This is not a valid save file. 'Next Player' label is missing.");
   }
   lines[lineNumber] = lines[lineNumber].substr(12);
   lines[lineNumber] = StringUtilities::stripString(lines[lineNumber]);
   if(lines[lineNumber] != "Human" && lines[lineNumber] != "Computer") {
      throw PinochleException("This is not a valid save file. 'Next Player' must be either 'Human' or 'Computer'");
   }
   humansTurn = (lines[lineNumber] == "Human" ? true : false);

}

/* *********************************************************************
Function Name: getSaveFileContent
Purpose: prompts user for save file name and returns all the lines in the ifle
Parameters: 
Return Value:
      vector of lines in the save file
Assistance Received: None
********************************************************************* */
std::vector<std::string> Round::getSaveFileContent() {
   std::string userResponse;
   std::ifstream saveFile;
   while(true) {
      std::cout << "Enter save file name: ";
      getline(std::cin, userResponse);
      try {
         saveFile.open(userResponse.c_str());
         if(!saveFile.is_open()) {
            std::cout << "Error: could not load save file." << std::endl;
            continue;
         }
         break;
      } catch(std::exception &e) {
         std::cerr << e.what() << std::endl;
      }
   }
   //first, remove all blank lines
   std::vector<std::string> fileLines;
   std::string nextLine;
   //read game data
   while(getline(saveFile, nextLine)) {
      nextLine = StringUtilities::stripString(nextLine);

      //ignore empty line
      if(nextLine.length() == 0) {
         continue;
      }
      fileLines.push_back(nextLine);
   }
   
   //now, parsing interpreting game data using Serialization object
   saveFile.close();
   return fileLines;
}

