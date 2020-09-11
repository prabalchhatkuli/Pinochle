#include "Card.h"

Card::Card()
{
	//cout << "Card object has been created" << endl;
	//initialize member variables
	cardFace = 0;
	cardSuit = 'z';
	cardPoints = 0;
}

Card::Card(unsigned int cardFace, char cardSuit)
{
	//cout << "Card object has been created" << endl;
	//initialize member variables
	this->cardFace = cardFace;
	this->cardSuit = cardSuit;

	switch (cardFace)
	{
	case 9: cardPoints = 0;
		break;
	case 11: cardPoints = 2;
		break;
	case 12: cardPoints = 3;
		break;
	case 13: cardPoints = 4;
		break;
	case 10: cardPoints = 10;
		break;
	case 1: cardPoints = 11;
		break;

	default:
		break;
	}
}