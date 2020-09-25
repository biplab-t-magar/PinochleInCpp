#include "Game.h"
#include <iostream>
#include "StringUtilities.h"

Game::Game() {
   if(newGameOrLoad() == 1) {
      startNewGame();
   } else {
      loadGame();
   }
}

void Game::startNewGame() {
   currentRound = 1;
   humanScore = 0;
   computerScore = 0;
   
   while(true) {
      startAnotherRound();
      if(playAnotherRound() == false) {
         break;
      }
   }

   std::cout << "Thank you for playing Pinochle!" << std::endl;
}

void Game::startAnotherRound() {
   Round round;
   
   std::cout << "Starting round " << currentRound << "..." << std::endl << std::endl;
   //start new round
   //passing arguments by reference
   round.startNewRound(currentRound, humanScore, computerScore);
   currentRound++;
}

int Game::newGameOrLoad() {
   std::cout << "Welcome to Pinochle!" << std::endl;
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
      std::string userResponse;
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

}