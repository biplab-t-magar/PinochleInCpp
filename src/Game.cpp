#include "Game.h"
#include <iostream>
#include "StringUtilities.cpp"

Game::Game() {
   humanScore = 0;
   computerScore = 0;
   currentRound = 1;
}

void Game::startGame() {
   std::cout << "Would you like to (1)start a new game or (2)load a previous game? Enter 1 or 2: " << std::endl << std::endl;
   std::string userResponse;
   while(true) {
      getline(std::cin, userResponse);
      userResponse = stripString(userResponse);
      if(userResponse != "1" || userResponse != "2" ) {
         std::cout << "You must enter either 1 or 2" << std::endl;
         continue;
      }
      break;
   }
   if(userResponse == "1") {
      Round round;
      while(true) {
         //start new round
         //passing arguments by reference
         round = Round();
         round.startRound(currentRound, humanScore, computerScore);
         currentRound++;
         std::cout << "Would you like to play another round? (y/n)" << std::endl;
         while(true) {
            getline(std::cin, userResponse);
            userResponse = stripString(userResponse);
            if(userResponse != "y" || userResponse != "n" ) {
               std::cout << "You must enter either y or n" << std::endl;
               continue;
            }
            break;
         }
         if(userResponse == "n") {
            std::cout << "Thank you for playing Pinochle!" << std::endl;
            break;
         }
      }
   } else if(userResponse == "2") {

   }

}