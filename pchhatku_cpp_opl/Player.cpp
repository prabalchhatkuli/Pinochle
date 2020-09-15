#include "Player.h"

//class constructor
Player::Player()
{
	cout << "Player object created" << endl;
}

//display all of the player's cards
void Player::displayPlayerCards(bool showDefinitions)
{
	cout << "Current Hand::   | ";

	//i is a counter for the cards, both hand and meld ones
	int i = 0;
	if(showDefinitions) cout << "Hand:" << endl << endl;
	//displaying hand cards
	for (vector<Card*>::iterator it = playerHand.begin(); it != playerHand.end(); ++it)
	{
		cout <<i++<<". "<<(*it)->getCardFace() << (*it)->getCardSuit() << " | ";
		
	}

	if (showDefinitions) cout << endl << "Meld" << endl << endl;
	//displaying meld cards
	for (vector<Card*>::iterator it = meldPile.begin(); it != meldPile.end(); ++it)
	{
		cout << i++ << ". " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";

	}
	cout << endl;
}

//add a provided card to the player's hand
void Player::addToHand(Card* newCard)
{
	playerHand.push_back(newCard);
}


//make a move for the player
void Player::play()
{
	//variablt to store what the user did in their turn
	unsigned int turnChoice = 2;

	//ask player to make a move
	/*cout << "Please select an option below:" << endl;
	cout << "1. Save the game" << endl;
	cout << "2. Make a move" << endl;
	cout << "3. Ask for help" << endl;
	cout << "4. Quit the game" << endl;*/

	//receiving user input and validating using the parentGame method
	//turnChoice = parentGame->validateMenuInput(1, 2);

	//switch statement for user input
	switch (turnChoice)
	{
	case 1:
		break;
	case 2:
		return makeMove();
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

//player makes a move
void Player::makeMove()
{
	//variable declaration
	unsigned int cardNumber;

	displayPlayerCards(false);

	cout << "Make a move(option):" << endl << endl;
	//input card for the move
	cout << "select a card to run: 0 - " << (playerHand.size() + meldPile.size() - 1) << endl;
	
	cin >> cardNumber;

	//validate cardNumber
	//if chosen card has index less than the size of playerhand, the card is in the playerHand, otherwise in the meld pile
	if (cardNumber < playerHand.size())
	{
		playedCards.push_back(playerHand[cardNumber]);
	}
	else
	{
		playedCards.push_back(meldPile[cardNumber - playerHand.size()]);
	}
}


//
void Player::callMeld(Card* trumpCard)
{
	//variable declaration
	char isMelding;

	cout << "you are the winner. Do you want to call a meld?" << endl;
	cout << "Y/N:";
	cin >> isMelding;

	if ('n' == isMelding)
	{
		return;
	}

	cout << "Choose the cards you want to meld from the following cards" << endl;
	cout << "Advice: **Choose carefully you only get one chance**" << endl;
	
	//display hand pile and meld pile
	cout << "The following are your cards:" << endl;
	
	//display cards with hand and meld distinction
	displayPlayerCards(true);

	//once displayed choose the cards using indexes
	unsigned int numberOfMeldCards;
	unsigned int cardNumber;

	cout << endl << "How many cards would you like to choose?" << endl;
	cin >> numberOfMeldCards;

	cout << "Please enter the index of the " << numberOfMeldCards << " cards below:" << endl;
	//loop the number of times the user wants to enter the cards
	while (numberOfMeldCards)
	{
		cin >> cardNumber;
		
		//push chosen cards in the playedCards vector
		if (cardNumber < playerHand.size())
		{
			playedCards.push_back(playerHand[cardNumber]);
		}
		else
		{
			playedCards.push_back(meldPile[cardNumber - playerHand.size()]);
		}

		//decrement the meld card counters
		numberOfMeldCards--;
	}
	
	//evaluate the meld
	evaluateMeld(trumpCard);
}

//evaluates meld from the played cards
bool Player::evaluateMeld(Card* trumpCard)
{
	//sort the played cards so that it is easy for comparision based on the index of the cards
	//the comments describe how they are compared
	sort(playedCards.begin(), playedCards.end(), []( Card* &lhs,  Card* &rhs)
	{
		return (lhs)->getCardPoints() > (rhs)->getCardPoints();
	});

	cout << "sorted chosen cards" << endl;
	for (vector<Card*>::iterator it = playedCards.begin(); it != playedCards.end(); ++it)
	{
		cout << " " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";
	}


	switch (playedCards.size())
	{
	case 1:
		//Nine of Trump suit - called dix - 10 points
		if ('9' == playedCards[0]->getCardFace() && trumpCard->getCardSuit() == playedCards[0]->getCardSuit())
		{
			//add scores and evaluate the meld
			cout << "dix" << endl;
			return true;
		}
		break;
	case 2:
		//King and Queen of Trump suit - called royal marriage - 40 points
		if ('K' == playedCards[0]->getCardFace() && 'Q' == playedCards[1]->getCardFace() && playedCards[0]->getCardSuit() == playedCards[1]->getCardSuit())
		{
			if (trumpCard->getCardSuit() == playedCards[0]->getCardSuit())
			{
				cout << "ROYAL marriage" << endl;
			}
			//King and Queen of any other suit - called marriage - 20 points
			else
			{
				cout << "marriage" << endl;
			}
			return true;
		}

		//Queen of Spades and Jack of Diamonds - called Pinochle - 40 points
		if ('Q' == playedCards[0]->getCardFace() && 'S' == playedCards[0]->getCardSuit())
		{
			if ('J' == playedCards[1]->getCardFace() && 'D' == playedCards[1]->getCardSuit())
			{
				cout << "PINOCHLE" << endl;
				return true;
			}
		}
		break;
	case 4:
		//Aces of four different suits - called four Aces - 100 points
		//Kings of four different suits - called four Kings - 80 points
		//Queens of four different suits - called four Queens - 60 points
		//Jacks of four different suits - called four Jacks - 40 points

		//check if the cards are of same suits and different faces
		for (unsigned i = 0; i < 4; i++)
		{
			for (unsigned j = i + 1; j < 4; j++)
			{
				if ((playedCards[i]->getCardFace() != playedCards[j]->getCardFace())) 
				{
					if((playedCards[i]->getCardSuit() == playedCards[j]->getCardSuit()))
					return false;
				}
			}
		}

		switch (playedCards[0]->getCardFace())
		{
		case 'A':
			cout << "four Aces" << endl;
			break;
		case 'K':
			cout << "four kings" << endl;
			break;
		case 'Q':
			cout << "four queens" << endl;
			break;
		case 'J':
			cout << "four jacks" << endl;
			break;
		}

		break;
	case 5:
		//Ace, Ten, King, Queen and Jack of Trump suit - called flush - 150 points
		//run a loop to see if all of them have Trump suit
		for (vector<Card*>::iterator it = playedCards.begin(); it != playedCards.end(); ++it)
		{
			if (trumpCard->getCardSuit() != (*it)->getCardSuit())
			{
				return false;
			}
		}

		//check if they are int the order: Ace, Ten, King, Queen and Jack
		if ('A' == playedCards[0]->getCardFace() && 'X' == playedCards[1]->getCardFace() && 'K' == playedCards[2]->getCardFace())
		{
			if ('Q'==playedCards[3]->getCardFace() && 'J' ==playedCards[4]->getCardFace())
			{
				cout << "Pinocle win" << endl;
				return true;
			}
		}
		break;
	default:
		return false;
	}
	//if no cases match return false;
	return false;
}
