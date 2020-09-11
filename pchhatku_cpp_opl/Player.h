#pragma once
#include<iostream>
#include<vector>
#include "Card.h"

using namespace std;

class Player
{
public:
	//class constructor
	Player();

	//display player cards
	void displayPlayerCards();

	//method to make a move including menu choices
	void play();

	//method to call a meld
	void callMeld();

	//method to add to the player's Hand
	void addToHand(Card*);

	//method to make a move
	void makeMove();

	//method to reset player hand
	void resetHand();

	//get the cards chosen by user during the move
	vector<Card*> getPlayedCards() { return playedCards; };

	//clear the played cards
	void clearPlayedCards() { playedCards.clear(); }

private:
	//variable declarations
	//player hand list
	vector<Card*> playerHand;

	//player score
	unsigned int playerScore;

	//capture pile
	vector<Card*> playerCapturePile;

	//meld pile: cards used for meld
	vector<Card*> meldPile;

	//chosen cards: when a player chooses one/many card at a move/meld
	vector<Card*> playedCards;

	//virtual methods necessary for human and computer
	//method to make a move


};