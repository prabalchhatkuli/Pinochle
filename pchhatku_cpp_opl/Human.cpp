#include "Human.h"

//class constructor
Human::Human()
{
	cout << "Human player object created" << endl;;
	playerName = "H";
}

//play function for human
//make a move for the player
unsigned int Human::play(vector<Card*> leadCard, Card* trumpCard)
{
	//variablt to store what the user did in their turn
	unsigned int turnChoice;

	//ask player to make a move
	cout << "Please select an option below:" << endl;
	if (leadCard.size() == 0)	//this is done because only the lead player can save the game
		cout << "1. Save the game" << endl;
	cout << "2. Make a move" << endl;
	cout << "3. Ask for help (Remember you can only ask for help once)" << endl;
	cout << "4. Quit the game" << endl;

	//receiving user input and validating using the parentGame method
	if (leadCard.size() == 0)
		turnChoice = validateMenuInput(1, 4);
	else
		turnChoice = validateMenuInput(2, 4);

	//switch statement for user input
	switch (turnChoice)
	{
	case 1:
		return 1;
	case 2:
		makeMove();
		return 0;
	case 3:
		//implementation of help function for human
		Card* temp_card;

		//if the player is chase
		if (leadCard.size() != 0)
		{
			temp_card = getCheapestCard(leadCard[0], trumpCard);
		}
		else //if the player is not chase: i.e. lead
		{
			temp_card = getTacticalCard(trumpCard);
			cout << "It is recommended that you choose " << temp_card->getCardFace() << temp_card->getCardSuit() << " because it is the best card you can play, after saving possible melds"<<endl;
		}
		playedCards.clear();
		//call make move after the help
		makeMove();
		return 0;
	case 4:
		return 3;
	default:
		cerr << "Invalid input" << endl;
		break;
	}
}

//player makes a move
void Human::makeMove()
{
	//variable declaration
	unsigned int cardNumber;

	displayPlayerCards(false);
	cout << "your score till now: " << playerRoundScore << "/" << playerScore << endl;
	cout << "Make a move(option):" << endl << endl;
	//input card for the move
	cout << "select a card to run: 0 - " << (playerHand.size() + meldPile.size() - 1) << endl;

	cardNumber = validateMenuInput(0, (playerHand.size() + meldPile.size() - 1));

	//validate cardNumber
	//if chosen card has index less than the size of playerhand, the card is in the playerHand, otherwise in the meld pile
	//the card needs to be inserted into the playedCards and removed from the hand or the meld
	if (cardNumber < playerHand.size())
	{
		playedCards.push_back(playerHand[cardNumber]);
		//playerHand.erase(playerHand.begin() + cardNumber);
	}
	else
	{
		playedCards.push_back(meldPile[cardNumber - playerHand.size()]);
		//meldPile.erase(meldPile.begin() + (cardNumber - playerHand.size()));
	}
}

//give player permission for a meld
void Human::callMeld(Card* trumpCard)
{
	//variable declaration

	cout << "you are the winner. Do you want to call a meld?" << endl;
	cout << "1: Yes \t";
	cout << "2: No" << endl;

	if (2 == validateMenuInput(1, 2))
	{
		return;
	}
	cout << endl;

	cout << "Do you need help in making a meld?" << endl;
	cout << "1: Yes \t";
	cout << "2: No, thank you" << endl;

	if (1 == validateMenuInput(1, 2))
	{
		cout << endl;
		decideMeld(trumpCard);
		cout << endl;

		//ask if user still wants to continue
		cout << "Do you wish to continue making melds?" << endl;
		cout << "1: Yes \t";
		cout << "2: No, thank you" << endl;

		if (2 == validateMenuInput(1, 2))
			return;
		cout << endl;
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
	numberOfMeldCards = validateMenuInput(1, (playerHand.size() + meldPile.size()));

	cout << "Please enter the index of the " << numberOfMeldCards << " cards, one by one, below:" << endl;
	vector<unsigned int> listOfChosenIndex;

	//loop the number of times the user wants to enter the cards
	while (numberOfMeldCards)
	{
		cardNumber = validateMenuInput(0, (playerHand.size() + meldPile.size() - 1));

		//push chosen cards in the playedCards vector
		if (find(listOfChosenIndex.begin(), listOfChosenIndex.end(), cardNumber) == listOfChosenIndex.end())
		{
			if (cardNumber < playerHand.size())
			{
				playedCards.push_back(playerHand[cardNumber]);
			}
			else
			{
				playedCards.push_back(meldPile[cardNumber - playerHand.size()]);
			}
			//insert the card index in a list
			listOfChosenIndex.push_back(cardNumber);

			//decrement the meld card counters
			numberOfMeldCards--;

		}
		else
		{
			cerr << "The card has already been chosen, please choose another one." << endl;
		}
	}

	//evaluate the meld
	unsigned int meldIndex = evaluateMeld(trumpCard);

	//empty the played cards
	playedCards.clear();

	//add the score to the current round score
	if(0 != meldIndex)
		playerRoundScore += MELD_POINTS.at(meldIndex); //.at() is used because MELD_POINT is a constant

	//process the move of card and such
	if (0 != meldIndex)
	{   // meld can be made
		//if the cards are in hand, remove the chosen cards from hand and insert into meld pile and card to meld map,
		//if the cards is in the meld pile, add to the back of the meld list of the card in the card to meld map

		//sort the list of chosen index
		sort(listOfChosenIndex.begin(), listOfChosenIndex.end());

		//counter for number of meld cards to remove from the player hand
		unsigned int removeCount = 0;

		//vector to collect the card belonging to a particular meld
		vector<Card*> cardsOfAMeld;

		//iterate the list of indexes
		for (vector<unsigned int>::iterator it = listOfChosenIndex.begin(); it != listOfChosenIndex.end(); ++it)
		{
			//if the card is in meld pile
			if ((*it - removeCount) >= playerHand.size())
			{
				unordered_map<Card*, vector<unsigned int>>::iterator foundCard = cardToMeldMap.find(meldPile[*it - playerHand.size() - removeCount]);

				//add the card to the list for this meld
				cardsOfAMeld.push_back(meldPile[*it - playerHand.size() - removeCount]);

				//if card is found in the card to meld map, then it is also in the meld pile, which means no need to remove from any pile
				if (foundCard != cardToMeldMap.end())
				{
					//append to the index in the cardToMeldMap
					(foundCard->second).push_back(meldIndex);
				}
			}
			else //the card is in the hand pile, we need to remove it from the hand and insert into meld
			{
				//insert into meld pile
				meldPile.push_back(playerHand[*it - removeCount]);

				//add the card to the list for this meld
				cardsOfAMeld.push_back(playerHand[*it - removeCount]);

				//insert into card to meld map
				cardToMeldMap.insert(pair<Card*, vector<unsigned int>>(playerHand[*it - removeCount], { meldIndex }));

				//remove from hand, decrease removeCount from index because after each erase, the indexes change
				playerHand.erase(playerHand.begin() + *it - removeCount);

				//increment counter
				removeCount++;
			}
		}

		//find a meld of the same index if it already exists
		map<unsigned int, vector<vector<Card*>>>::iterator foundMeld = meldToCardMap.find(meldIndex);

		//if found append the list to the matrix
		if (foundMeld != meldToCardMap.end())
		{
			(foundMeld->second).push_back(cardsOfAMeld);
		}
		//else, create a new entry in the map
		else
		{
			meldToCardMap.insert(pair<unsigned int, vector<vector<Card*>>>(meldIndex, { cardsOfAMeld }));
		}

		//increase player score
	}
	else
	{
		//show that the meld could not be made
		cout << "The meld could not be made. You chose the wrong cards." << endl;

	}
}