/************************************************************
 * Name:  Biplab Thapa Magar                                *
 * Project:  Pinochle in C++                                *
 * Class:  OPL Fall 2020                                    *
 * Date:  09/30/2020                                        *
************************************************************/
#include "PinochleException.h"

/* *********************************************************************
Function Name: PinochleException
Purpose: Construction for PinochleException class
Parameters: 
Return Value:
Assistance Received: None
********************************************************************* */
PinochleException::PinochleException() {
   this->message = "unspecified exception";
}

/* *********************************************************************
Function Name: PinochleException
Purpose: Constructor for PinochleException class, where message is initialized
Parameters: 
      card, the card to be checked for use by meld instances
Return Value:
      true if the card has been used by a meld instance, false if not
Assistance Received: None
********************************************************************* */
PinochleException::PinochleException(std::string message) {
   this->message = message;
}

/* *********************************************************************
Function Name: setMessage
Purpose: sets the exception message
Parameters: 
      message, the exception message
Return Value:
Assistance Received: None
********************************************************************* */
bool PinochleException::setMessage(std::string message) {
   this->message = message;
   return true;
}

/* *********************************************************************
Function Name: what
Purpose: gets the exception message
Parameters: 
Return Value:
      returns the exception message
Assistance Received: None
********************************************************************* */
std::string PinochleException::what() {
   return message;
   //
}
