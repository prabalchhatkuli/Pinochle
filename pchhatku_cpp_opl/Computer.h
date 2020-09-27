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

};