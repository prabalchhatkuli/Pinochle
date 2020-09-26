#pragma once
#include "Player.h"
#include<iostream>

using namespace std;

class Computer : public Player
{
public:
	//class constructor
	Computer();

	//destructor
	//~Human() {};

	//function to start the game for the player
	unsigned int play(vector<Card*>, Card*);

	//method to make a move
	void makeMove(vector<Card*> leadCard, Card* trumpCard);

	//method to call a meld
	void callMeld(Card*);
private:
	//returns the cheapest card that will win the turn
	Card* getCheapestCard(Card*, Card*);

	//method to get a lead card
	void getTacticalCard(Card*);

	//method to get a list of playable cards
	vector<Card*> findPlayableCards(Card*, Card*);

	//compare a two cards
	bool compareTwoCards(Card*, Card*, Card*);

	//find total possible meld score from the combination of all cards in a list
	unsigned int findPossibleScores(vector<Card*>, Card*);

	//function to start recursion
	void findCombinations(vector<Card*>, unsigned int, unsigned int, unsigned int&, Card*);

	//utility function to recurse and evaluate scores
	void utilityForMeldCombinations(vector<Card*>, unsigned int, unsigned int, unsigned int, vector<Card*>, unsigned int, unsigned int&, Card*);
};