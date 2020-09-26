#pragma once
#include "Player.h"
#include<iostream>

using namespace std;

class Human : public Player
{
public:
	//class constructor
	Human();

	//destructor
	//~Human() {};

	//function to start the game for the player
	unsigned int play(vector<Card*>, Card*);

	//method to make a move
	void makeMove();

	//method to call a meld
	void callMeld(Card*);

private:

};