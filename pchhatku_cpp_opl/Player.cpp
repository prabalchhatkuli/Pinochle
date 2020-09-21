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
		std::cout << " " << it->first << ":" << "[";
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
	cout << "your score till now: " << playerRoundScore << "/" << playerScore << endl;
	cout << "Make a move(option):" << endl << endl;
	//input card for the move
	cout << "select a card to run: 0 - " << (playerHand.size() + meldPile.size() - 1) << endl;
	
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
	vector<unsigned int> listOfChosenIndex;
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
		//insert the card index in a list
		listOfChosenIndex.push_back(cardNumber);

		//decrement the meld card counters
		numberOfMeldCards--;
	}
	
	//evaluate the meld
	unsigned int meldIndex = evaluateMeld(trumpCard);

	//empty the played cards
	playedCards.clear();

	//add the score to the current round score
	playerRoundScore += MELD_POINTS.at(meldIndex); //.at() is used because MELD_POINT is a constant

	//process the move of card and such
	if ( 0 != meldIndex )
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
			if ((*it-removeCount) >= playerHand.size())
			{
				unordered_map<Card*, vector<unsigned int>>::iterator foundCard = cardToMeldMap.find(meldPile[*it-playerHand.size()-removeCount]);
				
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
				meldPile.push_back(playerHand[*it-removeCount]);

				//add the card to the list for this meld
				cardsOfAMeld.push_back(playerHand[*it - removeCount]);

				//insert into card to meld map
				cardToMeldMap.insert(pair<Card*, vector<unsigned int>>(playerHand[*it - removeCount], {meldIndex}));

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
	else//else: the cards are in an active meld
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
		//find the corresponding entries in meldToCardMap
		//remove the vectors from the entry that has the card
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

	cout << "sorted chosen cards" << endl;
	for (vector<Card*>::iterator it = playedCards.begin(); it != playedCards.end(); ++it)
	{
		cout << " " << (*it)->getCardFace() << (*it)->getCardSuit() << " | ";
	}

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
			cout << "dix" << endl;
			possibleMeld = 4;
		}
		break;
	case 2:
		//King and Queen of Trump suit - called royal marriage - 40 points
		if ('K' == playedCards[0]->getCardFace() && 'Q' == playedCards[1]->getCardFace() && playedCards[0]->getCardSuit() == playedCards[1]->getCardSuit())
		{
			if (trumpCard->getCardSuit() == playedCards[0]->getCardSuit())
			{
				cout << "ROYAL marriage" << endl;
				possibleMeld = 2;
			}
			//King and Queen of any other suit - called marriage - 20 points
			else
			{
				cout << "marriage" << endl;
				possibleMeld = 3;
			}
		}

		//Queen of Spades and Jack of Diamonds - called Pinochle - 40 points
		if ('Q' == playedCards[0]->getCardFace() && 'S' == playedCards[0]->getCardSuit())
		{
			if ('J' == playedCards[1]->getCardFace() && 'D' == playedCards[1]->getCardSuit())
			{
				cout << "PINOCHLE" << endl;
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
			possibleMeld = 5;
			break;
		case 'K':
			cout << "four kings" << endl;
			possibleMeld = 6;
			break;
		case 'Q':
			cout << "four queens" << endl;
			possibleMeld = 7;
			break;
		case 'J':
			cout << "four jacks" << endl;
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
				cout << "flush" << endl;
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
	if (cardsInMeldPile > 0)
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
				if (ix != (foundCard->second).end())
					cout << "meld already made for this card" << endl;
				return 0;
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
