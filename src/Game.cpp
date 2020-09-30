#include "Game.h"
#include <iostream>
#include "StringUtilities.h"
#include <fstream>
#include <exception>

#define numOfPlayers 2

/* *********************************************************************
Function Name: Game
Purpose: 
      Constructor for Game class. Initializes member variables
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
Game::Game() {
   currentRound = 1;
   gameScores[0] = 0;
   gameScores[1] = 0;
}

/* *********************************************************************
Function Name: start
Purpose: 
      Starts a game of Pinochle
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
void Game::start() {
   std::cout << "Welcome to Pinochle!" << std::endl;
   if(newGameOrLoad() == 1) {
      startNewGame();
   } else {
      loadGame();
   }
}

/* *********************************************************************
Function Name: startNewGame
Purpose: 
      Begins a brand new game of Pinochle
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
void Game::startNewGame() {
   while(true) {
      startAnotherRound();
      if(playAnotherRound() == false) {
         std::cout << "Your total score this game was: " << gameScores[1];
         std::cout << "\nThe computer's total score this game was: " << gameScores[0];
         if(gameScores[1] > gameScores[0]) {
            std::cout << "\n\nYou won the game!\n\n";
         } else if(gameScores[1] < gameScores [0]) {
            std::cout << "\n\nThe computer won this game.\n\n";
         } else {
            std::cout << "\n\nThe game ended in a draw...\n\n";
         }
         std::cout << "Thank you for playing Pinochle!" << std::endl;
         exit(0);
      }
   }
}


/* *********************************************************************
Function Name: loadGame
Purpose: 
      Loads a game from a save file
Parameters: 
Return Value: 
Local Variables: 
      round, to start a round of Pinochle, of type Round
Algorithm: 
      1) Resume a round using data loaded from save file
      2) After resumed round is ended, loop the following:
      3)          Display prompt
      4)          If user wants to play another round, start a brand new round
      5)          If not, end loop
Assistance Received: None
********************************************************************* */
void Game::loadGame() {
   //for the first round, we call Round::countinueRound()
   Round round;
   round.continueRound(currentRound, gameScores[1], gameScores[0]);

   //continue with the rest of the rounds
   while(true) {
      if(playAnotherRound() == false) {
         std::cout << "Your total score this game was: " << gameScores[1];
         std::cout << "\nThe computer's total score this game was: " << gameScores[0];
         if(gameScores[1] > gameScores[0]) {
            std::cout << "\n\nYou won the game!\n\n";
         } else if(gameScores[1] < gameScores [0]) {
            std::cout << "\n\nThe computer won this game.\n\n";
         } else {
            std::cout << "\n\nThe game ended in a draw...\n\n";
         }
         std::cout << "Thank you for playing Pinochle!" << std::endl;
         exit(0);
      }
      startAnotherRound();
   }
}

/* *********************************************************************
Function Name: startAnotherRound
Purpose: 
      Sets up and starts a new Round
Parameters: 
Return Value: 
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
void Game::startAnotherRound() {

   Round round;
   std::cout << "Starting round " << currentRound << "..." << std::endl << std::endl;
   //start new round
   //passing all arguments by reference -- Round object updates these values to reflect the result of the
   round.startNewRound(currentRound, gameScores[1], gameScores[0]);
}


/* *********************************************************************
Function Name: newGameOrLoad
Purpose: 
      Prompts the user if they want to play a new game or load a previous game
Parameters: 
Return Value: 
      user's response, of type Int, could be 1 or 2 but no other value
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
int Game::newGameOrLoad() {
   std::cout << "Would you like to (1) start a new game or (2) load a previous game? Enter 1 or 2: ";
   std::string userResponse;
   while(true) {
      getline(std::cin, userResponse);
      userResponse = StringUtilities::stripString(userResponse);
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
 
/* *********************************************************************
Function Name: playAnotherRound
Purpose: asks the user if they want to play another round, and returns the response
Parameters: 
Return Value: 
      true or false, depening on the user's response, of type bool
Local Variables: 
Algorithm: 
Assistance Received: None
********************************************************************* */
bool Game::playAnotherRound(){
   std::string userResponse;
   while(true) {
      std::cout << "Would you like to play another round? (y/n)" << std::endl;
      getline(std::cin, userResponse);
      userResponse = StringUtilities::stripString(userResponse);
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


