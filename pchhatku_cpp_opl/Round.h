#pragma once
#include<iostream>
#include "Game.h"
#include "Deck.h"
#include "Player.h"

using namespace std;

class Game;

class Round 
{
	friend class Player;
public:
	//default constructor
	Round();

	//overloaded constructor
	Round(Game*);

	//method to start the round
	void startRound();

	//process the moves made by players on a set of turn
	unsigned int processPlayerMoves();

	//debug::::display deck
	void displayDeck() {
		roundDeck->DisplayDeck();
	}

	//get trumpCard()
	Card* getTrumpCard() { return trumpCard; }

	//get Round deck
	Deck* getRoundDeck() { return roundDeck; }

	//get nextTurn
	unsigned int getNextPlayer() { return nextTurn; }

	//set the trump Card for the round
	void setTrumpCard(string card) { trumpCard = new Card(card[0], card[1]); }

	//set the round deck
	void setRoundDeck(vector<string> cards);

	//class destructor
	~Round()
	{
		cout << "delelting round deck" << endl;
		cout << this->roundDeck << endl;
		this->roundDeck->DisplayDeck();
		delete roundDeck;
	}

private:
	//variable to store the number of remaining turns
	int remainingTurns;

	//variable for the winner of last turn
	unsigned int nextTurn;

	//player cards drawn at each turn: contains a maximum of two cards
	vector<Card*> playedCards;

	//Pointer variable for this rounds' deck
	Deck* roundDeck;	//deck has the list of stock cards and also the taken cards

	//variable to store trump card
	Card* trumpCard;

	//method to deal a certain number of cards to a player
	void dealCardsFromDeck(Player*, unsigned int);

	//variable for pointer to current game
	Game* parentGame;
	
};