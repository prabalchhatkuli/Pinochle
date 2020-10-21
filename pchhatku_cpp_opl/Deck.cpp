#include "Deck.h"

/* *********************************************************************
Function Name: Deck
Purpose: initialize the deck of cards
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1) generate 2 sets of cards for each of the characters in the FACES array in 
			combination with each of the characters in the SUITS array
Assistance Received: none
********************************************************************* */
Deck::Deck()
{

	//initializing the list of cards
	for (int i=0; i<2; i++)
	{
		//loop for faces
		for (int j = 0; j < sizeof(FACES) / sizeof(FACES[0]); j++)
		{
			//loop for suits
			for (int k = 0; k < sizeof(SUITS) / sizeof(SUITS[0]); k++)
			{
				listOfCards.push_back(new Card(FACES[j], SUITS[k]));
			}
		}
	}
}

/* *********************************************************************
Function Name: shuffleDeck
Purpose: shuffles the deck of cards
Parameters:
			none
Return Value: none
Local Variables:
			seed, unsigned value containing the seed for the current time of the system
Algorithm:
			1) generates a seed from std chronos
			2) using the std:shuffle to shuffle the vector of card objects
Assistance Received: none
********************************************************************* */
void Deck::shuffleDeck()
{
	//Taken reference from http://www.cplusplus.com/reference/algorithm/shuffle/

	//seed for random generation based on current time
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	//std::shuffle algorithm
	shuffle(listOfCards.begin(), listOfCards.end(), std::default_random_engine(seed));
}

/* *********************************************************************
Function Name: DisplayDeck
Purpose: displays the remaining cards in the deck
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1) iterates the listOfCards to display the face and suit of the cards
Assistance Received: none
********************************************************************* */
void Deck::DisplayDeck()
{
	for (deque<Card*>::iterator it = listOfCards.begin(); it != listOfCards.end(); ++it)
	{
		cout << (*it)->getCardFace() << (*it)->getCardSuit() << ",";
	}
}


/* *********************************************************************
Function Name: dealCard
Purpose: deals a card from the top of the deck
Parameters:
			none
Return Value: a Card pointer to the top card
Local Variables:
			topCard, the card from the top of the deque
Algorithm:
			1) get the front element in the deque and return
Assistance Received: none
********************************************************************* */
Card* Deck::dealCard()
{
	if (listOfCards.empty())
	{
		cout << "No more cards in stock to deal" << endl;
		exit(1);
	}

	//get the first element from Deck
	Card* topCard = listOfCards.front();
	listOfCards.pop_front();

	//return pointer to the card
	return topCard;
}

//destruct the remaining cards in the deck if any
Deck::~Deck()
{
	//for list of stock cards
	for (unsigned int i = 0; i < listOfCards.size(); i++)
	{
		delete listOfCards[i];
	}
}