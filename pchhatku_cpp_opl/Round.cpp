#include "Round.h"

Round::Round()
{
	//initializing the deck for the current round
	roundDeck = new Deck();
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

	//the total number of consecutive turns remaining for the stock cards to finish
	remainingTurns = 12;
	

	//determining trump card for the round
	trumpCard = roundDeck->dealCard();

	cout << "Cards have been dealt the following are your cards:" << endl << endl;

	//make move
	if (parentGame->winnerLastRound == 0)
		nextTurn = 0;
	else nextTurn = 1;

	//round loop start
	cout << "HUMAN Cards::" << endl<<endl;
	parentGame->listOfPlayers[0]->displayPlayerCards(false);
	cout << endl << endl;

	cout << endl << "COMPUTER Cards::" << endl<<endl;
	parentGame->listOfPlayers[1]->displayPlayerCards(false);
	cout << endl << endl;

	cout << "**TRUMP CARD** ::" << endl;
	cout << trumpCard->getCardFace() << trumpCard->getCardSuit() << endl << endl;

	//cout << endl << "Remaining stock cards:" << endl << endl;
	//roundDeck->DisplayDeck();

	while (-12 != remainingTurns)
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
			if (nextTurn == 0)
			{
				cout << "Human Chose" << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardFace() << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardSuit() << endl;
			}
			else
			{
				cout << "Computer Chose" << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardFace() << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardSuit() << endl;
			}
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

		//add both user's played cards to the capture pile of winner
		parentGame->listOfPlayers[nextTurn]->addToCapturePile( parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0] );
		parentGame->listOfPlayers[nextTurn]->addToCapturePile(parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->getPlayedCards()[0]);

		//add points of the cards to the winner player's round score
		parentGame->listOfPlayers[nextTurn]->addToRoundScore(parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardPoints());
		parentGame->listOfPlayers[nextTurn]->addToRoundScore(parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->getPlayedCards()[0]->getCardPoints());

		//clear played cards from the Played Cards because of the end of turns
		parentGame->listOfPlayers[nextTurn]->clearPlayedCards();
		parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->clearPlayedCards();


		//Call method for a meld: winner can make meld if they want
		parentGame->listOfPlayers[nextTurn]->callMeld(trumpCard);

		//deal cards to each player: first to the winner of the turn, then to the loser

		if (remainingTurns > 0) {
			dealCardsFromDeck(parentGame->listOfPlayers[nextTurn], 1);
			if (1 == remainingTurns)
				parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->addToHand(trumpCard);
			else
				dealCardsFromDeck(parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0], 1);
		}

		//winner starts the next set of turns
		remainingTurns--;
	}
	
	//find who has the highest score in this round
	//set the parent game's winner of last round value to the player with highest score/winner of the toss
	if (parentGame->listOfPlayers[nextTurn]->getRoundScore() == parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->getRoundScore())
	{
		//if draw: coin toss
		//human makes the call
		unsigned int coinChoice = 0;
		//seeding random generation
		srand(time(NULL));
		int coinTossResult = rand() % 2 + 1;
		cout << "Coin toss required to find out who will start another possible round" << endl;
		cout << "Because of the draw in the scores for the previous rounds, ";
		cout << "A coin has been tossed in the air....." << endl;
		cout << "Your call: 1) head || 2) tail: ";

		//input coinChoice and validate
		coinChoice = parentGame->validateMenuInput(1, 2);

		if (coinChoice == coinTossResult)
		{
			cout << "!!! You win the toss. You start the next round." << endl;
			parentGame->winnerLastRound = 0;
		}
		else
		{
			cout << "!!! You lose the toss. The Computer Starts the game." << endl;
			parentGame->winnerLastRound = 1;
		}
	}
	else if (parentGame->listOfPlayers[nextTurn]->getRoundScore() > parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->getRoundScore())
	{
		parentGame->winnerLastRound = nextTurn;
	}
	else
	{
		parentGame->winnerLastRound = (0 == nextTurn) ? 1 : 0;
	}

	//add round score of the players to game score


	//round loop end
	//reset player information for the end of round
	parentGame->listOfPlayers[0]->resetRoundInfo();
	parentGame->listOfPlayers[1]->resetRoundInfo();

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