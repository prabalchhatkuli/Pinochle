#include "Computer.h"

//class constructor
Computer::Computer()
{
	cout << "Computer player object created"<<endl;
}

//play function for human
//make a move for the player
void Computer::play(vector<Card*> leadCard, Card* trumpCard)
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
		getTacticalCard(trumpCard);
	}

	return;
}

//player makes a move
void Computer::makeMove()
{
	//variable declaration
	unsigned int cardNumber;

	displayPlayerCards(false);
	cout << "your score till now: " << playerRoundScore << "/" << playerScore << endl;
	cout << "Make a move(option):" << endl << endl;
	//input card for the move
	cout << "select a card to run: 0 - " << (playerHand.
		size() + meldPile.size() - 1) << endl;

	cin >> cardNumber;

	//validate cardNumber
	//if chosen card has index less than the size of playerhand, the card is in the playerHand, otherwise in the meld pile
	//the card needs to be inserted into the playedCards and removed from the hand or the meld
	if (cardNumber < playerHand.size())
	{
		playedCards.push_back(playerHand[cardNumber]);
		playerHand.erase(playerHand.begin() + cardNumber);
	}
	else
	{
		playedCards.push_back(meldPile[cardNumber - playerHand.size()]);
		meldPile.erase(meldPile.begin() + (cardNumber - playerHand.size()));
	}

}

//give player permission for a meld
void Computer::callMeld(Card* trumpCard)
{
	//vector to store the list of cards: hand and meld pile
	vector<Card*> listOfPlayableCards;

	//create a vector with all playable cards: combine hand and meld pile
	listOfPlayableCards.insert(listOfPlayableCards.end(), playerHand.begin(), playerHand.end());
	listOfPlayableCards.insert(listOfPlayableCards.end(), meldPile.begin(), meldPile.end());

	listOfPossibleMelds.clear();// variable that stores pair of a vector and its points

	//find all possible melds from the remaining list of cards
	unsigned int possibleScore = findPossibleScores(listOfPlayableCards, trumpCard);


	//if no possible melds, return
	if(0 == listOfPossibleMelds.size())
		return;

	//from the possible melds find the one with the highest number of meld points
	//sort the vector
	sort(listOfPossibleMelds.begin(), listOfPossibleMelds.end(), [](pair<vector<Card*>, unsigned int> &lhs, pair<vector<Card*>, unsigned int> &rhs)
	{
		return (lhs.second > rhs.second);
	});

	//get the first element
	vector<Card*> chosenMeld = listOfPossibleMelds[0].first;

	//place all the cards needed for the meld into playedCards.
	playedCards.clear();
	playedCards.insert(playedCards.end(), chosenMeld.begin(), chosenMeld.end());

	//vector to store the index of the cards in the listOfPlayableCards
	vector<unsigned int> listOfChosenIndex;

	//get the list of indexes the cards belong to
	for (int i = 0; i < playedCards.size(); i++)
	{
		vector<Card*>::iterator findACard = find(listOfPlayableCards.begin(), listOfPlayableCards.end(), playedCards[i]);
		if (findACard != listOfPlayableCards.end()) {
			// calculating the index 
			unsigned int index = distance(listOfPlayableCards.begin(), findACard);
			listOfChosenIndex.push_back(index);
		}
	}

	//evaluate the meld
	unsigned int meldIndex = evaluateMeld(trumpCard);

	//empty the played cards
	playedCards.clear();

	//add the score to the current round score
	playerRoundScore += MELD_POINTS.at(meldIndex); //.at() is used because MELD_POINT is a constant

	//process the move of card into the two meld maps
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
					//append to the index of the card in the cardToMeldMap
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

	cout << "Computer chose the meld: " << MELDS.at(meldIndex) << endl;
}

//get cheapest card 
Card* Computer::getCheapestCard(Card* leadCard, Card* trumpCard)
{
	//find all playable cards which will win the turn
	vector<Card*> winningCards = findPlayableCards(leadCard, trumpCard);
 
	//sort them based on points
	sort(winningCards.begin(), winningCards.end(), [](Card* &lhs, Card* &rhs)
	{
		return (lhs)->getCardPoints() > (rhs)->getCardPoints();
	});

	//find the first card on the vector that is not of trump suit
	for (int i = 0; i < winningCards.size(); i++)
	{
		//if found return card
		if (winningCards[i]->getCardSuit() != trumpCard->getCardSuit())
		{
			return winningCards[i];
		}
	}
	//if no such card is found
			//return the cheapest card: [0] first card of the sorted list
	return winningCards[0];
}

//get tactical card
void Computer::getTacticalCard(Card* trumpCard)
{
	//create a vector to store the overall meldscore if we remove a card from the deck
	vector<unsigned int> meldScoreEachCard;
	vector<Card*> listOfPlayableCards;

	//create a vector with all playable cards: combine hand and meld pile
	listOfPlayableCards.insert(listOfPlayableCards.end(), playerHand.begin(), playerHand.end());
	listOfPlayableCards.insert(listOfPlayableCards.end(), meldPile.begin(), meldPile.end());

	//iterate the vector
		//for each element(Card*): create another vector by removing that element from the overall vector
	for (int i = 0; i < listOfPlayableCards.size(); i++)
	{
		cout << "NUMBER:::: " << i << endl;
		//temporary variable to store the list of cards when an element at index i is removed
		vector<Card*> tempListOfCards = listOfPlayableCards;
		cout << "The card remove below is " << listOfPlayableCards[i]->getCardFace()<< listOfPlayableCards[i]->getCardSuit() << endl;
		tempListOfCards.erase(tempListOfCards.begin() + i);


		cout << endl << "each card removed" << endl;
		//print vector for debugging
		for (vector<Card*>::iterator it = tempListOfCards.begin(); it != tempListOfCards.end(); ++it)
		{
			cout << ". " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";

		}
		cout << endl;
		unsigned int possibleScore = findPossibleScores(tempListOfCards, trumpCard);
		cout << possibleScore << endl;
		//pushback the score of removing this card
		meldScoreEachCard.push_back(possibleScore);
	}

	//for the list of cards in listOfPlayableCards
	vector<int> indexesWithMaximumPoints;
	unsigned int maxPoints = 0;
	//find the indexes with the highest points for the melds in overalllMeldScoreEachCard
	maxPoints = *max_element(meldScoreEachCard.begin(), meldScoreEachCard.end());

	for (int i = 0; i < meldScoreEachCard.size(); i++)
	{
		//this means that the if this card is removed, we will have the highest possibility of scoring in meld, if we win
		if (maxPoints == meldScoreEachCard[i])
		{
			indexesWithMaximumPoints.push_back(i);
		}
	}
	//temporary vector to store cards with maximum points
	vector<Card*> cardsWithMaxPoints;//Meld points
	//find the card/s at the indexes
	for (int i = 0; i < indexesWithMaximumPoints.size(); i++)
	{
		//get the card from listOfPlayableCards
		cardsWithMaxPoints.push_back(listOfPlayableCards[indexesWithMaximumPoints[i]]);
	}

	//find the card with the highest value
	//sort them based on points
	sort(cardsWithMaxPoints.begin(), cardsWithMaxPoints.end(), [](Card* &lhs, Card* &rhs)
	{
		//in descending order of points
		return (lhs)->getCardPoints() > (rhs)->getCardPoints();
	});

	//get the cards with the highest point
		//from the cards with high meld points choose the first card/ if there is a trump card suit with equal points
	maxPoints = cardsWithMaxPoints[0]->getCardPoints();
	cout << "reached here 1" << endl;
	for (int i = 0; i < cardsWithMaxPoints.size(); i++)
	{
		if (cardsWithMaxPoints[i]->getCardPoints() != maxPoints)
			break;
		
		if (cardsWithMaxPoints[i]->getCardPoints() == maxPoints && cardsWithMaxPoints[i]->getCardSuit() == trumpCard->getCardSuit())
		{
			cout << "reached here 2" << endl;
			playedCards.push_back(cardsWithMaxPoints[i]);
			cout << "The possible played card is " << cardsWithMaxPoints[i]->getCardFace() << cardsWithMaxPoints[i]->getCardSuit() << endl;
			return;
		}
	}

	//remove the card from this index and insert into played cards
	cout << "The played card is from 2: " << cardsWithMaxPoints[0]->getCardFace()<< cardsWithMaxPoints[0]->getCardSuit() <<endl;
	playedCards.push_back(cardsWithMaxPoints[0]);

}

//find the list of playable cards in the player's meld and capture pile
vector<Card*> Computer::findPlayableCards(Card* leadCard, Card* trumpCard)
{
	//vector to store cards that should be used on the move
	vector<Card*> listOfPlayableCards;

	//find all cards from the hand pile which will win the turn
	for (int i = 0; i < playerHand.size(); i++)
	{
		if (compareTwoCards(leadCard, playerHand[i], trumpCard))
		{
			listOfPlayableCards.push_back(playerHand[i]);
		}
	}
	//same for meldPile
	for (int i = 0; i < meldPile.size(); i++)
	{
		if (compareTwoCards(leadCard, meldPile[i], trumpCard))
		{
			listOfPlayableCards.push_back(meldPile[i]);
		}
	}

	//if no card is found that can win the turn
	//get all cards
	if (0 == listOfPlayableCards.size())
	{
		listOfPlayableCards.insert(listOfPlayableCards.end(), playerHand.begin(), playerHand.end());
		listOfPlayableCards.insert(listOfPlayableCards.end(), meldPile.begin(), meldPile.end());
	}

	return listOfPlayableCards;
}

//compare two cards
bool Computer::compareTwoCards(Card* leadCard, Card* chaseCard, Card* trumpCard)
{
	//if both cards are same, lead player wins
	if (leadCard->getCardSuit() == chaseCard->getCardSuit() && leadCard->getCardFace() == chaseCard->getCardFace())
	{
		return 0;
	}

	//if lead card is of trump suit
	else if (trumpCard->getCardSuit() == leadCard->getCardSuit())
	{
		//if chase is also of trump suit and has more point than lead then the chase player wins the turn
		if (trumpCard->getCardSuit() == chaseCard->getCardSuit() && chaseCard->getCardPoints() > leadCard->getCardPoints())
		{
			return 1;
		}
		//else the lead player wins
		else
		{
			return 0;
		}
	}
	//if  lead card is not of trump suit,
	else if (trumpCard->getCardSuit() != leadCard->getCardSuit())
	{
		//if chase is of trump suit, chase wins
		if (trumpCard->getCardSuit() == chaseCard->getCardSuit())
		{
			return 1;
		}
		//if lead and chase are same suit and chase has higher card face, chase wins
		else if (leadCard->getCardSuit() == chaseCard->getCardSuit() && chaseCard->getCardPoints() > leadCard->getCardPoints())
		{
			return 1;
		}
		//otherwise lead wins
		else
		{
			return 0;
		}
	}
}

//find possible meld score for a list of cards
unsigned int Computer::findPossibleScores(vector<Card*> listOfCards, Card* trumpCard)
{
	//we only need to create combinations of 1 to 5 cards because,
	//if the size of the list>=5::melds have max:5 cards, and min:1 card
	//else:: we need possible scores for max::listOfCards.size() cards and min 1
	unsigned int scoreForThisList = 0;
	unsigned int size = listOfCards.size();

	//for 1-5 cards
	for (int i = 1; i < 6; i++)
	{
		findCombinations(listOfCards, size, i, scoreForThisList, trumpCard);
	}
	
	return scoreForThisList;
}

//recursive function for the tree structure created for combination
void Computer::findCombinations(vector<Card*> listOfCards, unsigned int sizeOfListOfCards, unsigned int sizeOfCombinations, unsigned int &scoreForThisList, Card* trumpCard)
{
	vector<Card*> dataVector(sizeOfCombinations);
	
	//used for calling melds
	//listOfPossibleMelds.clear();// variable that stores pair of a vector and its points

	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, 0, dataVector, 0, scoreForThisList, trumpCard);

}

//utility function deifinition
void Computer::utilityForMeldCombinations(vector<Card*> listOfCards, unsigned int sizeOfListOfCards, unsigned int sizeOfCombinations, unsigned int indexForDataVector, vector<Card*> dataVector, unsigned int indexForMainList, unsigned int& scoreForThisList, Card* trumpCard)
{
	// if combination has reached the require size 
	if (indexForDataVector == sizeOfCombinations)
	{
		//at this point dataVector has a combination of "sizeOfCombinations" elements
		// set the "playedCards" variable of Player class to the dataVector: this is done because the evaluateMeld() function uses data from playedCards
		playedCards.clear();
		playedCards.insert(playedCards.end(), dataVector.begin(), dataVector.end());

		//evaluate possible meld
		unsigned int possibleMeld = evaluateMeld(trumpCard);

		//if any meld possible: add to the score for this list
		if (0 != possibleMeld)
		{
			cout << endl;
			for (vector<Card*>::iterator it = dataVector.begin(); it != dataVector.end(); ++it)
			{
				cout << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";

			}
			cout << " points: "<< MELD_POINTS.at(possibleMeld)<<MELDS.at(possibleMeld) << endl;

			//the section of the code below is for storing the meld so that it can be used while the user want to call meld
			listOfPossibleMelds.push_back(make_pair(dataVector, MELD_POINTS.at(possibleMeld)));
			
			scoreForThisList += MELD_POINTS.at(possibleMeld);
		}

		//clear "playedCards" variable
		playedCards.clear();

		//scoreForThisList++;

		return;
	}

	//  if no more 
	if (indexForMainList >= sizeOfListOfCards)
		return;

	// current is included, put next at next location 
	dataVector[indexForDataVector] = listOfCards[indexForMainList];
	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, indexForDataVector + 1, dataVector, indexForMainList + 1, scoreForThisList, trumpCard);

	// current is excluded, replace it with next (Note that 
	// i+1 is passed, but index is not changed) 
	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, indexForDataVector, dataVector, indexForMainList + 1, scoreForThisList, trumpCard);
}
