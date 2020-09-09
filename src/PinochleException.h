#pragma once
#ifndef PINOCHLE_EXCEPTION_H
#define PINOCHLE_EXCEPTION_H

#include <string>
#include <exception>

class PinochleException : public std::exception {
public:
    PinochleException();
    PinochleException(std::string message);
    bool setMessage(std::string message);
    std::string what();
private:
    std::string message;
};


#endif