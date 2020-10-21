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
	unsigned int startRound(int);

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

	//display all information for the round
	void displayRoundInfo();

	//get nextTurn
	const unsigned int getNextPlayer() { return nextTurn; }

	//set the trump Card for the round
	void setTrumpCard(string card) 
	{
		if(card.length() == 2 )trumpCard = new Card(card[0], card[1]);
		else trumpCard = new Card('9', card[0]);
	}

	//set the round deck
	void setRoundDeck(vector<string> cards);

	//set the next turn player : called when game is loaded
	void setNextTurn(string player) 
	{
		if ("Computer" == player) nextTurn = 1;
		else nextTurn = 0;
	}

	//class destructor
	~Round()
	{
		delete roundDeck;
		delete trumpCard;
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