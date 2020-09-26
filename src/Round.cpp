#include "Round.h"
#include <iostream>
#include "StringUtilities.h"
#include <fstream>

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
   roundNumber++;
}

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
      userAction = stripString(userAction);
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
      userAction = stripString(userAction);
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
         meldsString = meldsString + "[" + melds[i][j].getMeldTypeString() + "], ";
      }
   }
   return meldsString;
}

void Round::promptSaveGame() {
   std::cout << "Would you like to save your progress so far? (y/n): ";
   std::string userResponse;
   while(true) {
      getline(std::cin, userResponse);
      userResponse = stripString(userResponse);
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

}

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

   //once game data has been loaded, we can start forming objects from our serializations

   //first, create all the cards in the game by creating a dummy deck, which automatically generates all cards
   Deck deck;
   //empty out the deck into a GroupOfCards object
   GroupOfCards allCards;
   for(int i = 0; i < deck.getNumRemaining(); i++) {
      allCards.addCard(deck.takeOneFromTop());
   }
   //now, get a vector of all the stock pile cards from the string
   //this only creates "ghost" cards, with no ids, only rank and suit defined
   std::vector<Card> stockCards = strToVectorOfCards(loadedStockStr);

   stock.clear();

   //create the final stock pile by transferring over matching cards from allCards
   //it is very important here to add the cards in reverse order from how they were listed (so we need to go from right to left)
   //this is beause stock::putCardAtTop() adds the card to the "top" of the pile each time
   for(int i = stockCards.size() - 1; i >= 0; i++) {
      //take the first instance of a matching card returned from allCards
      stockCards[i] = allCards.getCardsByRankAndSuit(stockCards[i].getRank(), stockCards[i].getSuit())[0];
      //remove the card from allCards
      allCards.removeCardById(stockCards[i].getId());
      stock.putCardAtTop(stockCards[i]);
   }

   //now, we must create the player objects
   //first, deserialize the strings
   //important to receive the version of allCards returned by the Serialization::setPlayerStrings in order to reflect true stock pile
   Serialization szs[numOfPlayers];
   for(int i = 0; i < numOfPlayers; i++) {
      allCards = szs[i].setPlayerStrings(loadedHandStrs[i], loadedMeldStrs[i], loadedCaptureStrs[i], allCards, trumpSuit);
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
   
   std::cout << "Game successfully loaded!\n\n";

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
   roundNumber++;

}


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
   lines[lineNumber] = stripString(lines[lineNumber]);
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
         if(stripString(lines[lineNumber]) != "Computer:" && stripString(lines[lineNumber]) != "Human:") {
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
      lines[lineNumber] = stripString(lines[lineNumber]);
      index = lines[lineNumber].find("/");
      data = lines[lineNumber].substr(0, index);
      //game score
      try {
         gameScores[i] = std::stoi(data);
      } catch (std::invalid_argument &e) {
         throw PinochleException("This is not a valid save file. Game score is invalid");
      }
      lines[lineNumber] = lines[lineNumber].substr(index + 1);
      lines[lineNumber] = stripString(lines[lineNumber]); 
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
      lines[lineNumber] = stripString(lines[lineNumber]);
      loadedHandStrs[i] = lines[lineNumber];


      //next line
      lineNumber++;
      //get Capture Pile
      if(lines[lineNumber].substr(0, 13) != "Capture Pile:") {
         throw PinochleException("This is not a valid save file. 'Capture' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(13);
      lines[lineNumber] = stripString(lines[lineNumber]);
      loadedCaptureStrs[i] = lines[lineNumber];
      
      //next line
      lineNumber++;
      //get Melds
      if(lines[lineNumber].substr(0, 6) != "Melds:") {
         throw PinochleException("This is not a valid save file. 'Melds' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(6);
      lines[lineNumber] = stripString(lines[lineNumber]);
      loadedMeldStrs[i] = lines[lineNumber];
      //next line
      lineNumber++;
   }

   //trump card
   if(lines[lineNumber].substr(0, 11) != "Trump Card:") {
      throw PinochleException("This is not a valid save file. 'Trump Card: label is missing" );
   }
   lines[lineNumber] = lines[lineNumber].substr(11);
   lines[lineNumber] = stripString(lines[lineNumber]);
   if(lines[lineNumber].size() == 1) {
      if(!isAValidSuitStr(lines[lineNumber][0])) {
         throw PinochleException("This is not a valid save file. The trump suit specified is not valid."); 
      }
      trumpSuit = strToSuit(lines[lineNumber][0]);
   } else if(lines[lineNumber].size() == 2) {
      if(!isAValidCardStr(lines[lineNumber])) {
         throw PinochleException("This is not a valid save file. The trump card specified is invalid."); 
      }
      trumpCard.setRank(strToRank(lines[lineNumber][0]));
      trumpCard.setSuit(strToSuit(lines[lineNumber][1]));
      trumpSuit = strToSuit(lines[lineNumber][1]);
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
   lines[lineNumber] = stripString(lines[lineNumber]);
   loadedStockStr = lines[lineNumber];

   //next line
   lineNumber++;

   //next player
   if(lines[lineNumber].substr(0, 12) != "Next Player:") {
      throw PinochleException("This is not a valid save file. 'Next Player' label is missing.");
   }
   lines[lineNumber] = lines[lineNumber].substr(12);
   lines[lineNumber] = stripString(lines[lineNumber]);
   if(lines[lineNumber] != "Human" && lines[lineNumber] != "Computer") {
      throw PinochleException("This is not a valid save file. 'Next Player' must be either 'Human' or 'Computer'");
   }
   humansTurn = (lines[lineNumber] == "Human" ? true : false);

}

std::vector<std::string> Round::getSaveFileContent() {
   std::string userResponse;
   std::ifstream saveFile;
   while(true) {
      std::cout << "Enter save file name: ";
      getline(std::cin, userResponse);
      // userResponse = stripString(userResponse);
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
      nextLine = stripString(nextLine);

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

