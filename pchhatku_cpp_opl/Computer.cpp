#include "Computer.h"

//class constructor
Computer::Computer()
{
	cout << "Computer player object created"<<endl;
	playerName = "C";
}

//play function for human
//make a move for the player
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
	cout << "input:";
	cin >> turnChoice;

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

//player makes a move
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
void Computer::callMeld(Card* trumpCard)
{
	//call decideMeld
	decideMeld(trumpCard);
}
