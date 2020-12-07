/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#pragma once
#ifndef PINOCHLE_EXCEPTION_H
#define PINOCHLE_EXCEPTION_H

#include <string>
#include <exception>

class PinochleException : public std::exception {
public:
   PinochleException();
   PinochleException(std::string message);
   std::string what();
   bool setMessage(std::string message);
   
private:
   std::string message;
};


#endif