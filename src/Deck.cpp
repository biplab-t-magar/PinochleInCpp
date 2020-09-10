#include "Deck.h"

Deck::Deck(int numOfEachCard):numOfEachCard(numOfEachCard) {
    //first, using the class initializer list, we initialized the number of instances of each card type

    //populate all the card in the deck (with 2 of each separate cards to make a total of 48 cards)
    populate();

    //shuffle the deck
    shuffle();
}

void Deck::populate() {
    
}

void Deck::shuffle() {
    
}