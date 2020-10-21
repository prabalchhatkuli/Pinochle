#include "Round.h"

/* *********************************************************************
Function Name: Round
Purpose: constructor for Round class
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1)Initialize member variables with NULL for pointer values and 0 for integers
Assistance Received: none
********************************************************************* */
Round::Round()
{
	//initializing the variables for the round
	roundDeck = new Deck();
	remainingTurns = 0;
	trumpCard = NULL;
	parentGame = NULL;
	nextTurn = 0;
	playedCards = {};
}

/* *********************************************************************
Function Name: Round
Purpose: overloaded constructor for Round class
Parameters:
			parentGame, a Game object. it holds the pointer to the parent game that calls the round class
Return Value: none
Local Variables:
			none
Algorithm:
			1)sets the parentGame, and creates a new Deck
Assistance Received: none
********************************************************************* */
Round::Round(Game* parentGame)
{
	//setting the parentGame
	this->parentGame = parentGame;
	
	//initializing the deck for the current round
	roundDeck = new Deck();

	//other member variables
	nextTurn = 0;
	remainingTurns = 0;
	trumpCard = NULL;
	playedCards = {};
}

/* *********************************************************************
Function Name: displayRoundInfo
Purpose: displays the round informational diagram
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1)display round data and data for each user by calling displayPlayerCards function
Assistance Received: none
********************************************************************* */
void Round::displayRoundInfo()
{
	cout<<"Round:: "<< parentGame->numRounds << endl;
	cout << "HUMAN Cards::" << endl << endl;
	parentGame->listOfPlayers[0]->displayPlayerCards(true);
	cout << endl;

	cout << endl << "COMPUTER Cards::" << endl << endl;
	parentGame->listOfPlayers[1]->displayPlayerCards(true);
	cout << endl;

	cout << "**TRUMP CARD** ::" << endl;
	cout << trumpCard->getCardFace() << trumpCard->getCardSuit() << endl;

	cout << "Stock Cards:: "; displayDeck();
	cout << endl << endl;;
}

/* *********************************************************************
Function Name: startRound
Purpose: displays and interfaces the round loop
Parameters:
			loadedTurns, number of turns remaining for the game round

Return Value: integer value based on the user choice for the round loop
Local Variables:
			dealCount and playerCount, integers. helper variable for dealing cards initially
			turnTaken, an integer. Holds the plays left in a turn.
			coinChoice, an integer. it holds the player's choice of the coin toss for a draw game.
			leadPlayerCard, a pointer to a Card object. It holds the card played by a lead player for the turn.
Algorithm:
			1) the deck is shuffled
			2) cards are distribute to the users and trump card is taken
			3) Round loop is started, where the first player is asked to play
				4) Based on the player's option the integer return is made for game saves and game quits.
				5) The cards played is processed and who won the turn is determined
				6) The cards are pushed into the capture pile of the winner
				7) The points for the turn is evaluated
				8) The played cards are then processed and cleared from buffer
				9) The winner is asked to call a meld
				10) cards are dealth at the end of round to each player
					11) if there are no more cards in the stockpile, the trumpCard is given to the player.
			12) At the end of the round the based on the round score, the starting player for next round is determined
				11) if it is a draw, a coin toss is made
Assistance Received: none
********************************************************************* */
unsigned int Round::startRound(int loadedTurns)
{

	//if the number of loaded turns is not 12 then this is a loaded game, hence, no need to manage deck
	//here 12 is a literal constant which determines the number of remainingTurns for the game
	//loadedTurns of loaded rounds range from -12 to +12, while that of a new round is  always +12.
	if (12 == loadedTurns)
	{
		//variable declarations and initializations
		//helper variable for dealing cards initially
		int dealCount;

		//**note: dealCount and playerCount will only be used if new game is started
		//initializing dealCount
		dealCount = 0;

		//shuffle cards before dealing
		roundDeck->shuffleDeck();

		//deal cards to both users
		while (dealCount < 3)
		{
			//deal 4 cards at a time to the two users
			unsigned int playerCount = 0;
			while (playerCount < 2)
			{
				dealCardsFromDeck(parentGame->listOfPlayers[playerCount], 4);
				playerCount++;
			}
			//increment dealCounter
			dealCount++;
		}

		//determining trump card for the round
		trumpCard = roundDeck->dealCard();
	}

	//the total number of consecutive turns remaining for the stock cards to finish
	remainingTurns = loadedTurns;

	cout << "Cards have been dealt the following are your cards:" << endl << endl;

	//make move
	if (parentGame->winnerLastRound == 0)
		nextTurn = 0;
	else nextTurn = 1;

	while (-12 != remainingTurns)
	{
		unsigned int turnTaken = 2;
		while (0 != turnTaken)
		{
			//display all information of the round
			displayRoundInfo();

			if (nextTurn == 0)
			{
				cout << "Turn of HUMAN:" << endl;
			}
			else
			{
				cout << "Turn of COMPUTER PLAYER:" << endl;
			}
			cout << endl;

			//variable to store the leadPlayerCard 
			vector<Card*> leadPlayerCard;

			//send the lead player card to the chase player, if leadPlayer has chosen a card, during the chase player's turn the card will be opposite of nextTurn
			if (1 == turnTaken)
			{
				leadPlayerCard.push_back(parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->getPlayedCards()[0]);
			}

			//play for the user : if the user chooses to play a card: the card will be saved to the vector of played cards in the player
			switch (parentGame->listOfPlayers[nextTurn]->play(leadPlayerCard, trumpCard))
			{
			case 0: break;
			case 1: //save game
				return 1;
			case 3: //quit game
				return 3;
			}

			if (nextTurn == 0)
			{
				cout << "Human Chose: " << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardFace() << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardSuit() << endl;
			}
			else
			{
				cout << "Computer Chose: " << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardFace() << parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardSuit() << endl;
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

		//add points of both the played cards to the winner player's round score
		parentGame->listOfPlayers[nextTurn]->addToRoundScore(parentGame->listOfPlayers[nextTurn]->getPlayedCards()[0]->getCardPoints());
		parentGame->listOfPlayers[nextTurn]->addToRoundScore(parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->getPlayedCards()[0]->getCardPoints());

		//process played Cards for both user
		parentGame->listOfPlayers[nextTurn]->processPlayedCards();
		parentGame->listOfPlayers[(0 == nextTurn) ? 1 : 0]->processPlayedCards();

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
	return 0;
}

/* *********************************************************************
Function Name: processPlayerMoves
Purpose: to process the played cards and determining the winner of the turn
Parameters:
			none

Return Value: the index of the winning player
Local Variables:
			leadCard, a card pointer. It holds the pointer to the Card of the lead player.
			chaseCard, a card pointer. It holds the pointer to the Card of the chase player.

Algorithm:
			1) The played card both players are retrieved
			2) if both cards are the same, the lead player wins the game
			3) if the lead card is of trump suit and the chase is also of trump suit but higher card points, the chase player wins
				4) else, the lead player wins
			5)if the lead card is not of trump suit ,
				6) if the chase card is of trump suit, chase player wins
				7) if chase card is not of trump suit as well, and its points are higher, the chase player wins
				8) in all other cases the leaad player wins
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: setRoundDeck
Purpose: to set the deck based on a vector
Parameters:
			cards, a vector of strings. Each string is a ASCII representation of a card
Return Value: none
Local Variables:
			vectorOfCards, a vector of Card pointers passed by value. it hold the Card objects translated from the vector
Algorithm:
			1) for each element in cards vector create a Card object
			2) send the vector of cards to the deck
Assistance Received: none
********************************************************************* */
//set the round deck
void Round::setRoundDeck(vector<string> cards)
{
	//vector to store the card objects
	vector<Card*> vectorOfCards;

	//for each card in the string create a card object and insert in the player's hand
	for (int i = 0; i < cards.size(); i++)
	{
		vectorOfCards.push_back(new Card(cards[i][0], cards[i][1]));
	}

	//set the deck with vectorOfCards
	roundDeck->setDeck(vectorOfCards);
}

/* *********************************************************************
Function Name: dealCardsFromDeck
Purpose: to deal a number of cards from deck to player hand
Parameters:
			currentPlayer, a Player object pointer. it holds the player to give the cards to.
			numberOfDraws, an integer. It holds the number of cards to give to a player
Return Value: none
Local Variables:
			tempCard, a pointer to act as a buffer to copy from deck to player's hand
Algorithm:
			1) for the numberOfDraws
			2) deal the card into the tempCard variable
			2) add the tempCard variable to the player's hand
Assistance Received: none
********************************************************************* */
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