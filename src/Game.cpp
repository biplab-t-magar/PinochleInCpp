#include "Game.h"
#include <iostream>
#include "StringUtilities.h"
#include <fstream>
#include <exception>

#define numOfPlayers 2

Game::Game() {}

void Game::start() {

   std::cout << "Welcome to Pinochle!" << std::endl;
   if(newGameOrLoad() == 1) {
      startNewGame();
   } else {
      loadGame();
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

void Game::loadGame() {
   
   //for the first round, we call Round::countinueRound()
   Round round;
   round.continueRound(currentRound, gameScores[1], gameScores[0]);

   //continue with the rest of the rounds
   while(true) {
      if(playAnotherRound() == false) {
         std::cout << "Thank you for playing Pinochle!" << std::endl;
         exit(0);
      }
      startAnotherRound();
   }
}

void Game::startAnotherRound() {

   Round round;
   std::cout << "Starting round " << currentRound << "..." << std::endl << std::endl;
   //start new round
   //passing all arguments by reference -- Round object updates these values to reflect the result of the
   round.startNewRound(currentRound, gameScores[1], gameScores[0]);
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


