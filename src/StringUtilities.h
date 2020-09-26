#pragma once
#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>
#include "Card.h"

std::string stripString(std::string str);
// std::string removeWhiteSpace(std::string str);
bool isAValidCardStr(std::string str);
bool isAValidRankStr(char rank);
bool isAValidSuitStr(char suit);
bool checkStrValidity(std::string str);
std::vector<std::string> splitCardsInString(std::string str);
std::vector<std::vector<std::string>> splitMeldsInString(std::string str);
Card strToCard(std::string str);
Rank strToRank(char rank);
Suit strToSuit(char suit);
std::vector<Card> strToVectorOfCards(std::string str);

#endif