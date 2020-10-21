#include "Computer.h"

/* *********************************************************************
Function Name: Computer
Purpose: class constructor
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1) set the player name
Assistance Received: none
********************************************************************* */
Computer::Computer()
{
	playerName = "C";
}


/* *********************************************************************
Function Name: play
Purpose: play function for Computer
Parameters:
			leadCard, vector possibly containing the card chosen by leadcard
			trumpCard, trumpCard for the round
Return Value: an integer that contains the choice made by the player during the play function
Local Variables:
			turnChoice, an integer array used to store the choice made by the user
Algorithm:
			1) take user input for menu choices
			2) route the user to other function based on the choice
Assistance Received: none
********************************************************************* */
unsigned int Computer::play(vector<Card*> leadCard, Card* trumpCard)
{
	//variablt to store what the user did in their turn
	unsigned int turnChoice;

	//ask player to make a move
	cout << "Please select an option below:" << endl;
	if(leadCard.size()==0)
		cout << "1. Save the game" << endl;
	cout << "2. Make a move" << endl;
	cout << "3. Quit the game" << endl;

	//receiving user input and validating using the parentGame method
	if (leadCard.size() == 0)
		turnChoice = validateMenuInput(1, 3);
	else
		turnChoice = validateMenuInput(2, 3);


	//switch statement for user input
	switch (turnChoice)
	{
	case 1:
		return 1;
	case 2:
		makeMove(leadCard, trumpCard);
		return 0;
	case 3:
		return 3;
	default:
		cerr << "Invalid input" << endl;
		break;
	}
}

/* *********************************************************************
Function Name: makeMove
Purpose: choose the card to play for the move
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1) determine whether the computer is lead or chase player
			2) based on that, call respective function
			3) add move cars to playedCards list
Assistance Received: none
********************************************************************* */
void Computer::makeMove(vector<Card*> leadCard, Card* trumpCard)
{
	//decide which card to play on its turn
	//if leadCard is given, the computer is the chase player
	if (leadCard.size() != 0)
	{
		//process for chase player
		//when it is the chase player of the turn - it will want to try to win the turn as inexpensively as possible, i.e., use Trump cards only when absolutely necessary.

		//find the cheapest card which will win the turn
		playedCards.push_back(getCheapestCard(leadCard[0], trumpCard));
	}
	else
	{
		//process for lead player
		//when it is the lead player of the turn - it will want to take possible melds into account before playing a card
		playedCards.push_back(getTacticalCard(trumpCard));
		cout << "Computer Chose " << playedCards[0]->getCardFace() << playedCards[0]->getCardSuit() << " because this card was the highest card it could find after saving melds with the highest point." << endl;
	}

	return;

}

//give player permission for a meld
/* *********************************************************************
Function Name: callMeld
Purpose: get 
Parameters:
			trumpCard, contains the trump card for the round
Return Value: none
Local Variables:
			none
Algorithm:
			1) call the decideMeld function, which will evaluate, decide, and execute melds for computer
Assistance Received: none
********************************************************************* */
void Computer::callMeld(Card* trumpCard)
{
	//call decideMeld
	decideMeld(trumpCard);
}
