#pragma once
#include<iostream>

using namespace std;

class Card
{
public:
	//default constructor
	Card();

	//overloaded constructor for card
	Card(char cardFace, char cardSuit);

	//accessors for face, suit, and points of cards
	const char getCardFace() { return cardFace; };
	const char getCardSuit() { return cardSuit; };
	unsigned int getCardPoints() { return cardPoints; }

private:

	//member variable declaration
	char cardFace;
	char cardSuit;
	unsigned int cardPoints;
	
};