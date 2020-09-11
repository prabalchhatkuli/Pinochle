#include "Deck.h"

Deck::Deck()
{
	cout << "Deck object is created";

	//initializing the list of cards
	for (int i=0; i<2; i++)
	{
		//loop for faces
		for (int j = 0; j < 6; j++)
		{
			//loop for suits
			for (int k = 0; k < 4; k++)
			{
				listOfCards.push_back(new Card(FACES[j], SUITS[k]));
			}
		}
	}
}

void Deck::shuffleDeck()
{
	//Taken reference from http://www.cplusplus.com/reference/algorithm/shuffle/

	//seed for random generation based on current time
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	//std::shuffle algorithm
	shuffle(listOfCards.begin(), listOfCards.end(), std::default_random_engine(seed));
}

void Deck::DisplayDeck()
{
	int counterStock = 1;
	int counterDealt = 1;
	//iterator-> it for deque
	cout << "The following is the list of cards in stock:" << endl;
	for (deque<Card*>::iterator it = listOfCards.begin(); it != listOfCards.end(); ++it)
	{
		cout << counterStock++ << ": " << (*it)->getCardFace() << ' ' << (*it)->getCardSuit() << endl;
	}

	//iterator-> it for deque
	cout << "The following is the list of dealth cards:" << endl;
	for (deque<Card*>::iterator it = dealtCards.begin(); it != dealtCards.end(); ++it)
	{
		cout << counterDealt++ << ": " << (*it)->getCardFace() << ' ' << (*it)->getCardSuit() << endl;
	}
}

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

	//add the card in the list of dealt cards
	dealtCards.push_front(topCard);

	//return pointer to the card
	return topCard;
}

Deck::~Deck()
{
	//for list of stock cards
	for (unsigned int i = 0; i < listOfCards.size(); i++)
	{
		delete listOfCards[i];
	}

	//for dealt cards
	for (unsigned int i = 0; i < dealtCards.size(); i++)
	{
		delete dealtCards[i];
	}
}