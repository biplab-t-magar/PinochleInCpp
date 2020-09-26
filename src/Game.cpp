#include "Game.h"
#include <iostream>
#include "StringUtilities.h"
#include <fstream>
#include <exception>

#define numOfPlayers 2

Game::Game() {
   std::cout << "Welcome to Pinochle!" << std::endl;
   while(true) {
      if(newGameOrLoad() == 1) {
         startNewGame();
      } else {
         loadGame();
      }
   }
}

void Game::startNewGame() {
   currentRound = 1;
   gameScores[0] = 0;
   gameScores[1] = 0;
   
   while(true) {
      startAnotherRound();
      if(playAnotherRound() == false) {
         std::cout << "Thank you for playing Pinochle!" << std::endl;
         exit(0);
      }
   }
}

void Game::startAnotherRound() {

   Round round;
   std::cout << "Starting round " << currentRound << "..." << std::endl << std::endl;
   //start new round
   //passing arguments by reference
   round.startNewRound(currentRound, gameScores[1], gameScores[0]);
   currentRound++;
}

int Game::newGameOrLoad() {
   std::cout << "Would you like to (1) start a new game or (2) load a previous game? Enter 1 or 2: ";
   std::string userResponse;
   while(true) {
      getline(std::cin, userResponse);
      userResponse = stripString(userResponse);
      if(userResponse != "1" && userResponse != "2" ) {
         std::cout << "You must enter either 1 or 2: ";
         continue;
      }
      break;
   }
   if(userResponse == "1") {
      return 1;
   } else {
      return 2;
   }
}
 
bool Game::playAnotherRound(){
   std::string userResponse;
   while(true) {
      std::cout << "Would you like to play another round? (y/n)" << std::endl;
      getline(std::cin, userResponse);
      userResponse = stripString(userResponse);
      if(userResponse != "y" && userResponse != "n" ) {
         std::cout << "You must enter either y or n" << std::endl;
         continue;
      }
      break;
   }
   if(userResponse == "n") {
      return false;
   } else {
      return true;
   }
}


void Game::loadGame() {
   try {
      loadGameData();
   } catch(PinochleException &e) {
      std::cout << e.what() << std::endl;
      return;
   } catch(std::exception &e) {
      std::cout << e.what() << std::endl;
      return;
   }

   std::cout << "current round" << currentRound;
   std::cout << "\nh g score" << gameScores[1];
   std::cout << "\nc g score" << gameScores[0];
   std::cout << "\nh r score" << loadedRoundScores[1];
   std::cout << "\nc r score" << loadedRoundScores[0];
   std::cout << "\nh hand" << loadedHandStrs[1];
   std::cout << "\nc hand" << loadedHandStrs[0];
   std::cout << "\nh capture" << loadedCaptureStrs[1];
   std::cout << "\nc capture" << loadedCaptureStrs[0];
   std::cout << "\nc Meld" << loadedMeldStrs[0];
   std::cout << "\nh Meld" << loadedMeldStrs[0];
   std::cout << "\nstock: " << loadedStockStr;
   std::cout << "\ntrump: " << loadedTrumpCard.getSuitString();
   std::cout << "\nnext player: " << loadedNextPlayerStr;
   
}

void Game::loadGameData() {
   std::vector<std::string> lines = getSaveFileContent();
   std::string data;
   int index;
   int lineNumber = 0;
   //first get round number
   
   if(lines[lineNumber].substr(0, 6) != "Round:") {
      throw PinochleException("This is not a valid save file");
   }
   //get round number
   lines[lineNumber] = lines[lineNumber].substr(6);
   lines[lineNumber] = stripString(lines[lineNumber]);
   try {
      currentRound = std::stoi(lines[lineNumber]);
   } catch (std::invalid_argument &e) {
      throw PinochleException("This is not a valid save file. The round number is not readable.");
   }

   //lines 1 to 10 (1- 5 in first iteration, 2-6 in second iteration)
   for(int i = 0; i < 2; i++) {
      //skip the "Computer:" / "Human:" lines
      if(lineNumber == 1 || lineNumber == (1 + 5)) {
         if(stripString(lines[lineNumber]) != "Computer:" || stripString(lines[lineNumber]) != "Human:") {
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
         gameScores[lineNumber] = std::stoi(data);
      } catch (std::invalid_argument &e) {
         throw PinochleException("This is not a valid save file. Game score is invalid");
      }
      lines[lineNumber] = lines[lineNumber].substr(index + 1);
      lines[lineNumber] = stripString(lines[lineNumber]); 
      //round score
      try {
         loadedRoundScores[lineNumber] = std::stoi(lines[lineNumber]);
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
      loadedHandStrs[lineNumber] = lines[lineNumber];


      //next line
      lineNumber++;
      //get Capture Pile
      if(lines[lineNumber].substr(0, 13) != "Capture Pile:") {
         throw PinochleException("This is not a valid save file. 'Capture' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(13);
      lines[lineNumber] = stripString(lines[lineNumber]);
      loadedCaptureStrs[lineNumber] = lines[lineNumber];
      
      //next line
      lineNumber++;
      //get Melds
      if(lines[lineNumber].substr(0, 6) != "Melds:") {
         throw PinochleException("This is not a valid save file. 'Melds' label is missing");
      }
      lines[lineNumber] = lines[lineNumber].substr(6);
      lines[lineNumber] = stripString(lines[lineNumber]);
      loadedMeldStrs[lineNumber] = lines[lineNumber];
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
      loadedTrumpCard.setSuit(strToSuit(lines[lineNumber][0]));
   } else if(lines[lineNumber].size() == 2) {
      if(!isAValidCardStr(lines[lineNumber])) {
         throw PinochleException("This is not a valid save file. The trump card specified is invalid."); 
      }
      loadedTrumpCard.setRank(strToRank(data[0]));
      loadedTrumpCard.setSuit(strToSuit(data[1]));
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
   if(lines[lineNumber] != "Human" || lines[lineNumber] != "Computer") {
      throw PinochleException("This is not a valid save file. 'Next Player' must be either 'Human' or 'Computer'");
   }
   loadedNextPlayerStr = lines[lineNumber];

}

std::vector<std::string> Game::getSaveFileContent() {
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