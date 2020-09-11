#include "Round.h"

Round::Round()
{

}

Round::Round(Game* parentGame)
{
	cout << "This is a round class" << endl;
	//setting the parentGame
	this->parentGame = parentGame;
	
	cout << "Round object created";
	//initializing the deck for the current round
	roundDeck = new Deck();

}

void Round::startRound()
{
	//variable declarations and in
	//helper variable for dealing cards initially
	unsigned int dealCount;

	//initializing dealCount
	dealCount = 0;

	cout << "start round method started" << endl << endl;

	//shuffle cards before dealing
	roundDeck->shuffleDeck();

	//deal cards to both users
	while (dealCount < 3)
	{
		//deal 4 cards at a time to the two users
		int playerCount = 0;
		while (playerCount < 2)
		{
			dealCardsFromDeck(parentGame->listOfPlayers[playerCount], 4);
			playerCount++;
		}
		//increment dealCounter
		dealCount++;
	}

	//find the total number of remaining turns, i.e. in this case total number of cards in deck
	//remainingTurns = roundDeck->numberOfCards();
	remainingTurns = 2;

	//determining trump card for the round
	trumpCard = roundDeck->dealCard();

	cout << "Cards have been dealt the following are your cards:" << endl << endl;

	//make move
	if (parentGame->winnerLastRound == 0)
		nextTurn = 0;
	else nextTurn = 1;

	//round loop start
	cout << "HUMAN Cards::" << endl<<endl;
	parentGame->listOfPlayers[0]->displayPlayerCards();
	cout << endl << endl;

	cout << endl << "COMPUTER Cards::" << endl<<endl;
	parentGame->listOfPlayers[1]->displayPlayerCards();
	cout << endl << endl;

	cout << "**TRUMP CARD** ::" << endl;
	cout << trumpCard->getCardFace() << trumpCard->getCardSuit() << endl << endl;

	//cout << endl << "Remaining stock cards:" << endl << endl;
	//roundDeck->DisplayDeck();

	while (0 != remainingTurns)
	{
		unsigned int turnTaken = 2;
		while (0 != turnTaken)
		{
			if (nextTurn == 0)
			{
				cout << "Turn of HUMAN:" << endl;
			}
			else
			{
				cout << "Turn of COMPUTER:" << endl;
			}
			//play for the user : if the user chooses to play a card: the card will be saved to the vector
			parentGame->listOfPlayers[nextTurn]->play();
			//change the user
			nextTurn = (0 == nextTurn) ? 1 : 0;
			//evaluate the winner of the turn and set nextTurn
			turnTaken--;
		}

		//after both players have taken turn, find who won the turns
		nextTurn = processPlayerMoves();

		//scores are added to the lead player's profile
		
		if (nextTurn == 0)
		{
			cout << "HUMAN wins" << endl;
		}
		else
		{
			cout << "COMPUTER wins:" << endl;
		}

		cout <<" Player: "<< nextTurn <<" wins "<< endl << endl;

		cout << "" << endl;
		parentGame->listOfPlayers[nextTurn]->clearPlayedCards();
		parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->clearPlayedCards();


		//winner can make meld if they want
		parentGame->listOfPlayers[nextTurn]->callMeld();

		//winner starts the next set of turns
		remainingTurns--;
		

	}
	
	//round loop end
	//reset player hand
	parentGame->listOfPlayers[0]->resetHand();
	parentGame->listOfPlayers[1]->resetHand();

}

//process player moves at the end of a set of turns
//returns the index of the winning player
unsigned int Round::processPlayerMoves()
{
	//note here that: nextTurn always has the value of the leading player

	//variable declaration and initializations
	Card* leadCard = parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0];
	Card* chaseCard = parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0 ]->getPlayedCards()[0];

	
	//if both cards are same, lead player wins
	if (leadCard->getCardSuit() == chaseCard->getCardSuit() && leadCard->getCardFace() == chaseCard->getCardFace())
	{
		return nextTurn;
	}

	//if lead card is of trump suit
	else if (trumpCard->getCardSuit() == leadCard->getCardSuit())
	{
		//if chase is also of trump suit and has more point than lead then the chase player wins the turn
		if (trumpCard->getCardSuit() == chaseCard->getCardSuit() && chaseCard->getCardPoints() > leadCard->getCardPoints())
		{
			return (0 == nextTurn) ? 1 : 0;
		}
		//else the lead player wins
		else
		{
			return nextTurn;
		}
	}
	//if  lead card is not of trump suit,
	else if(trumpCard->getCardSuit() != leadCard->getCardSuit())
	{
		//if chase is of trump suit, chase wins
		if (trumpCard->getCardSuit() == chaseCard->getCardSuit())
		{
			return (0 == nextTurn) ? 1 : 0;
		}
		//if lead and chase are same suit and chase has higher card face, chase wins
		else if (leadCard->getCardSuit() == chaseCard->getCardSuit() && chaseCard->getCardPoints() > leadCard->getCardPoints())
		{
			return (0 == nextTurn) ? 1 : 0;
		}
		//otherwise lead wins
		else
		{
			return nextTurn;
		}
	}
}


//deals four cards to a player
void Round::dealCardsFromDeck(Player* currentPlayer , unsigned int numberOfDraws)
{
	//temporary variable declarations
	Card* tempCard;

	for (unsigned int i = 0; i < numberOfDraws; i++)
	{
		tempCard = roundDeck->dealCard();
		currentPlayer->addToHand(tempCard);
	}
}