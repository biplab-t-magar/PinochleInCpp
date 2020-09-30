#include "StringUtilities.h"
#include <string>
#include <vector>
#include "PinochleException.h"

/* *********************************************************************
Function Name: stripString
Purpose: Removes all white space from the beginning and end of a string
Parameters: 
      str, the string which is to be stripped of white space on both ends
Return Value:
      the string without white space on either end
Assistance Received: None
********************************************************************* */
std::string StringUtilities::stripString(std::string str) {
   std::string strippedString;
   //strip from front
   for(int i = 0; i < str.length(); i++) {
      if(str[i] != ' ' && str[i] != '\t') {
         strippedString = str.substr(i);
         break;
      }
   }
   //strip from back
   str = strippedString;
   for(int i = str.length() - 1; i >= 0; i--) {
      if(str[i] != ' ' && str[i] != '\t') {
         strippedString = str.substr(0, i + 1);
         break;
      }
   }
   return strippedString;
}

/* *********************************************************************
Function Name: isAValidCardStr
Purpose: checks if a string represents a valid card (does not include asterisked cards)
Parameters: 
      str, the string whose validity is to be checked
Return Value:
      true if string is a valid card, false otherwise
Assistance Received: None
********************************************************************* */
bool StringUtilities::isAValidCardStr(std::string str) {
   //all cards are represented by two characters
   if(str.size() != 2) {
      return false;
   }
   //first character of a string must be a valid rank
   if(str[0] != 'A' && str[0] != 'X' && str[0] != 'K' && str[0] != 'Q' && str[0] != 'J' && str[0] != '9') {
      return false;
   }
   //second character of a string must be a valid suit
   if(str[1] != 'C' && str[1] != 'D' && str[1] != 'H' && str[1] != 'S') {
      return false;
   }
   return true;
}

/* *********************************************************************
Function Name: isAValidRankStr
Purpose: Checks if a character represents a valid rank
Parameters: 
      rank, the character to be checked for validity
Return Value:
      true if the character represent a valid rank, false otherwise
Assistance Received: None
********************************************************************* */
bool StringUtilities::isAValidRankStr(char rank) {
   if(rank != 'A' && rank != 'X' && rank != 'K' && rank != 'Q' && rank != 'J' && rank != '9') {
      return false;
   }
   return true;
}

/* *********************************************************************
Function Name: isAValidSuitStr
Purpose: Checks if a character represents a valid suit
Parameters: 
      suit, the character to be checked for validity
Return Value:
      true if the character represent a valid suit, false otherwise
Assistance Received: None
********************************************************************* */
bool StringUtilities::isAValidSuitStr(char suit) {
   if(suit != 'C' && suit != 'D' && suit != 'H' && suit != 'S') {
      return false;
   }
   return true;
}

/* *********************************************************************
Function Name: checkCardStrValidity
Purpose: Checks if a string representa valid card (also includes asterisked cards)
Parameters: 
      str, the string to be checked for validity
Return Value:
      true if the string represents a valid card string, false otherwise
Assistance Received: None
********************************************************************* */
bool StringUtilities::checkCardStrValidity(std::string str) {
   //check if character has asterisk
   if(str.size() == 3 && str[2] != '*') {
      return false;
   } 
   //check if character has correct number of characters
   if(str.size() > 3) {
      return false;
   }
   //check if string (excluding asterisk) is a valid card
   if(!isAValidCardStr(str.substr(0, 2))) {
      return false;
   }
   return true;
}

/* *********************************************************************
Function Name: splitCardsInString
Purpose: 
      splits a string of cards into a vector of individual card strings
Parameters: 
      str, the string to be split into component card strings
Return Value:
      the vector of individual card strings
Assistance Received: None
********************************************************************* */
std::vector<std::string> StringUtilities::splitCardsInString(std::string str) {
   //extract card strings (including asterisk if present)
   //throw exception if any string besides card, white space, or * occurs

   //remove white space from both ends of the string
   str = stripString(str);
   std::string cardStr = "";
   std::vector<std::string> separatedCardStrs;
   for(int i = 0; i < str.size(); i++) {
      if(str[i] != ' ') {
         cardStr = cardStr + str[i];
      } else {
         if(checkCardStrValidity(cardStr) == false) {
            throw PinochleException("Invalid serialization." + cardStr + " is not a proper card.");
         }
         //if the string is valid, add it the result
         separatedCardStrs.push_back(cardStr);
         cardStr = "";
      }
   }
   //store last card string
   if(cardStr != "") {
      if(checkCardStrValidity(cardStr) == false) {
         throw PinochleException("Invalid serialization." + cardStr + " is not a proper card.");
      }
      //if the string is valid, add it the result
      separatedCardStrs.push_back(cardStr);
   }
   return separatedCardStrs;
}

/* *********************************************************************
Function Name: strToVectorOfCards
Purpose: 
      gets a vector of cards from a string representing a series of cards
Parameters: 
      str, the string containing the cards
Return Value:
      the vector consisting of each card extracted from the string
Assistance Received: None
********************************************************************* */
std::vector<Card> StringUtilities::strToVectorOfCards(std::string str) {
   std::vector<Card> cards;
   std::vector<std::string> cardStrs = splitCardsInString(str);
   for(int i = 0; i < cardStrs.size(); i++) {
      cards.push_back(strToCard(cardStrs[i]));
   }
   return cards;
}

/* *********************************************************************
Function Name: splitMeldsInString
Purpose: separates each meld listed in a string into individual meld strings
Parameters: 
      str, the string representation of melds
Return Value:
      vector containing each individual meld in the string, which are in turn stored as vectors of individual card strings
Assistance Received: None
********************************************************************* */
std::vector<std::vector<std::string>> StringUtilities::splitMeldsInString(std::string str) {
   //extract all individual cards of all individual melds
   //throw exception if not a valid meld string

   //remove white space from both ends of the string
   str = stripString(str);
   std::string meldStr;
   std::vector<std::vector<std::string>> allMelds;
   int meldIndex = 0;
   for(int i = 0; i < str.size(); i++) {
      if(str[i] != ',') {
         meldStr = meldStr + str[i];
      } else {
         //add a new meld entry to allMelds vector
         allMelds.push_back(std::vector<std::string>());
         try {
            allMelds[meldIndex] = splitCardsInString(meldStr);
            meldStr = "";
            meldIndex++;
         } catch(PinochleException &e) {
            throw e;
         }
      }
   }
   //store last meld in the string
   if(meldStr.size() != 0) {
      allMelds.push_back(std::vector<std::string>());
      try {
         allMelds[meldIndex] = splitCardsInString(meldStr);
      } catch(PinochleException &e) {
         throw e;
      }
   }
   return allMelds;

}

/* *********************************************************************
Function Name: strToCard
Purpose: converts a string representation of a card to a Card object
Parameters: 
      str, the string representation of the card
Return Value:
      the card object represented by the string
Assistance Received: None
********************************************************************* */
Card StringUtilities::strToCard(std::string str) {
   if(!isAValidCardStr(str)) {
      throw PinochleException(str + " is not a valid card");
   }
   Card card;
   Rank rank;
   Suit suit;
   rank = strToRank(str[0]);
   suit = strToSuit(str[1]);
   
   card.setRank(rank);
   card.setSuit(suit);

   return card;
}

/* *********************************************************************
Function Name: strToRank
Purpose: converts a string representation of a rank to a Rank enum type
Parameters: 
      rank, the caracter representation of the rank
Return Value:
      the Rank represented by the string
Assistance Received: None
********************************************************************* */
Rank StringUtilities::strToRank(char rank) {
   switch (rank) {
      case 'A':
         return Rank::Ace;
         break;
      case 'X':
         return Rank::Ten;
         break;
      case 'K':
         return Rank::King;
         break;
      case 'Q':
         return Rank::Queen;
         break;
      case 'J':
         return Rank::Jack;
         break;
      case '9':
         return Rank::Nine;
         break;
      default:
         throw PinochleException("Invalid rank");
         break;
   }
}

/* *********************************************************************
Function Name: strToSuit
Purpose: converts a string representation of a suit to a Suit enum type
Parameters: 
      suit, the caracter representation of the suit
Return Value:
      the Suit represented by the string
Assistance Received: None
********************************************************************* */
Suit StringUtilities::strToSuit(char suit) {
   switch (suit) {
      case 'C':
         return Suit::Clubs;
         break;
      case 'S':
         return Suit::Spades;
         break;
      case 'H':
         return Suit::Hearts;
         break;
      case 'D':
         return Suit::Diamonds;
         break;
      default:
         throw PinochleException("Invalid suit");
         break;
   }
}

