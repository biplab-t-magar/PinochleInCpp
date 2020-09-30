#include "Deck.h"
#include <algorithm>
#include <random>

#define numOfRanks 6
#define numOfSuits 4
/* *********************************************************************
Function Name: Deck
Purpose: 
      Constructor for Deck class. It can be called without parameters, owing to default constructors.
      So, it acts as a default constructor as well. Initializes the deck with at least one card of each type.
Parameters: 
      numOfEachCard, the number of repetitions each distinct card should have in the deck
Return Value: 
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
      id, for storing and incrementing unique id's to all generated cards
Algorithm: 
      1) for each rank
      2)      for each suit
      3)             for each iteration of the same card
      4)                   create a new card to the cards vector, with the rank, suit, and id being currently looped over
      5)                   increment id value
Assistance Received: None
********************************************************************* */

void Deck::populate() {
   //start with id of 0
   int id = 0;
   //loop once for each rank (i.e. loop 6 times for 6 different ranks)
   //note: since Ranks is set as an enum, ranks correspond to integers in the following way:
   // Nine = 0, Jack = 1, Queen = 2, King = 3, Ten = 4, Ace = 5
   //refer to Card.h file to see enum definition
   for(int i = 0; i < numOfRanks; i++) {
      //loop once for each suit (i.e. loop 4 times for 4 different suits of each rank)
      //note: since Suits is set as an enum, suits correspond to integers in the following way:
      // Clubs = 0, Diamonds = 1, Hearts = 2, Spades = 3
      //refer to Card.h file to see enum definition
      for(int j = 0; j < numOfSuits; j++) {
         //loop once for each number of each card type (2 in default case)
         for(int k = 0; k < numOfEachCard; k++) {
               //push a new card to the vector
               //ints i and j are cast into corresponding Rank and Suit values to pass as arguments into Card constructor
               cards.push_back(Card(id, static_cast<Rank>(i), static_cast<Suit>(j)));
               id++;
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
********************************************************************* */
int Deck::getNumRemaining() const{
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


/* *********************************************************************
Function Name: getAllRemainingCards
Purpose: 
      To return copy of all the cards that are still stored in the deck object 
Parameters: 
Return Value: 
      cards, the vector containing all the cards
Assistance Received: None
********************************************************************* */
std::vector<Card> Deck::getAllRemainingCards() const{
   return cards;
}

/* *********************************************************************
Function Name: putCardAtTop
Purpose: 
      Puts the given card at the top of the deck. 
Parameters: 
      card, the card to be put on top of the deck, of Card type
Return Value: 
Assistance Received: None
********************************************************************* */
void Deck::putCardAtTop(Card card) {
   cards.push_back(card);
}


/* *********************************************************************
Function Name: clear
Purpose: 
      Removes all the card from the deck
Parameters: 
Return Value: 
Assistance Received: None
********************************************************************* */
void Deck::clear() {
   cards.clear();
}