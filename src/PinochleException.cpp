#include "PinochleException.h"

PinochleException::PinochleException() {
   this->message = "unspecified exception";
}

PinochleException::PinochleException(std::string message) {
   this->message = message;
}

bool PinochleException::setMessage(std::string message) {
   this->message = message;
   return true;
}

std::string PinochleException::what() {
   return message;
}