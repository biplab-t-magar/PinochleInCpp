// int Human::promptUser() {
//    std::cout << "Pick an action:" << std::endl << std::endl;
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
   
// }


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