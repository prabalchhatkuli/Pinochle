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
	void callMeld(Card*);

	//method to evaluate the meld made by player
	unsigned int evaluateMeld(Card*);

	//method to add to the player's Hand
	void addToHand(Card* newCard) { playerHand.push_back(newCard); }

	//method to add to capture pile
	void addToCapturePile(Card* captureCard) { playerCapturePile.push_back(captureCard); }

	//method to add to the round score
	void addToRoundScore(int score) { playerRoundScore += score; }

	//method to get round score
	unsigned int getRoundScore() { return playerRoundScore; }

	//method to make a move
	void makeMove();

	//method to reset all information stored for the user for a round
	void resetRoundInfo() { playerHand.clear();
							meldPile.clear();
							playedCards.clear();
							playerRoundScore = 0;
							cardToMeldMap.clear();
							meldToCardMap.clear();
							playerCapturePile.clear();}

	//method to reset capture pile

	//method to process played cards
	void processPlayedCards();

	//get the cards chosen by user during the move
	vector<Card*> getPlayedCards() { return playedCards; }

	//get the cards in hand excluding melds
	vector<Card*> getHandCards() { return playerHand; }

	//clear the played cards
	void clearPlayedCards() { playedCards.clear(); }

private:
	//variable declarations

	//constants
	const map<int, string> MELDS= {
		{1,"flush "},{2,"royal marriage"},{3,"marriage"},{4,"dix"},{5,"four Aces"},{6,"four Kings"},{7,"four Queens"},{8,"four Jacks"},{9,"Pinochle"}
	};

	const map<unsigned int, unsigned int> MELD_POINTS = {
		{1, 150},{2, 40},{3, 20},{4, 10},{5, 100},{6,80},{7, 60},{8, 40},{9, 40}
	};

	//player hand list
	vector<Card*> playerHand;

	//player score
	unsigned int playerScore;

	//player's score for the round
	unsigned int playerRoundScore;

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