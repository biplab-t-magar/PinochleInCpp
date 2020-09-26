#include "StringUtilities.h"
#include <string>
#include <vector>
#include "PinochleException.h"

std::string stripString(std::string str) {
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

// std::string removeWhiteSpace(std::string str) {
//    std::string cleanString = "";
//    for(int i = 0; i < str.length(); i++) {
//       if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
//          cleanString += str[i];
//       }
//    }
//    return cleanString;
// }

bool isAValidCardStr(std::string str) {
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

bool isAValidRankStr(char rank) {
   if(rank != 'A' && rank != 'X' && rank != 'K' && rank != 'Q' && rank != 'J' && rank != '9') {
      return false;
   }
   return true;
}

bool isAValidSuitStr(char suit) {
   if(suit != 'C' && suit != 'D' && suit != 'H' && suit != 'S') {
      return false;
   }
   return true;
}

bool checkCardStrValidity(std::string str) {
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

std::vector<std::string> splitCardsInString(std::string str) {
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

std::vector<Card> strToVectorOfCards(std::string str) {
   std::vector<Card> cards;
   std::vector<std::string> cardStrs = splitCardsInString(str);
   for(int i = 0; i < cardStrs.size(); i++) {
      cards.push_back(strToCard(cardStrs[i]));
   }
   return cards;
}

std::vector<std::vector<std::string>> splitMeldsInString(std::string str) {
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


Card strToCard(std::string str) {
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

Rank strToRank(char rank) {
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

Suit strToSuit(char suit) {
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

