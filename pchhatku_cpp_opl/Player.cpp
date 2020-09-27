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

	if (showDefinitions) cout << endl << "Meld Pile" << endl << endl;
	//displaying meld cards
	for (vector<Card*>::iterator it = meldPile.begin(); it != meldPile.end(); ++it)
	{
		cout << i++ << ". " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";

	}
	cout << endl;

	//display card to meld map
	cout << "Card to meld map:" << endl;
	for (auto it = cardToMeldMap.begin(); it != cardToMeldMap.end(); ++it)
	{
		//display card
		std::cout << " " << it->first->getCardFace()<< it->first->getCardSuit()<< " : "<<"[";
		for (int i=0 ; i < (it->second).size(); i++)
		{
			cout << (it->second)[i] << ", ";
		}
		cout << "]" << endl;
	}
		
	//display meld to card map
	cout << "Meld to card map:" << endl;
	for (auto it = meldToCardMap.begin(); it != meldToCardMap.end(); ++it)
	{
		//display meld
		std::cout << " " << MELDS.at(((int)it->first)) << ":" << "[";
		cout << (it->second).size() << endl;
		for (int i = 0; i < (it->second).size(); i++)
		{
			cout << "[";
			for (int j = 0; j < (it->second)[i].size(); j++)
			{
				cout << (it->second)[i][j]->getCardFace() << (it->second)[i][j]->getCardSuit() << ", ";
			}
			cout << "]," << endl;
		}
		cout << "]" << endl;
	}

	//display capture pile
	cout << "Player capture pile" << endl;
	for (int i = 0; i < playerCapturePile.size(); i++)
	{
		cout << playerCapturePile[i]->getCardFace() << playerCapturePile[i]->getCardSuit() << ", ";
	}
	cout << endl;

	//display the meld string
	cout << getMeldString() << endl;

}

//process/move played cards for the user after the turn has been completed
void Player::processPlayedCards() {
	//process played cards
	//process the hand and meld cards,

	//if the played card is in hand pile: 1)it can either be only part of a hand, 2) it can be part of a hand and an earlier meld
	vector<Card*>::iterator ip = std::find(playerHand.begin(), playerHand.end(), playedCards[0]);
	if (ip != playerHand.end())
	{
		//check if it is in the card to meld map as well	
		unordered_map<Card*, vector<unsigned int>>::iterator im = cardToMeldMap.find(playedCards[0]);

		//if true remove from meld pile also
		if (im != cardToMeldMap.end())
		{
			cardToMeldMap.erase(im);
		}

		//remove card from hand pile
		playerHand.erase(ip);
	}
	else//else: the cards are in an active meld: in the meld pile and the cardToMeldMap
	{
		//find the card entry in cardToMeldMap
		unordered_map<Card*, vector<unsigned int>>::iterator im = cardToMeldMap.find(playedCards[0]);

		//for each of the card's meld
		for (int i = 0; i < (im->second).size(); i++)
		{
			//temporary variable which stores the current meld for the chosen card
			unsigned int tempMeld = (im->second)[i];

			//Among the vectors in the entry in meldToCardMap, find the one in which this card has been used
			for (int j = 0; j < meldToCardMap[tempMeld].size(); j++)
			{
				vector<Card*>::iterator iv = find(((meldToCardMap[tempMeld])[j]).begin(), ((meldToCardMap[tempMeld])[j]).end(), playedCards[0]);
				
				//iv is the iterator to the found vector: this needs to be erased from ((meldToCardMap[tempMeld])[j])
				if (iv != ((meldToCardMap[tempMeld])[j]).end())
				{
					//for all other cards in this vector: we need to decide whether to send them to the hand or not
					for (int k = 0; k < ((meldToCardMap[tempMeld])[j]).size(); k++)
					{
						//if same card, continue
						if(playedCards[0] == (meldToCardMap[tempMeld])[j][k])
						{
							continue;
						}
						//find it in the cardToMeldMap
						unordered_map<Card*, vector<unsigned int>>::iterator meldCheckIterator = cardToMeldMap.find((meldToCardMap[tempMeld])[j][k]);

						//flag to see if variable found
						bool isfound = false;

						//go to each of the mentioned meld to find the card
						for (int l = 0; l < (meldCheckIterator->second).size(); l++)
						{
							//temporary variable for checking if the meld exists in the map
							unsigned int tempCheckMeld = (meldCheckIterator->second)[l];

							//if the checkmeld is the same as parent meld, continue, because we are going to remove from this
							if (tempMeld == tempCheckMeld) continue;

							for (int m = 0; m < meldToCardMap[tempCheckMeld].size(); m++)
							{
								vector<Card*>::iterator findIterator = find(((meldToCardMap[tempCheckMeld])[m]).begin(), ((meldToCardMap[tempCheckMeld])[m]).end(), (meldToCardMap[tempMeld])[j][k]);
								//if found:
								if (findIterator != ((meldToCardMap[tempCheckMeld])[m]).end())
								{
									//only erase the original vector from the original meld entry in meldToCard map
									//no need to send the to player's hand
									isfound = true;
									break;
								}
							}

							if (isfound)
							{
								break;
							}
						}
						//if the card is not found in any of the listed meld, it means that the melds are no longer active
						//send the card to the user's hand
						if (!isfound)
						{
							addToHand((meldToCardMap[tempMeld])[j][k]);
							//find in meld pile
							vector<Card*>::iterator findInMeldIterator = find(meldPile.begin(), meldPile.end(), (meldToCardMap[tempMeld])[j][k]);
							//remove from meld pile
							if (findInMeldIterator != meldPile.end())
								meldPile.erase(findInMeldIterator);
							else
								cout << "Error!! not found in the pile" << endl;
						}
						//else do nothing
					}
					//erase iv from the vector ((meldToCardMap[tempMeld])[j])
					((meldToCardMap[tempMeld])[j]).erase(iv);
				}
			}
		}

		//erase from cardToMeld map
		cardToMeldMap.erase(im);

		//erase from meldpile
		vector<Card*>::iterator ix = find(meldPile.begin(), meldPile.end(), playedCards[0]);
		if (ix != meldPile.end())
		{
			meldPile.erase(ix);
		}
		else
			cout<<"error in deletin the chosen card"<<endl;
	}

}

//evaluates meld from the played cards
unsigned int Player::evaluateMeld(Card* trumpCard)
{
	//check if at least one card has been used from the hand, otherwise do not continue, show that meld cannot happen
	unsigned int cardsInMeldPile = 0;
	for (vector<Card*>::iterator it = playedCards.begin(); it != playedCards.end(); ++it)
	{
		//if all the cards can be found in the card to meld map, then no cards from hand has been used
		unordered_map<Card*, vector<unsigned int>>::iterator foundCard = cardToMeldMap.find(*it);
		if (foundCard != cardToMeldMap.end())
		{
			cardsInMeldPile++;
		}
	}
	//meld not possible
	if (cardsInMeldPile == playedCards.size())
	{
		return 0;
	}

	//sort the played cards so that it is easy for comparision based on the index of the cards
	//the comments describe how they are compared
	sort(playedCards.begin(), playedCards.end(), []( Card* &lhs,  Card* &rhs)
	{
		return (lhs)->getCardPoints() > (rhs)->getCardPoints();
	});

	/*cout << "sorted chosen cards" << endl;
	for (vector<Card*>::iterator it = playedCards.begin(); it != playedCards.end(); ++it)
	{
		cout << " " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";
	}*/

	//variable to store the index of the possible meld
	unsigned int possibleMeld = 0;

	//find which meld is possible from the available cards
	switch (playedCards.size())
	{
	case 1:
		//Nine of Trump suit - called dix - 10 points
		if ('9' == playedCards[0]->getCardFace() && trumpCard->getCardSuit() == playedCards[0]->getCardSuit())
		{
			//add scores and evaluate the meld
			//cout << "dix" << endl;
			possibleMeld = 4;
		}
		break;
	case 2:
		//King and Queen of Trump suit - called royal marriage - 40 points
		if ('K' == playedCards[0]->getCardFace() && 'Q' == playedCards[1]->getCardFace() && playedCards[0]->getCardSuit() == playedCards[1]->getCardSuit())
		{
			if (trumpCard->getCardSuit() == playedCards[0]->getCardSuit())
			{
				//cout << "ROYAL marriage" << endl;
				possibleMeld = 2;
			}
			//King and Queen of any other suit - called marriage - 20 points
			else
			{
				//cout << "marriage" << endl;
				possibleMeld = 3;
			}
		}

		//Queen of Spades and Jack of Diamonds - called Pinochle - 40 points
		if ('Q' == playedCards[0]->getCardFace() && 'S' == playedCards[0]->getCardSuit())
		{
			if ('J' == playedCards[1]->getCardFace() && 'D' == playedCards[1]->getCardSuit())
			{
				//cout << "PINOCHLE" << endl;
				possibleMeld = 9;
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
				//if not same face, return false
				if ((playedCards[i]->getCardFace() != playedCards[j]->getCardFace())) 
				{
					return false;
				}

				//if same suit, return false
				if (playedCards[i]->getCardSuit() == playedCards[j]->getCardSuit())
				{
					return false;
				}
			}
		}

		switch (playedCards[0]->getCardFace())
		{
		case 'A':
			//cout << "four Aces" << endl;
			possibleMeld = 5;
			break;
		case 'K':
			//cout << "four kings" << endl;
			possibleMeld = 6;
			break;
		case 'Q':
			//cout << "four queens" << endl;
			possibleMeld = 7;
			break;
		case 'J':
			//cout << "four jacks" << endl;
			possibleMeld = 8;
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
				return 0;
			}
		}

		//check if they are int the order: Ace, Ten, King, Queen and Jack
		if ('A' == playedCards[0]->getCardFace() && 'X' == playedCards[1]->getCardFace() && 'K' == playedCards[2]->getCardFace())
		{
			if ('Q'==playedCards[3]->getCardFace() && 'J' ==playedCards[4]->getCardFace())
			{
				//cout << "flush" << endl;
				possibleMeld = 1;

			}
		}
		break;
	default://if no cases match return false;
		possibleMeld = 0;
		return 0;
	}
	
	//if meld cards are used and player is trying to make a meld they already made
			//the meld cannot be allowed
	if (cardsInMeldPile > 0 && possibleMeld != 0)
	{
		for (vector<Card*>::iterator it = playedCards.begin(); it != playedCards.end(); ++it)
		{
			//find the meld cards
			unordered_map<Card*, vector<unsigned int>>::iterator foundCard = cardToMeldMap.find(*it);

			//if card is found in cardToMeldMap
			if (foundCard != cardToMeldMap.end())
			{
				//check if the possibleMeld is in the vector of Melds for a card
				vector<unsigned int>::iterator ix = find((foundCard->second).begin(), (foundCard->second).end(), possibleMeld);
				//if found
				if (ix != (foundCard->second).end())
				{
					return 0;
				}
			}
		}

		//if the condition passes, the meld card/s is/are used for a new valid meld
		return possibleMeld;

	}
	//else
		//proceed for scoring
	else
	{
		return possibleMeld;
	}
}

//give player permission for a meld
void Player::decideMeld(Card* trumpCard)
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
	if (0 == listOfPossibleMelds.size())
	{
		if ("H" == playerName)
			cout << "No Melds are possible from your list of cards. Choose any number of cards to cancel." << endl;
		else
			cout << "The computer decided to not make any melds" << endl;
		return;
	}

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

	//if Player is human
	if ("H" == playerName)
	{
		cout << "Human should choose cards the cards:";
		for (int i = 0; i < chosenMeld.size(); i++)
		{
			cout <<" "<<chosenMeld[i]->getCardFace()<< chosenMeld[i]->getCardSuit();
		}

		cout<<"at index: ";
		for (int i = 0; i < listOfChosenIndex.size(); i++)
		{
			cout << listOfChosenIndex[i] << ", ";
		}
		cout << " repectively to make a " << MELDS.at(meldIndex) << ", which will bring the Human "<< MELD_POINTS.at(meldIndex) <<" points."<< endl;
		return;
	}

	else
	{
		cout << "Computer chose the cards:";
		for (int i = 0; i < chosenMeld.size(); i++)
		{
			cout << " " << chosenMeld[i]->getCardFace() << chosenMeld[i]->getCardSuit();
		}
		cout << " repectively to make a " << MELDS.at(meldIndex) << ", which will brought " << MELD_POINTS.at(meldIndex) << " points." << endl;
	}

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
Card* Player::getCheapestCard(Card* leadCard, Card* trumpCard)
{
	//find all playable cards which will win the turn
	vector<Card*> winningCards = findPlayableCards(leadCard, trumpCard); //winning cards will have all cards, if no winning cards present

	//sort them based on points on ascending order
	sort(winningCards.begin(), winningCards.end(), [](Card* &lhs, Card* &rhs)
	{
		return (lhs)->getCardPoints() < (rhs)->getCardPoints();
	});

	//print for debug
	cout << endl << "Winning cards" << endl;
	for (int i = 0; i < winningCards.size(); i++)
	{
		cout << winningCards[i]->getCardFace() << winningCards[i]->getCardSuit()<<" ";
	}

	//find the first card on the vector that is not of trump suit
	for (int i = 0; i < winningCards.size(); i++)
	{
		//if found return card
		if (winningCards[i]->getCardSuit() != trumpCard->getCardSuit())
		{
			Card* tempCard = winningCards[0];
			winningCards[0] = winningCards[i];
			winningCards[i] = tempCard;
			break;
		}
	}

	if (winningCards.size() == (playerHand.size() + meldPile.size()))
	{
		if (playerName == "H")
			cout << "You should choose " << winningCards[0]->getCardFace() << winningCards[0]->getCardSuit() << " because you have no cards that will win the move." << endl;
		else
			cout << "Computer chose " << winningCards[0]->getCardFace() << winningCards[0]->getCardSuit() << " because it has no cards that will win the move." << endl;
	}
	else
	{
		if (playerName == "H")
			cout << "Human should chose " << winningCards[0]->getCardFace() << winningCards[0]->getCardSuit() << " because human could win the move." << endl;
		else
			cout << "Computer chose " << winningCards[0]->getCardFace() << winningCards[0]->getCardSuit() << " because it could win the move." << endl;
	}

	//if no such card is found
			//return the cheapest card: [0] first card of the sorted list
	return winningCards[0];
}

//get tactical card
Card* Player::getTacticalCard(Card* trumpCard)
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
		//cout << "NUMBER:::: " << i << endl;
		//temporary variable to store the list of cards when an element at index i is removed
		vector<Card*> tempListOfCards = listOfPlayableCards;
		//cout << "The card remove below is " << listOfPlayableCards[i]->getCardFace()<< listOfPlayableCards[i]->getCardSuit() << endl;
		tempListOfCards.erase(tempListOfCards.begin() + i);


		//cout << endl << "each card removed" << endl;
		////print vector for debugging
		//for (vector<Card*>::iterator it = tempListOfCards.begin(); it != tempListOfCards.end(); ++it)
		//{
		//	cout << ". " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";

		//}
		//cout << endl;
		unsigned int possibleScore = findPossibleScores(tempListOfCards, trumpCard);
		//cout << possibleScore << endl;
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

	for (int i = 0; i < cardsWithMaxPoints.size(); i++)
	{
		if (cardsWithMaxPoints[i]->getCardPoints() != maxPoints)
			break;

		if (cardsWithMaxPoints[i]->getCardPoints() == maxPoints && cardsWithMaxPoints[i]->getCardSuit() == trumpCard->getCardSuit())
		{
			return cardsWithMaxPoints[i];
		}
	}

	//remove the card from this index and insert into played cards
	cout << "The played card is from 2: " << cardsWithMaxPoints[0]->getCardFace() << cardsWithMaxPoints[0]->getCardSuit() << endl;
	return cardsWithMaxPoints[0];

}

//find the list of playable cards in the player's meld and capture pile
vector<Card*> Player::findPlayableCards(Card* leadCard, Card* trumpCard)
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
bool Player::compareTwoCards(Card* leadCard, Card* chaseCard, Card* trumpCard)
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
unsigned int Player::findPossibleScores(vector<Card*> listOfCards, Card* trumpCard)
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
void Player::findCombinations(vector<Card*> listOfCards, unsigned int sizeOfListOfCards, unsigned int sizeOfCombinations, unsigned int &scoreForThisList, Card* trumpCard)
{
	vector<Card*> dataVector(sizeOfCombinations);

	//used for calling melds
	//listOfPossibleMelds.clear();// variable that stores pair of a vector and its points

	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, 0, dataVector, 0, scoreForThisList, trumpCard);

}

//utility function deifinition
void Player::utilityForMeldCombinations(vector<Card*> listOfCards, unsigned int sizeOfListOfCards, unsigned int sizeOfCombinations, unsigned int indexForDataVector, vector<Card*> dataVector, unsigned int indexForMainList, unsigned int& scoreForThisList, Card* trumpCard)
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
			//cout << endl;
			/*for (vector<Card*>::iterator it = dataVector.begin(); it != dataVector.end(); ++it)
			{
				cout << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";

			}*/
			//cout << " points: "<< MELD_POINTS.at(possibleMeld)<<MELDS.at(possibleMeld) << endl;

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


//get a string representation of the player's string
string Player::getMeldString()
{
	//declaring and initializing string variable to combine the melds
	string meldString = "";

	//for each meld in the meld to card map
	for (map<unsigned int, vector<vector<Card*>>>::iterator im = meldToCardMap.begin(); im != meldToCardMap.end(); im++)
	{
		// get the vector of vector of cards
		vector<vector<Card*>> tempVectorOfAMeld = im->second;

		//if this vector has no size, continue
		if (0 == tempVectorOfAMeld.size()) continue;

		//save the meld index in a variable
		unsigned int currentMeld = im->first;

		//for each vector within the tempVectorOfAMeld list
		for (int i = 0; i < tempVectorOfAMeld.size(); i++)
		{
			vector<Card*> vectorWithinVector = tempVectorOfAMeld[i];
			//for each card in this vector
			for (int j = 0; j < vectorWithinVector.size(); j++)
			{
				//append the card to the output string
				//vectorWithinVector[j] is the card
				meldString += " ";
				meldString += vectorWithinVector[j]->getCardFace();
				meldString += vectorWithinVector[j]->getCardSuit();
				

				//check if an asterisk is required
				//flag to see if the card was used in another active meld
				bool isFound = false;

				//go to the card index in card to meld map
				vector<unsigned int> meldsForACard = cardToMeldMap[vectorWithinVector[j]];

				//for each meld mentioned for the card
				for (int k = 0; k < meldsForACard.size(); k++)
				{
					//go to the meld to see if another meld with the card is still active
					if (currentMeld == meldsForACard[k]) continue;

					else
					{
						//collection of vectors for this meld
						vector<vector<Card*>> tempCheckMeldCollection = meldToCardMap[meldsForACard[k]];

						//for the vector of melds hence received
						for (int l = 0; l < tempCheckMeldCollection.size(); l++)
						{
							//iterate the melds to see if the card , vectorWithinVector[j] is in the list
							vector<Card*>::iterator findIterator = find(tempCheckMeldCollection[l].begin(), tempCheckMeldCollection[l].end(), vectorWithinVector[j]);
			
							//if the card is found an asterisk is required, break off the program, go to the next card
							if (findIterator != tempCheckMeldCollection[l].end())
							{
								meldString += '*';
								isFound = true;
								break;
							}
						}
						if (isFound)
							break;
					}
				}
			}

			//insert a comma to separate the melds
			meldString += ',';
		}

	}
	return meldString;
}