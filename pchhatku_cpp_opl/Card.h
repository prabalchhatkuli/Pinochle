#pragma once
#include<iostream>

using namespace std;

class Card
{
public:
	//default constructor
	Card();

	//overloaded constructor for card
	Card(unsigned int cardFace, char cardSuit);

	//accessors for face, suit, and points of cards
	unsigned int getCardFace() { return cardFace; };
	char getCardSuit() { return cardSuit; };
	unsigned int getCardPoints() { return cardPoints; }

private:

	//member variable declaration
	unsigned int cardFace;
	char cardSuit;
	unsigned int cardPoints;
	
};