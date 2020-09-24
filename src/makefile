pinochle: MeldInstance.o MeldServices.o MeldsStorage.o Computer.o Human.o Player.o Card.o Deck.o Game.o GroupOfCards.o Pinochle.o PinochleException.o Round.o StringUtilities.o
	g++ -std=c++0x MeldInstance.o MeldServices.o MeldsStorage.o Computer.o Human.o Player.o Card.o Deck.o Game.o GroupOfCards.o Pinochle.o PinochleException.o Round.o StringUtilities.o -o pinochle

MeldInstance.o: ./MeldUtilities/MeldInstance.cpp 
	g++ -std=c++0x -c ./MeldUtilities/MeldInstance.cpp -I ./HeaderFiles

MeldServices.o: ./MeldUtilities/MeldServices.cpp
	g++ -std=c++0x -c ./MeldUtilities/MeldServices.cpp -I ./HeaderFiles

MeldsStorage.o: ./MeldUtilities/MeldsStorage.cpp
	g++ -std=c++0x -c ./MeldUtilities/MeldsStorage.cpp -I ./HeaderFiles

Computer.o: ./Players/Computer.cpp
	g++ -std=c++0x -c ./Players/Computer.cpp -I ./HeaderFiles

Human.o: ./Players/Human.cpp
	g++ -std=c++0x -c ./Players/Human.cpp  -I ./HeaderFiles

Player.o: ./Players/Player.cpp
	g++ -std=c++0x -c ./Players/Player.cpp -I ./HeaderFiles 

Card.o: Card.cpp
	g++ -std=c++0x -c Card.cpp -I ./HeaderFiles

Deck.o: Deck.cpp
	g++ -std=c++0x -c Deck.cpp -I ./HeaderFiles

Game.o: Game.cpp
	g++ -std=c++0x -c Game.cpp -I ./HeaderFiles

GroupOfCards.o: GroupOfCards.cpp
	g++ -std=c++0x -c GroupOfCards.cpp -I ./HeaderFiles

Pinochle.o: Pinochle.cpp
	g++ -std=c++0x -c Pinochle.cpp -I ./HeaderFiles

PinochleException.o: PinochleException.cpp
	g++ -std=c++0x -c PinochleException.cpp -I ./HeaderFiles

Round.o: Round.cpp
	g++ -std=c++0x -c Round.cpp -I ./HeaderFiles

StringUtilities.o: StringUtilities.cpp
	g++ -std=c++0x -c StringUtilities.cpp -I ./HeaderFiles

clean:
	rm *.o pinochle