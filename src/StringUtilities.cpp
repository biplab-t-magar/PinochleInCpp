#include "StringUtilities.h"
#include <string>

std::string stripString(std::string str) {
   std::string strippedString;
   //strip from front
   for(int i = 0; i < str.length(); i++) {
      if(str[i] != ' ') {
         strippedString = str.substr(i);
         break;
      }
   }
   //strip from back
   str = strippedString;
   for(int i = str.length() - 1; i >= 0; i--) {
      if(str[i] != ' ') {
         strippedString = str.substr(0, i + 1);
         break;
      }
   }
   return strippedString;
}

std::string removeWhiteSpace(std::string str) {
   std::string cleanString = "";
   for(int i = 0; i < str.length(); i++) {
      if(str[i] != ' ') {
         cleanString += str[i];
      }
   }
   return cleanString;
}