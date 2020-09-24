#include "Serialization.h"

Serialization::Serialization(){
   playerCardsEntered = false;
   playerDataEntered = false;
}

bool Serialization::getPlayerCards(GroupOfCards hand, MeldsStorage meldsPlayed, GroupOfCards capturePile) {
   

   //first 

   playerCardsEntered = true;
   return true;
}

bool Serialization::getPlayerData(std::string handSerialization, std::string meldsSerialization, std::string captureSerialization) {
   
   playerDataEntered = true;
   return true;
}



std::string Serialization::getHandSerialization() {
   if(playerCardsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return handSerialization;
}
std::string Serialization::getCaptureSerialization() {
   if(playerCardsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return captureSerialization;
}
std::string Serialization::getMeldSerialization() {
   if(playerCardsEntered == false) {
      throw PinochleException("Player cards have not been entered yet");
   }
   return meldSerialization;
}

GroupOfCards getHand();
MeldsStorage getMeldsPlayed();
GroupOfCards getCapturePile();


void Serialization 