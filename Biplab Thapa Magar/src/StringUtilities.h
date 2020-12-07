/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/

#pragma once
#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>
#include "Card.h"

class StringUtilities {
public:
   static std::string stripString(std::string str);
   static bool isAValidCardStr(std::string str);
   static bool isAValidRankStr(char rank);
   static bool isAValidSuitStr(char suit);
   static bool checkStrValidity(std::string str);
   static bool checkCardStrValidity(std::string str);
   static std::vector<std::string> splitCardsInString(std::string str);
   static std::vector<std::vector<std::string>> splitMeldsInString(std::string str);
   static Card strToCard(std::string str);
   static Rank strToRank(char rank);
   static Suit strToSuit(char suit);
   static std::vector<Card> strToVectorOfCards(std::string str);
};

// static std::string stripString(std::string str);
// static bool isAValidCardStr(std::string str);
// static bool isAValidRankStr(char rank);
// static bool isAValidSuitStr(char suit);
// static bool checkStrValidity(std::string str);
// static std::vector<std::string> splitCardsInString(std::string str);
// static std::vector<std::vector<std::string>> splitMeldsInString(std::string str);
// static Card strToCard(std::string str);
// static Rank strToRank(char rank);
// static Suit strToSuit(char suit);
// static std::vector<Card> strToVectorOfCards(std::string str);

#endif