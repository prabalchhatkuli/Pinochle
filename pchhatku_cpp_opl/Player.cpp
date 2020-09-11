#include "Player.h"

//class constructor
Player::Player()
{
	cout << "Player object created" << endl;
}

//display all of the player's cards
void Player::displayPlayerCards()
{
	cout << "Current Hand::   | ";
	int i = 0;
	for (vector<Card*>::iterator it = playerHand.begin(); it != playerHand.end(); ++it)
	{
		cout <<i++<<". "<<(*it)->getCardFace() << (*it)->getCardSuit() << " | ";
		
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


	cout << "Make a move(option):" << endl << endl;
	//input card for the move
	cout << "select a card to run: 0 - " << (playerHand.size()-1) << endl;
	
	cin >> cardNumber;

	//validate cardNumber
	playedCards.push_back(playerHand[cardNumber]);
}


//
void Player::callMeld()
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
	//display hand pile and meld pile

	//once displayed choose the cards using indexes

	//push chosen cards in the playedCards vector
	
	//evaluate meld made
}

//reset hand
void Player::resetHand()
{
	playerHand.clear();
}
