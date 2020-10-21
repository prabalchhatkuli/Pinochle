#include "Player.h"

/* *********************************************************************
Function Name: Player
Purpose: to initalize member variables
Parameters:
			none

Assistance Received: none
********************************************************************* */
//class constructor
Player::Player()
{
	//member variable initializations
	playerScore = 0;
	playerRoundScore = 0;
	playerCapturePile = {};
	meldPile = {};
	playedCards = {};
	meldToCardMap = {};
	cardToMeldMap = {};
	listOfPossibleMelds = {};
}

/* *********************************************************************
Function Name: displayPlayerCards
Purpose: to display all of the player's cards
Parameters:
			showDefinitions, a boolean. It holds a flag to whether display the meld cards and hand cards differently
Return Value: none
Local Variables:
			none
Algorithm:
			1) Display the game score for the user
			2) display hand ,meldPile, meld connections, and the string of the meld

Assistance Received: none
********************************************************************* */
void Player::displayPlayerCards(bool showDefinitions)
{
	cout << "Score:: ";
	cout << getGameScore() << "/" << getRoundScore() << endl;;

	cout << "Current cards::   | ";
	//variable to act as a counter for the cards, both hand and meld ones
	int i = 0;
	if(showDefinitions) cout << "Hand: ";
	//displaying hand cards
	for (vector<Card*>::iterator it = playerHand.begin(); it != playerHand.end(); ++it)
	{
		cout <<i++<<". "<<(*it)->getCardFace() << (*it)->getCardSuit() << " | ";
	}

	if (showDefinitions) cout << endl << "Single cards from Meld Pile: |";
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
		for (int i = 0; i < (it->second).size(); i++)
		{
			cout << "[";
			for (int j = 0; j < (it->second)[i].size(); j++)
			{
				cout << (it->second)[i][j]->getCardFace() << (it->second)[i][j]->getCardSuit() << ", ";
			}
			cout << "],";
		}
		cout << "]" << endl;
	}

	//display capture pile
	cout << "Player capture pile: ";
	for (int i = 0; i < playerCapturePile.size(); i++)
	{
		cout << playerCapturePile[i]->getCardFace() << playerCapturePile[i]->getCardSuit() << ", ";
	}
	cout << endl;

	//display the meld string
	cout << "Meld String: "<<getMeldString() << endl;

}

/* *********************************************************************
Function Name: processPlayedCards
Purpose: to process/move played cards for the user after the turn has been completed
Parameters:
			none
Return Value: none
Local Variables:
			ip, iterator of a vector od Card pointer. it holds the iterator of position of played card in the player hand.
			im, an iterator for map. it holds the iterator position of the played card in the cardToMeldMap.
			tempMeld, an integer. It holds the value of the meld currently being evaluated.
			isFound, a boolean. It flags whether a certain cards in a meld is in any other active meld.
			iv, an iterator for a vector of cards. It holds the position of the playedCard in a vector inside a meldToCardMap entry.
Algorithm:
			1) find the playedCard in the playerHand
			2) if the card is found
				3) find the card in cardToMeldMap
				4) if the card is found
					5) erase the entry from the map
				6) Then, erase it from the hand
			7)else, the card is in the meldPile
				8) find the card in cardToMeldMap
				9) for each of the mentioned meld(tempMeld) for this card
					10) iterate the respective entry in meldToCardMap
						11) for each meld in the entry, find the played card in the vector
						12) if it is found
							13) for all other cards
								14) get the cardToMeldMap entry
								15) Then go to each of the mentioned meld
								16) for each of the meld that is not tempMeld
									17) find the card in the entry for meldToCardMap
									18) if it is found then do not take the card to the handPile
									19) else, remove it from the meldPile and the HandPile 
						20) finally, remove this entry from the meldToCarMap

Assistance Received: none
********************************************************************* */
void Player::processPlayedCards() {
	//process played cards, hand, and meld cards,
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
				
				//iv is the iterator to the found vector
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
								cerr << "Error!! not found in the pile" << endl;
						}
						//else do nothing
					}
					//erase the vector containing iv from meldtoCardMap ((meldToCardMap[tempMeld])[j])
					((meldToCardMap[tempMeld])).erase((meldToCardMap[tempMeld]).begin() + j);
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
			cerr<<"error in deleting the chosen card"<<endl;
	}

}

/* *********************************************************************
Function Name: evaluateMeld
Purpose: to evaluate the meld from the played cards
Parameters:
			trumpCard, a Card object pointer. it holds the pointer to the trump card.
Return Value: the integer denoting the type of meld
Local Variables:
			possibleMeld, an integer. It holds the integer representation of the meld formed. Initialized to 0.
			cardsInMeldPile, an integer. It hold the number of cards chosen for the meld that were at sometime in the meldPile.
			foundCard, an iterator for a map. it hold the entry of the card found in the cardToMeldMap.

Algorithm:
			1) first count the number of cards that are in the cardToMeldMap to see if they were ever used in a meld
			2) if the count is equal to the total number of played cards, then no new cards were used to form the meld, the meld is not possible
			3) the cards are then sorted indescending order based on their points
			4) based on the number of cards chosen and the faces and suits of cards the possibleMeld is determined
			
			5) if the cound of cardsInMeldPile is not zero then all cards are found in cardToMeldMap
				6) for each found card, find the possibleMeld in the list of melds, if it is found no melds can be made.
			6)return the possibleMeld

Assistance Received: none
********************************************************************* */
unsigned int Player::evaluateMeld(Card* trumpCard)
{
	//variable to store the index of the possible meld
	unsigned int possibleMeld = 0;

	//variable to store the number of cards used in previous melds
	unsigned int cardsInMeldPile = 0;

	//check if at least one card has been used from the hand, otherwise do not continue, show that meld cannot happen
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

/* *********************************************************************
Function Name: decideMeld
Purpose: to automatically generate melds for a user
Parameters:
			trumpCard, a Card object pointer. This holds the trump card.

Return Value:
			none
Local Variables:
			listOfPlayableCards, a vector of card pointers. This holds the playable cards for the player
			possibleScore, an integer. The total scores generated by all possible melds.
			listOfChosenIndex, a vector of integers. This holds the indexes of the cards to be chosen.
			meldIndex, an integer. This holds the evalauted meld.
			cardsOfAMeld, a vector of card pointers. This holds the card pointer of the final meld.

Algorithm:
			1) all playable cards are found by combining hand and meld pile
			2) all possible melds are taken by calling the findPossibleScores function, which stores the melds and point in a map called lisOfPossibleMelds
			4) The map is sorted in descending order based on the points of the melds.
			5) the first entry is taken and this is returned for the human player, while the meld is executed for computer
			6) the cards are sorted based on index and evalauted
			7)for each card the cardToMeld map is updated, and it is added to cardsOfAMeld
			8) the meldToCard map is updated with the cardsOfAMeld
Assistance Received: none
********************************************************************* */
void Player::decideMeld(Card* trumpCard)
{
	//vector to store the list of cards: hand and meld pile
	vector<Card*> listOfPlayableCards;

	//variable to store the possible store
	unsigned int possibleScore;

	//create a vector with all playable cards: combine hand and meld pile
	listOfPlayableCards.insert(listOfPlayableCards.end(), playerHand.begin(), playerHand.end());
	listOfPlayableCards.insert(listOfPlayableCards.end(), meldPile.begin(), meldPile.end());

	//member variable that stores pair of a vector of cards and its points
	listOfPossibleMelds.clear();

	//find all possible melds from the remaining list of cards
	//the listOfPossibleMelds gets updated here
	possibleScore = findPossibleScores(listOfPlayableCards, trumpCard);

	//if no possible melds, return
	if (0 == listOfPossibleMelds.size())
	{
		cout << "No Melds are possible from the list of cards for the "<< getPlayerName() << "player." << endl;
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
	if ("Human" == playerName)
	{
		cout << getPlayerName() <<"should choose cards the cards:";
		for (int i = 0; i < chosenMeld.size(); i++)
		{
			cout <<" "<<chosenMeld[i]->getCardFace()<< chosenMeld[i]->getCardSuit();
		}

		cout<<" at index: ";
		for (int i = 0; i < listOfChosenIndex.size(); i++)
		{
			cout << listOfChosenIndex[i] << ", ";
		}
		cout << " repectively to make a " << MELDS.at(meldIndex) << ", which will bring the "<<getPlayerName()<<" "<< MELD_POINTS.at(meldIndex) <<" points."<< endl;
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
	playerRoundScore += MELD_POINTS.at(meldIndex);

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
				playerHand.erase(playerHand.begin() + (*it - removeCount));

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

/* *********************************************************************
Function Name: getTacticalCard
Purpose: to find the cheapest card which will win the move
Parameters:
			trumpCard, a Card object pointer. This holds the trump card.
			leadCard, a Card object pointer. This holds the card chosen by the lead player.
Return Value:
			a card Pointer to a Card object that should be played
Local Variables:
			winningCards, a vector of card object pointers. This will hold the winning or the playable cards.
Algorithm:
			1) All playable cards is found
			2) the cards are sorted on ascending order based on points
			3) The list is then iterated and the first card that is not of trumpSuit is taken, if it exists
			4) otherwise the first card in the list is taken
Assistance Received: none
********************************************************************* */
Card* Player::getCheapestCard(Card* leadCard, Card* trumpCard)
{
	//variable to store the winning cards
	vector<Card*> winningCards;

	//find all playable cards which will win the turn
	winningCards = findPlayableCards(leadCard, trumpCard);

	//sort them based on points on ascending order
	sort(winningCards.begin(), winningCards.end(), [](Card* &lhs, Card* &rhs)
	{
		return (lhs)->getCardPoints() < (rhs)->getCardPoints();
	});

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
		cout << getPlayerName() <<"should choose " << winningCards[0]->getCardFace() << winningCards[0]->getCardSuit() << " because it has no cards that will win the move." << endl;
	}
	else
	{
		
		cout << getPlayerName() << " should choose " << winningCards[0]->getCardFace() << winningCards[0]->getCardSuit() << " because it could win the move." << endl;
	}

	//if no such card is found
			//return the cheapest card: [0] first card of the sorted list
	return winningCards[0];
}

/* *********************************************************************
Function Name: getTacticalCard
Purpose: to find the card to choose for a player when the player is a lead player
Parameters:
			trumpCard, a Card object pointer. This holds the trump card.
Return Value:
			a card Pointer to a Card object that should be played
Local Variables:
			meldScoreEachCard,  a vector of integers. It holds the total meld scores for all melds if a card is removed: for all cards.
			listOfPlayableCards, a vector of Card object pointers. It holds all of the playable cards for the player.
			cardsWithMaxPoints, a vector of Card object pointers. It holds the cards from meldScoreEachCard that have maximum points 
			indexesWithMaximumPoints, a vector of integers. It holds the indexes for the card object in cardsWithMaxPoints. The indexes are for the hand and meld pile.
Algorithm:
			1) All cards from hand and meld pile are collected
			2) For each card
				3) remove the card from the list and get the possible melds scores from the remaining cards
				4) insert in meldScoreEachCard
			5) collect indexes of cards with maximum scores in meldScoreEachCard
			6) get card pointers from indexes
			7)sort the cards in descending order of points of cards
			8)get the first card which is of trump card if it exists
			9) otherwise, return the first card in the sorted list
Assistance Received: none
********************************************************************* */
Card* Player::getTacticalCard(Card* trumpCard)
{
	//vector to store the overall meldscore if we remove a card from the deck
	vector<unsigned int> meldScoreEachCard;

	//vector to store all playable cards
	vector<Card*> listOfPlayableCards;

	//temporary vector to store cards with maximum points
	vector<Card*> cardsWithMaxPoints;

	//variable for the list of cards in listOfPlayableCards
	vector<int> indexesWithMaximumPoints;

	//create a vector with all playable cards: combine hand and meld pile
	listOfPlayableCards.insert(listOfPlayableCards.end(), playerHand.begin(), playerHand.end());
	listOfPlayableCards.insert(listOfPlayableCards.end(), meldPile.begin(), meldPile.end());

	//iterate the vector
		//for each element(Card*): create another vector by removing that element from the overall vector
	for (int i = 0; i < listOfPlayableCards.size(); i++)
	{
		//temporary variable to store the list of cards when an element at index i is removed
		vector<Card*> tempListOfCards = listOfPlayableCards;
		tempListOfCards.erase(tempListOfCards.begin() + i);

		unsigned int possibleScore = findPossibleScores(tempListOfCards, trumpCard);

		//pushback the score of removing this card
		meldScoreEachCard.push_back(possibleScore);
	}

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
		if (cardsWithMaxPoints[i]->getCardSuit() == trumpCard->getCardSuit())
		{
			return cardsWithMaxPoints[i];
		}
	}
	
	return cardsWithMaxPoints[0];

}


/* *********************************************************************
Function Name: findPlayableCards
Purpose: to find the playable cards
Parameters:
			leadCard, a Card object. This holds the card value that is being challenged
			trumpCard, a Card object. This holds the trump card.
Return Value:
			vector of playable Card object pointers.
Local Variables:
			listOfPlayableCards, a vector of card object pointers
Algorithm:
			1) for each card in hand find the winning cards
			2) Repeat for meld pile
			3) if no winning cards are found, return all the cards by combining hand and meld cards
Assistance Received: none
********************************************************************* */
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
/* *********************************************************************
Function Name: compareTwoCards
Purpose: compare who wins between two cards
Parameters:
			leadCard, a Card object. This holds the card value that is being challenged
			chaseCard, a Card object. This holds the challenging card.
			trumpCard, a Card object. This holds the trump card.
Return Value:
			boolean, depending on whether the chaseCards wins or not.
Local Variables:
		
Algorithm:
			1) if the leadCard and chaseCard have the same suit, and face, the chase card loses
			2) if the lead card is of trump suit
				3) if the chase card is also of trump suit and has more points than lead, chase wins
				4) else it loses
			5)if the lead card is not of trump suit
				6) if the chase card is of trump suit, it wins
				7) if chase card is also of same suit and has more points than lead, chase wins
				8) chase card loses
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: findPossibleScores
Purpose: find possible meld score for a list of cards
Parameters:
			listOfCards, a vector of Card pointers. This holds the list of cards whose total meld score is to be determined.
			trumpCard, a Card object. This holds the trump card.
Return Value:
			total points from all melds
Local Variables:
			scoreForThisList, an integer. It holds the total meld score for the given list.
			size, an integer. It holds the size of the given list
Algorithm:
			1) calls findCombinations function
Assistance Received: none
********************************************************************* */
unsigned int Player::findPossibleScores(vector<Card*> listOfCards, Card* trumpCard)
{
	
	//else:: we need possible scores for max::listOfCards.size() cards and min 1
	unsigned int scoreForThisList = 0;
	unsigned int size = listOfCards.size();

	//for 1-5 cards
	//we only need to create combinations of 1 to 5 cards because,
	//melds can only be of maximum size 5
	for (int i = 1; i < 6; i++)
	{
		findCombinations(listOfCards, size, i, scoreForThisList, trumpCard);
	}

	return scoreForThisList;
}

/* *********************************************************************
Function Name: findCombinations
Purpose: to call the utility function 
Parameters:
			listOfCards, a vector of Card pointers. This holds the list of cards whose total meld score is to be determined.
			sizeOfListOfCards, an integer. It holds the length of listOfCards.
			sizeOfCombinations, an integer. It holds the size of combinations to be made from the list.
			scoreForThisList, an intege passed by reference. It holds the total meld score for this specific list.
			trumpCard, a Card object. This holds the trump card.
Return Value:
			none
Local Variables:
			dataVector, a vector of card objects. It is initialized as an empty vector. It will hold the cards while finding meld combinations.
Algorithm:
			1) calls findCombinations function
Assistance Received: none
********************************************************************* */
void Player::findCombinations(vector<Card*> listOfCards, unsigned int sizeOfListOfCards, unsigned int sizeOfCombinations, unsigned int &scoreForThisList, Card* trumpCard)
{
	//variable to act as a buffer for the utility
	vector<Card*> dataVector(sizeOfCombinations);

	//used for calling melds
	//listOfPossibleMelds.clear();// variable that stores pair of a vector and its points

	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, 0, dataVector, 0, scoreForThisList, trumpCard);

}

/* *********************************************************************
Function Name: utilityForMeldCombinations
Purpose: to the 
Parameters:
			listOfCards, a vector of Card pointers. This holds the list of cards whose total meld score is to be determined.
			sizeOfListOfCards, an integer. It holds the length of listOfCards.
			sizeOfCombinations, an integer. It holds the size of combinations to be made from the list.
			indexForDataVector, an integer. It holds the index of the element in the dataVector
			indexForMainList, an integer. It holds the index for a pivot in the mainlist.
			scoreForThisList, an intege passed by reference. It holds the total meld score for this specific list.
			trumpCard, a Card object. This holds the trump card.
Return Value:
			none
Local Variables:
			none
Algorithm:
			1) if the index for the data vector is the same as the sizeOfCombinations,
				2) evaluate the meld from the collection of cards in the dataVector
				3) add the score to scoreForThisList
			2) make recursive calls until all combinations are made and evaluated
Assistance Received: none
********************************************************************* */
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
			//the section of the code below is for storing the meld so that it can be used while the user want to call meld
			listOfPossibleMelds.push_back(make_pair(dataVector, MELD_POINTS.at(possibleMeld)));

			scoreForThisList += MELD_POINTS.at(possibleMeld);
		}

		//clear "playedCards" variable
		playedCards.clear();

		//scoreForThisList++;

		return;
	}

	//  if all the size of cards are chosen
	if (indexForMainList >= sizeOfListOfCards)
		return;

	// current is included, put next at next location 
	dataVector[indexForDataVector] = listOfCards[indexForMainList];


	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, indexForDataVector + 1, dataVector, indexForMainList + 1, scoreForThisList, trumpCard);

	utilityForMeldCombinations(listOfCards, sizeOfListOfCards, sizeOfCombinations, indexForDataVector, dataVector, indexForMainList + 1, scoreForThisList, trumpCard);
}

/* *********************************************************************
Function Name: setPlayerHand
Purpose: to set Player hand
Parameters:
		cards, a vector of strings passed by value. It holds the player hand in string representation from the loaded file.
Return Value:
		none
Local Variables: 
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::setPlayerHand(vector<string> cards) {
	//vector to store the card objects
	vector<Card*> vectorOfCards;

	//for each card in the string create a card object and insert in the player's hand
	for (int i = 0; i < cards.size(); i++)
	{
		vectorOfCards.push_back(new Card(cards[i][0], cards[i][1]));
	}

	//set the card to player hand
	playerHand.clear();
	playerHand = vectorOfCards;
}

//
/* *********************************************************************
Function Name: setMeldPile
Purpose: to set Player hand
Parameters:
			meldCards, a vector of string vectors. It holds the vector of each active meld in string representation from the loaded file
			trumpCard, a card object. It holds the trumpCard for the loaded game.
Return Value:
			none
Local Variables:
			cardToMeldMapBuffer, an unordered_map. It holds the card to meld entry for cards with repeated melds, denoted by (*) asterisk in the laoded file.
			currentMeld, an integer. It holds the integer index for the type of meld the vector of cards has.
			currentMeldCards, a vector of card object pointers. It holds the card object pointers for the current meld being evaluated.
			possibleCard, a vector of card object pointer. It holds the possible cards that might have been repeated in the melds.
			isCardFound, a boolean. It flags whether the repeated card has been found in the cardToMeldMap.
Algorithm:
			1) for each entry in the meldCards vector, evaluate what kind of meld it contains.
				2) for each of the cards in the entry
					3) if it is a repeated meld card, found by checking for asterisk
						4) find a similar card i.e. same face and suit in the cardToMeldMap
							5) if found push into the list of possible cards
						6) for each card in possible cards
							7) find a card with exactly one meld
								8) if such card is found add the meld to the entry
							9) if such card is not found and the list of possible cards is empty
								10)insert a new entry into the cardToMeldMap
							11) in all cases take the first card in the listOfPossibleCards and add the meld to it's entry in cardToMeldMap
							12) take the card and insert it into currentMeldCards
					13) insert the meld and it's cards in meldToCardMap

Assistance Received: none
********************************************************************* */
void Player::setMeldPile(vector<vector<string>> meldCards, Card* trumpCard)
{
	//variable to act as a buffer for repeated meld cards (i.e. with *)
	unordered_map<Card*, vector<unsigned int>> cardToMeldMapBuffer;

	//variable to store the current Meld
	unsigned int currentMeld;

	for (int i = 0; i != meldCards.size(); i++)
	{
		//insert from meldCards to palyed cards by creating new cards
		for (int j=0; j<meldCards[i].size(); j++)
		{
			playedCards.push_back(new Card(meldCards[i][j][0], meldCards[i][j][1]));
		}

		//evaluate the meld with these cards
		currentMeld = evaluateMeld(trumpCard);
		
		//reset the played cards because we don't need it from now
		for (int j = 0; j < playedCards.size(); j++)
		{
			delete playedCards[j];
		}
		playedCards.clear();

		//variable to store the cards in the current Meld currently being evaluated
		vector<Card*> currentMeldCards;

		//for each of the cards in the meldsCards, evaluate the meld and insert in the respective member maps
		for (int j = 0; j < meldCards[i].size(); j++)
		{
			//variable to store the card
			Card* chosenCard;

			//if the card is being shared with another meld, it has an asterisk
			if (3 == meldCards[i][j].length() && '*' == meldCards[i][j][2])
			{
				//find a card in the cardToMeldBuffer which does not have the "currentMeld" in it
				vector<Card*> possibleCard;

				for (unordered_map<Card*, vector<unsigned int>>::iterator it = cardToMeldMapBuffer.begin(); it != cardToMeldMapBuffer.end(); it++)
				{
					//if a similar card is found, check whether a similar meld already exists
					if ((it->first)->getCardSuit() == meldCards[i][j][1] && (it->first)->getCardFace() == meldCards[i][j][0])
					{
						//searching for the meld in the meld vector
						vector<unsigned int>::iterator foundMeld = std::find((it->second).begin(), (it->second).end(), currentMeld);

						//if not found, add to the list of possible Cards. Ignore if found
						if (foundMeld == (it->second).end())
						{
							possibleCard.push_back((it->first));
						}
					}
				}

				//for each of the possible cards, find out which one has only one meld
				//if such card is found, choose this card
				//otherwise add to the first card in the list

				bool isCardFound = false;

				for (int k = 0; k < possibleCard.size(); k++)
				{
					if ((cardToMeldMapBuffer[possibleCard[i]]).size() == 1)
					{
						chosenCard = possibleCard[i];
						isCardFound = true;
						break;
					}
				}

				//if a card was found
				if (isCardFound)
				{
					(cardToMeldMapBuffer[chosenCard]).push_back(currentMeld);
				}
				//if a card was not found and there was no similar card in the buffer map
				else if(!isCardFound && 0 == possibleCard.size())
				{
					chosenCard = new Card(meldCards[i][j][0], meldCards[i][j][1]);
					//insert into the buffer
					cardToMeldMapBuffer.insert(pair<Card*, vector<unsigned int>>(chosenCard, { currentMeld }));
					//insert into the meld list
					meldPile.push_back(chosenCard);
				}
				else // possible card was found, and there were no cards with a only a single Meld 
				{
					(cardToMeldMapBuffer[possibleCard[0]]).push_back(currentMeld);
				}
			}
			else// it is not being shared with another meld
			{
				//no need to insert in the buffer, directly insert to the main map
				chosenCard = new Card(meldCards[i][j][0], meldCards[i][j][1]);

				cardToMeldMap.insert(pair<Card*, vector<unsigned int>>(chosenCard, { currentMeld }));

				//insert to the meld pile
				meldPile.push_back(chosenCard);
			}

			//add the chosen card to the vector of cards for the current meld
			currentMeldCards.push_back(chosenCard);
		}

		//add to an entry for the meldToCard map
		//if the current meld already exists in the meldToCardMap, push the meld vector
		map<unsigned int, vector<vector<Card*>>>::iterator it = meldToCardMap.find(currentMeld);

		if (it != meldToCardMap.end())
		{
			meldToCardMap[currentMeld].push_back(currentMeldCards);
		}
		//else, create a new entry
		else
		{
			meldToCardMap.insert(pair<unsigned int, vector<vector<Card*>>>(currentMeld, { currentMeldCards }));
		}
	}

	//finally merge the buffer into the main cardToMeldMap
	cardToMeldMap.insert(cardToMeldMapBuffer.begin(), cardToMeldMapBuffer.end());
}

/* *********************************************************************
Function Name: validateMenuInput
Purpose: to set capture pile cards for a player, when the game is loaded
Parameters:
		cards, a vector of strings passed by value. It holds the capture pile in string representation from the loaded file.
Return Value:
		none
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::setCapturePile(vector<string> cards)
{
	//vector to store the card objects
	vector<Card*> vectorOfCards;

	//for each card in the string create a card object and insert in the player's hand
	for (int i = 0; i < cards.size(); i++)
	{
		vectorOfCards.push_back(new Card(cards[i][0], cards[i][1]));
	}

	//set the card to player hand
	playerCapturePile.clear();
	playerCapturePile = vectorOfCards;
}

/* *********************************************************************
Function Name: validateMenuInput
Purpose: To get a string representation of the player's melds
Parameters:
		none
Return Value:
		the generated string for the melds of a player
Local Variables:
			meldString,  a string. It holds the string generated for a user
			currentMeld, an integer. It holds the value of the current meld that is being evaluated
Algorithm:
			1) get each currentMeld from the meldToCard Map
			2) for all such melds, get the list of list of cards which make up a meld
			3) for each list which is a single meld do the following
				4) for each card in the list
					5) add the face and suit to the meldString
					6) for each such meld
						7) check to see if there is another meld other than the currentMeld
						8) Then check if this new meld is still active in the meldToCardMap
						9) if it is active add an asterisk in the meldString
Assistance Received: none
********************************************************************* */
string Player::getMeldString()
{
	//declaring and initializing string variable to combine the melds
	string meldString = "";

	//for each meld in the meldPile to card map
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

/* *********************************************************************
Function Name: validateMenuInput
Purpose: To validate input based on a given range of integers
Parameters:
		start, an integer. It holds the start value of the range of input.
		stop, an integer. It holds the stop value of the range of input.
Return Value:
		the validated input made by the user
Local Variables:
			menuSelection, a integer value to store the final value of user input.
			tempMenuSelection, a string to store the line from the cin stream.
			invalidInput, a flag to see if the input is valid.

Algorithm:
			1) gets the line from user's input and stores in tempMenuSelection
			2)	tries to convert it into an integer and repeatedly checks if it is within the given range

Assistance Received: none
********************************************************************* */
unsigned int Player::validateMenuInput(unsigned int start, unsigned int end)
{
	//declaring and initializing menu selection variables
	unsigned int menuSelection = 0;
	string tempMenuSelection;

	//error check flag for the string conversion
	bool invalidInput;

	do
	{
		invalidInput = false;
		cout << "Input: ";
		getline(cin, tempMenuSelection);

		//validating string to integer cast
		try
		{
			menuSelection = stoi(tempMenuSelection);
		}
		catch (...)
		{
			invalidInput = true;
			cerr << "The input is invalid. Please enter a number between " << start << " and " << end << endl;
			continue;
		}

		//validating input range
		if (!(menuSelection >= start && menuSelection <= end))
		{
			cerr << "The input is invalid. Please enter a number between " << start << " and " << end << endl;
		}
	} while (!(menuSelection >= start && menuSelection <= end && invalidInput == false));

	return menuSelection;
}

/* *********************************************************************
Function Name: resetRoundInfo
Purpose: to reset the information of the user for the round
Parameters:
			none
Return Value: none
Local Variables:
			none
Algorithm:
			1) for each container of card object(hand, meld, capture piles), the cards are deallocated
			2) for information like score is set to zero
			3) Then all containers are reset
Assistance Received: none
********************************************************************* */
void Player::resetRoundInfo()
{
	//delete all cards from playerHand
	for (int i = 0; i < playerHand.size(); i++)
	{
		delete (playerHand[i]);
	}
	//delete all cards from player meldpile
	for (int i = 0; i < meldPile.size(); i++)
	{
		delete (meldPile[i]);
	}
	//delete all from capturepile
	for (int i = 0; i < playerCapturePile.size(); i++)
	{
		delete (playerCapturePile[i]);
	}

	//reset the containers
	playerHand.clear();
	meldPile.clear();
	playedCards.clear();
	playerCapturePile.clear();

	playerRoundScore = 0;
	cardToMeldMap.clear();
	meldToCardMap.clear();
}

Player::~Player()
{
	resetRoundInfo();
}