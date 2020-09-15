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
	case '9': cardPoints = 0;
		break;
	case 'J': cardPoints = 2;
		break;
	case 'Q': cardPoints = 3;
		break;
	case 'K': cardPoints = 4;
		break;
	case 'X': cardPoints = 10;
		break;
	case 'A': cardPoints = 11;
		break;

	default:
		break;
	}
}