pinochle: MeldInstance.o MeldServices.o MeldsStorage.o Computer.o Human.o Player.o Card.o Deck.o Game.o GroupOfCards.o Pinochle.o PinochleException.o Round.o StringUtilities.o
	g++ -std=c++0x MeldInstance.o MeldServices.o MeldsStorage.o Computer.o Human.o Player.o Card.o Deck.o Game.o GroupOfCards.o Pinochle.o PinochleException.o Round.o StringUtilities.o -o pinochle

MeldInstance.o: ./MeldUtilities/MeldInstance.cpp 
	g++ -std=c++0x -c ./MeldUtilities/MeldInstance.cpp 

MeldServices.o: ./MeldUtilities/MeldServices.cpp
	g++ -std=c++0x -c ./MeldUtilities/MeldServices.cpp 

MeldsStorage.o: ./MeldUtilities/MeldsStorage.cpp
	g++ -std=c++0x -c ./MeldUtilities/MeldsStorage.cpp 

Computer.o: ./Players/Computer.cpp
	g++ -std=c++0x -c ./Players/Computer.cpp 

Human.o: ./Players/Human.cpp
	g++ -std=c++0x -c ./Players/Human.cpp  

Player.o: ./Players/Player.cpp
	g++ -std=c++0x -c ./Players/Player.cpp  

Card.o: Card.cpp
	g++ -std=c++0x -c Card.cpp 

Deck.o: Deck.cpp
	g++ -std=c++0x -c Deck.cpp 

Game.o: Game.cpp
	g++ -std=c++0x -c Game.cpp 

GroupOfCards.o: GroupOfCards.cpp
	g++ -std=c++0x -c GroupOfCards.cpp 

Pinochle.o: Pinochle.cpp
	g++ -std=c++0x -c Pinochle.cpp 

PinochleException.o: PinochleException.cpp
	g++ -std=c++0x -c PinochleException.cpp 

Round.o: Round.cpp
	g++ -std=c++0x -c Round.cpp 

StringUtilities.o: StringUtilities.cpp
	g++ -std=c++0x -c StringUtilities.cpp 

clean:
	rm *.o pinochle