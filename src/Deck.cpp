#include "Deck.h"
#include <algorithm>
#include <random>

/* *********************************************************************
Function Name: Deck
Purpose: 
        Constructor for Deck class. It can be called without parameters, owing to default constructors.
        So, it acts as a default constructor as well. Initializes the deck with at least one card of each type.
        The number of cards of each type depends on what is sent as argument to constructor. Default is 2, abiding
        with the rules of Pinochle. Cards are also shuffled.
Parameters: 
        numOfEachCard, the number of repetitions each distinct card should have in the deck
Return Value: 
Local Variables: 
Algorithm: 
        1) Check if the number of each card is less than 1
        2) If yes, throw exception
        3) Populate the deck with cards, taking into account the specified number of duplicates of each distinct card
        4) Shuffle the cards
Assistance Received: None
********************************************************************* */
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

/* *********************************************************************
Function Name: Populate
Purpose: 
        Populates the Deck with all the cards, each distinct card repeated as many times as specified 
Parameters: 
Return Value: 
Local Variables: 
        i, index in loop that corresponds to each Rank type
        j, index in loop that corresponds to each Suit type
        k, index in loop used to keep track of repetition of each card type 
Algorithm: 
        1) initialize i to 0
        2) if i is not less than 6, go to step 14
        3)     initialize j to 0
        4)     if j is not less than 4, go to step 12
        5)          initialize k to 0
        6)          if k is not less than numOfEachCard, go to step 10
        7)              push into cards vector a new Card object with arguments i and j respectively
        8)              increase value of k by one
        9)              go to step 6
        10)         increase value of j by 1
        11)         go to step 4
        12)     increase value of i by 1
        13)     go to step 2
        14)     end    
Assistance Received: None
********************************************************************* */

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

/* *********************************************************************
Function Name: shuffle
Purpose: 
        Shuffles the order of the cards in the deck
Parameters: 
Return Value: 
Local Variables: 
        randomEngine, to hold the object returned by default_random_engine
Algorithm: 
        1) get an instance of default_random_engine
        2) shuffle the vector
********************************************************************* */

void Deck::shuffle() {
    //intialize a random number engine, taken from c++ stl
    auto randomEngine = std::default_random_engine(std::time(NULL));
    //use shuffle function from stl to shuffle vector of Cards
    std::shuffle(std::begin(cards), std::end(cards), randomEngine);
}

/* *********************************************************************
Function Name: getNumRemaining
Purpose: 
        selector function that returns the number of cards remaining
Parameters: 
Return Value: 
        returns the number of cards currently in the deck, in size_t form
Local Variables: 
Algorithm: 
        1)return the size of the vector holding the cards
********************************************************************* */
size_t Deck::getNumRemaining() {
    //return size of vector of cards
    return cards.size();
}


/* *********************************************************************
Function Name: takeOneFromTop
Purpose: 
        removes a card from the top of the deck and returns it
Parameters: 
Return Value: 
        returns the a single card from the top of the deck
Local Variables: 
        cardAtTop, the card to be returned at the top of the deck
Algorithm: 
        1)if the number of cards left is 0 or less, throw exception
        2)copy card at the end of vector to local variable cardAtTop
        3)remove card from end of vector
        4)return local variable cardAtTop
********************************************************************* */
Card Deck::takeOneFromTop() {
    //throw error if there aren't any cards left
    if(cards.size() <= 0 ) {
        throw PinochleException("Deck/Stock pile has run out of cards");
    }

    //get card to return
    Card cardAtTop = cards[cards.size() - 1];
    //remove the card from the vector
    cards.pop_back();

    return cardAtTop;
}