#include "Deck.h"

Deck::Deck(int numOfEachCard):numOfEachCard(numOfEachCard) {
    //first, using the class initializer list, we initialized the number of instances of each card type

    //check if numOfEachCard is invalid
    if(numOfEachCard < 1) {
        throw PinochleException("A deck cannot have less than 1 of each card");
    }

    //populate all the card in the deck (in the default case, with 2 of each separate cards to make a total of 48 cards)
    populate();

    //shuffle the deck
    shuffle();
}

void Deck::populate() {
    
    //loop once for each rank (i.e. loop 6 times for 6 different ranks)
    //note: since Ranks is set as an enum, ranks correspond to integers in the following way:
    // Nine = 0, Jack = 1, Queen = 2, King = 3, Ten = 4, Ace = 5
    //refer to Card.h file to see enum definition
    for(int i = 0; i < 6; i++) {
        //loop once for each suit (i.e. loop 4 times for 4 different suits of each rank)
        //note: since Suits is set as an enum, suits correspond to integers in the following way:
        // Clubs = 0, Diamonds = 1, Hearts = 2, Spades = 3
        //refer to Card.h file to see enum definition
        for(int j = 0; j < 4; j++) {
            //loop once for each number of each card type (2 in default case)
            for(int k = 0; k < numOfEachCard; k++) {
                //push a new card to the vector
                //ints i and j are cast into corresponding Rank and Suit values to pass as arguments into Card constructor
                cards.push_back(Card(static_cast<Rank>(i), static_cast<Suit>(j)));
            }
        }
    }
}

void Deck::shuffle() {

}