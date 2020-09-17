#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include <map>
#include <unordered_map>
#include "Card.h"

using namespace std;

class Player
{
public:
	//class constructor
	Player();

	//display player cards
	void displayPlayerCards(bool);

	//method to make a move including menu choices
	void play();

	//method to call a meld
	unsigned int callMeld(Card*);

	//method to evaluate the meld made by player
	bool evaluateMeld(Card*);

	//method to add to the player's Hand
	void addToHand(Card* newCard) { playerHand.push_back(newCard); }

	//method to add to capture pile
	void addToCapturePile(Card* captureCard) { playerCapturePile.push_back(captureCard); }

	//method to make a move
	void makeMove();

	//method to reset player hand
	void resetHand() { playerHand.clear(); };

	//get the cards chosen by user during the move
	vector<Card*> getPlayedCards() { return playedCards; };

	//clear the played cards
	void clearPlayedCards() { playedCards.clear(); }

private:
	//variable declarations

	//constants
	const map<int, string> MELDS= {
		{1,"flush "},{2,"royal marriage"},{3,"marriage"},{4,"dix"},{5,"four Aces"},{6,"four Kings"},{7,"four Queens"},{8,"four Jacks"},{9,"Pinochle"}
	};

	//player hand list
	vector<Card*> playerHand;

	//player score
	unsigned int playerScore;

	//capture pile
	vector<Card*> playerCapturePile;

	//meld pile: cards used for meld
	vector<Card*> meldPile;

	//melds
	//variable to store the melds and corresponding cards for those melds

	map<unsigned int, vector<vector<Card*>>> meldToCardMap;	//what cards does a meld have
	unordered_map<Card*, vector<unsigned int>> cardToMeldMap;  //which melds does a card belong to


	//chosen cards: when a player chooses one/many card at a move/meld
	vector<Card*> playedCards;

	//virtual methods necessary for human and computer
	//method to make a move


};