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