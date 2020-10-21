#include "Card.h"

/* *********************************************************************
Function Name: Card
Purpose: default constructor for initializing a card variable
Parameters:
			none
Return Value: 
			none
Local Variables:
			
Algorithm:
			1) initialize member variables
Assistance Received: none
********************************************************************* */
Card::Card()
{
	//initialize member variables
	cardFace = 0;
	cardSuit = 'z';
	cardPoints = 0;
}

/* *********************************************************************
Function Name: Card
Purpose: overloaded constructor for initializing a card variable
Parameters:
			cardFace, a character that determines face of the card
			cardSuit, a character that determines the suit of the card
Return Value:
			none
Local Variables:
			none
Algorithm:
			1) initialize member variables
			2) based on the face of the cards determine the points for the cards
Assistance Received: none
********************************************************************* */
Card::Card(char cardFace, char cardSuit)
{
	//initialize member variables
	this->cardFace = cardFace;
	this->cardSuit = cardSuit;

	//note that:: the cards can only be of the faces mentioned in the cases below
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
