#pragma once
#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>
#include "Card.h"

std::string stripString(std::string str);
std::string removeWhiteSpace(std::string str);
bool isAValidCardStr(std::string str);
bool checkStrValidity(std::string str);
std::vector<std::string> splitCardsInString(std::string str);
std::vector<std::vector<std::string>> splitMeldsInString(std::string str);
Card strToCard(std::string str);

#endif