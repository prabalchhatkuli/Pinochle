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

	//determines who the first player for the round
	void findFirstPlayerRound()
	{

	}

	//determines who the first player for the turn
	void findFirstPlayerTurn()
	{

	}

	//process the moves made by players on a set of turn
	unsigned int processPlayerMoves();

	//class destructor
	~Round()
	{
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