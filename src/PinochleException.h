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