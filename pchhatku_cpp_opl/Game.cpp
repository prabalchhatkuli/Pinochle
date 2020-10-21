#include "Game.h"

/* *********************************************************************
Function Name: Game() constructor
Purpose: To initialize the instance of a game class
Parameters:
		None
Return Value: None
Local Variables:
			None
Algorithm:
			1) Adds player object in the listOfPlayers
			2) initializes playerscores, numRounds(number of rounds), and currentRound which is the round object for the game

Assistance Received: none
********************************************************************* */
//class constructor
Game::Game()
{
	// member variable initializations for constructor
	//creating player objects
	listOfPlayers.push_back(new Human());
	listOfPlayers.push_back(new Computer());

	//player scores
	//initializing all scores to 0
	playerScores.push_back(0);
	playerScores.push_back(0);

	//number of rounds
	numRounds = 1;

	//winner of last round,i.e. the player who will start the game
	//assumed to be the first player
	winnerLastRound = 0;

	//currentRound
	currentRound = NULL;
}

/* *********************************************************************
Function Name: startGame()
Purpose: To start the game
Parameters:
		None
Return Value: None
Local Variables:
			remainingTurns, an intger. it holds number of turns remaining for the game(runs from +12 to -12)
			menuSelection, an integer. it stores the user's selction of menu
			roundInfo, an integer. it stores an integer returned by the round
			winnerIndex,an integer. it stores the index of the winner
Algorithm:
			1) Select the player's choice to start the game
			2) if a new game is started , a coin toss is made
			3) if load game is chosen, loadGame() function is chosen
			4) otherwise, the game calls quitGame() function
			5)Then, the game loop is started, where a new round object is created
			6)based on the value of roundInfo, then it decided whether to quit game, save game, or start a new round

Assistance Received: none
********************************************************************* */
void Game::startGame()
{
	cout << "///** PINOCHLE **\\\\\\" << endl;

	//declaring and initializing menu selection variables
	unsigned int menuSelection = 0;
	unsigned int remainingTurns = 12;

	//Main Menu display
	cout << "Main Menu:" << endl;
	cout << "1. Start a new game" << endl;
	cout << "2. Load an existing game" << endl;
	cout << "3. Quit game" << endl;

	//input menu option and validate
	menuSelection = validateMenuInput(1, 3);

	//switch cases for menuSelection
	switch (menuSelection)
	{
		//load game file method is called
		//all of the game state is set
		case 1 :
			{
				cout << endl << "Starting round: "<< numRounds << endl << endl;
				//coin toss for the first round
				unsigned int coinChoice = 0;
				//seeding random generation
				srand(time(NULL));
				int coinTossResult = rand() % 2 + 1;
				cout << "A coin has been tossed in the air....." << endl;
				cout << "Your call: 1) head || 2) tail: ";

				//input coinChoice and validate
				coinChoice = validateMenuInput(1, 2);

				if (coinChoice == coinTossResult)
				{
					cout << "!!! You win the toss. You start the first round." << endl;
					winnerLastRound = 0;
				}
				else
				{
					cout << "!!! You lose the toss. The Computer Starts the game." << endl;
					winnerLastRound = 1;
				}
			}
			break;
		
		//selecting option 2 loads game from a file
		case 2 :
			//create a new round for the game
			currentRound = new Round(this);
			remainingTurns = loadGame();
			break;

		//selecting option 3 quits the game
		case 3: quitGame();
			return;
	}


	//selection of option 1
	//this point is common for both load game and new game
	//game loop for running the number of rounds
	do
	{
		//single round loop
		if(2 != menuSelection)
			currentRound = new Round(this);

		//reset menu option
		menuSelection = 0;

		//start the round
		unsigned int roundInfo = currentRound->startRound(remainingTurns);

		if (1 == roundInfo)
		{
			saveGame();
			return;
		}
		if (3 == roundInfo)
		{
			quitGame();
			return;
		}

		//end of single round loop
		//reset the remaining turns
		remainingTurns = 12;

		//End of round prompt
		unsigned int winnerIndex = 0;
		for (unsigned int i = 0; i < listOfPlayers.size(); i++)
		{
			if (listOfPlayers[i]->getRoundScore() > listOfPlayers[winnerIndex]->getRoundScore())
				winnerIndex = i;
		}
		cout << "*************************************************" << endl;
		cout << "The winner of the round is :" << (winnerIndex ? "Computer" : "Human") << endl;
		cout << "The scores for the round are :  ";
		for (unsigned int i = 0; i < listOfPlayers.size(); i++)
		{
			cout << listOfPlayers[i]->getRoundScore() << "  ";
		}
		cout << endl << "***************************************************" << endl;
		//asking user for another round
		cout << "Want to start another round?" << endl;
		cout << "1) Yes, Let's start another round." << endl;
		cout << "2) No, I am good for today." << endl;

		//input menu option and validate
		menuSelection = validateMenuInput(1, 2);

		//if selected 1 continue the game and start another round
		if (1 == menuSelection)
		{

			for (unsigned int i = 0; i < listOfPlayers.size(); i++)
			{
				listOfPlayers[i]->addRoundToGameScore();
				listOfPlayers[i]->resetRoundInfo();
			}
			
			//increment number of rounds
			numRounds++; 
			cout << endl << "Starting round: " << numRounds << endl << endl;

			//reset remaining turns for a round
			remainingTurns = 12;

			//deleting the round object
			currentRound->displayDeck();
			delete currentRound;
		}
		else
		{
			quitGame();
			return;
		}

	} while (1 == menuSelection);

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
unsigned int Game::validateMenuInput(unsigned int start, unsigned int end)
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
			cerr << "The input is invalid. Please enter a number between "<<start<<" and "<<end << endl;
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
Function Name: quitGame
Purpose: To display information after quitting the game
Parameters:
		none
Return Value:
		none
Local Variables:
			totalScoresForPlayers, a vector of integers. it holds the scores for each player
			winnerIndex, an integer. it holds the index of the winner

Algorithm:
			1) finds the score for each player
			2) finds the maximum in the list of scores
			3) displays the final game scores for all players

Assistance Received: none
********************************************************************* */
void Game::quitGame()
{
	//variable to store totalScores
	vector<unsigned int> totalScoresForPlayers;

	//variable to store the index of the winner
	unsigned int winnerIndex = 0;

	//find the total game score for each player
	for (unsigned int i = 0; i < listOfPlayers.size(); i++)
	{
		listOfPlayers[i]->addRoundToGameScore();
	}

	//find maximum total scores
	for (unsigned int i = 0; i < listOfPlayers.size(); i++)
	{
		if (listOfPlayers[i]->getGameScore() > listOfPlayers[winnerIndex]->getGameScore())
			winnerIndex = i;
	}

	//display winner
	cout << "The winner is :" << (winnerIndex ? "Computer" : "Human") <<endl;
	cout << "The scores are ";

	for (unsigned int i = 0; i < listOfPlayers.size(); i++)
	{
		cout<<listOfPlayers[i]->getGameScore()<<"  ";
	}
	cout << "." << endl;
}

/* *********************************************************************
Function Name: trim
Purpose: trims a string of succeeding and trailing whitespace
Parameters:
		line, a string with whitespaces succeeding and preceding characters
Return Value:
		a string with trimmed whitespaces
Local Variables:
			WhiteSpace, a string. it holds the whitespace characters to exclude
			start, integer to store the starting index of the normal characters
			stop, integer to store the ending index of the normal characters

Algorithm:
			1) find the start position in the string that is not a whitespace
			2) find the end position in the string that is not a whitespace
			3) splice the string and return

Assistance Received: none
********************************************************************* */
string trim(string line)
{
	string WhiteSpace = " \t\v\r\n";
	//leftside trim
	size_t start = line.find_first_not_of(WhiteSpace);
	if (start == string::npos)
	{
		line = "";
	}
	else
	{
		line = line.substr(start);
	}

	//right side trim
	size_t end = line.find_last_not_of(WhiteSpace);

	if (end == string::npos)
	{
		return "";
	}
	else
	{
		return line.substr(0, end + 1);
	}
}

/* *********************************************************************
Function Name: loadGame
Purpose: function to load saved game from a file
Parameters:
		none
Return Value:
		integer with the remaining turns a value between +12 and -12
Local Variables:
			filename, string to store the input file
			input, input stream object for the file
			line, string to store each read line
			playerNumber, integer to denote the number of player whose information is being read
			totalNumberOfPlayedCards, integer to store the number of cards played in the game
			meldsForPlayers, vector to store the list of melds of cards(string) for each player 
			trumpCard, Card object to store the trumpCard for the loaded game
			slicedVectorOfCards, vector to store the list of strings(cards) in the given line

Algorithm:
			1) read the filename and read each line, one at a time
			2) based on the first string of the line, determine the route
			3) for each of the route, if it is based on cards, slice the cards and send it to the player to handle
			4) for meld cards, read the cards and store it in the meldsForPlayers for each player until the trumpCard is determined
			5)	for played cards, count the played cards, and send the played cards to each player
			6) return the remainingTurns by returning (12 - totalNumberOfPlayedCards / 2)

Assistance Received: none
********************************************************************* */
int Game::loadGame()
{
	//variable for the name of the save file
	string filename;

	//variable for a buffer while reading each line
	string line;

	//variable to store the player number who is currently being evaluated
	unsigned int playerNumber;
	unsigned int totalNumberOfPlayedCards = 0;

	//for two players in our case
	vector<vector<vector<string>>> meldsForPlayers(2);

	//variable to store the trump card
	Card* trumpCard;


	//get the name of file to output
	//make sure to get a .txt extension at the end
	cout << "Please enter the name of the savefile with .txt extension: " << endl;;
	do
	{
		if (cin.peek() == '\n')
		{
			cin.ignore();
		}
		//cin.ignore();
		getline(cin, filename);
	} while (!(filename.length() > 4 && ".txt" == (filename.substr(filename.length() - 4))));

	//file write stream object //open the file to write
	ifstream input(filename.c_str());

	while (getline(input, line))
	{
		//trimming the line to see if it has any content other than whitespace
		line = trim(line);
		if (0 == line.length())
			continue;
		
		//line has only valid content
		istringstream iss(line);

		//create a vector of strings from the line
		vector<string> temp_vector{
			istream_iterator<string>(iss), {}
		};

		//first job is to evaluate the round number
		if (temp_vector[0] == "Round:")
		{
			//the last element is the round number
			numRounds = stoi(temp_vector.back());
			cout << numRounds << " is the round" << endl;
			continue;
		}

		//once round number has been evaluated, skip the next line because it is of human
		//start loop for player
		if (temp_vector[0] == "Human:" || temp_vector[0] == "Computer:")
		{
			//evaluate the player
			if (temp_vector[0] == "Human:") playerNumber = 0;
			else playerNumber = 1;

			continue;
		}


		if (temp_vector[0] == "Score:")
		{
			//for the score line vector
			//1  is Game Score, 3 is Round Score
			listOfPlayers[playerNumber]->setPlayerScores(stoi(temp_vector[1]), stoi(temp_vector[3]));
			continue;
		}

		if (temp_vector[0] == "Hand:")
		{
			//break the vector and send it to the player to create a hand
			cout << "Hand Cards" << endl;
			if (temp_vector.size() > 1)
			{
				vector<string> slicedVectorOfCards(temp_vector.begin() + 1, temp_vector.begin() + temp_vector.size());

				for (int i = 0; i < slicedVectorOfCards.size(); i++)
					cout << slicedVectorOfCards[i] << endl;

				listOfPlayers[playerNumber]->setPlayerHand(slicedVectorOfCards);
			}
			continue;
		}

		if (temp_vector[0] == "Capture")
		{
			//1 is "pile:"
			//break the remaining and send it to the player to create capture pile
			//break the vector and send it to the player to create a hand
			cout << "capture Cards" << endl;
			if (temp_vector.size() > 2)
			{
				vector<string> slicedVectorOfCards(temp_vector.begin() + 2, temp_vector.begin() + temp_vector.size());

				for (int i = 0; i < slicedVectorOfCards.size(); i++)
					cout << slicedVectorOfCards[i] << endl;
				totalNumberOfPlayedCards += slicedVectorOfCards.size();
				listOfPlayers[playerNumber]->setCapturePile(slicedVectorOfCards);
			}
			continue;
		}

		if (temp_vector[0] == "Melds:")
		{
			//break the remaining and send it to the player to create capture pile
			cout << "Meld Cards" << endl;
			if (temp_vector.size() > 1)
			{
				vector<string> slicedVectorOfCards(temp_vector.begin() + 1, temp_vector.begin() + temp_vector.size());

				vector<vector<string>> vectorSingleMelds;
				vector<string> singleMeld;

				//find each meld by searching for comma
				for (int i = 0; i < slicedVectorOfCards.size(); i++)	
				{ 
					//check for the comma to separate melds, or the end of list of cards
					if ((3 == slicedVectorOfCards[i].length() && ',' == slicedVectorOfCards[i][2]) || (i == (slicedVectorOfCards.size()-1)))
					{
						if((3 == slicedVectorOfCards[i].length() && ',' == slicedVectorOfCards[i][2]))
							slicedVectorOfCards[i].erase(slicedVectorOfCards[i].begin() + 2);
						singleMeld.push_back(slicedVectorOfCards[i]);
						vectorSingleMelds.push_back(singleMeld);
					}
					else
					{
						singleMeld.push_back(slicedVectorOfCards[i]);
					}
				}

				meldsForPlayers[playerNumber] = vectorSingleMelds;
			}
			else
			{
				meldsForPlayers[playerNumber] = {};
			}
			continue;
		}

		if (temp_vector[0] == "Trump")
		{
			//the card is in index 2, beacuse index 1 is "card:"
			cout << "Trump card" << temp_vector[2] << endl;
			currentRound->setTrumpCard(temp_vector[2]);
			continue;
		}

		if (temp_vector[0] == "Stock:")
		{
			//send the card to the round to create the deck
			cout << "Stock Cards" << endl;
			if (temp_vector.size() > 1)
			{
				vector<string> slicedVectorOfCards(temp_vector.begin() + 1, temp_vector.begin() + temp_vector.size());

				for (int i = 0; i < slicedVectorOfCards.size(); i++)
					cout << slicedVectorOfCards[i] << endl;

				currentRound->setRoundDeck(slicedVectorOfCards);
			}
			else
			{
				// if the deck is empty, send an empty vector
				currentRound->setRoundDeck(vector<string> {});
			}
			continue;
		}

		if (temp_vector[0] == "Next")
		{
			//the player with next turn is in 2
			//send it to round to set it to the nextTurn variable
			cout << "Next Player is " <<temp_vector[2]<< endl;

			winnerLastRound = (temp_vector[2] == "Human") ? 0 : 1;
			currentRound->setNextTurn(temp_vector[2]);
			continue;
		}
	}

	//once the trumpCard has been determined send the list of melds to respective players to verify
	int counter = 1;
	while (counter >= 0)
	{
		listOfPlayers[counter]->setMeldPile(meldsForPlayers[counter], currentRound->getTrumpCard());
		cout << "Meld loaded for player:" << counter << endl;
		counter--;
	}

	//return the remaining turns left to play for the game: derived from the capture pile number of the players
	return (12 - totalNumberOfPlayedCards / 2);
}

/* *********************************************************************
Function Name: saveGame
Purpose: function to save the game to the file
Parameters:
		none
Return Value:
		none
Local Variables:
			filename, string to store the input file
			output, output stream object
			tempCards, store the vector of cards(played, hand) to output to the file

Algorithm:
			1) input the filename for the file to output
			1) for each player in the game output the played cards, meld cards, hand cards, and other info
			2) output the information about the game by using getter functions

Assistance Received: none
********************************************************************* */
void Game::saveGame()
{	
	//variable for the name of the save file
	string filename;

	//get the name of file to output
	//make sure to get a .txt extension at the end
	cout << "Please enter the name of the savefile with .txt extension: " << endl;;
	do
	{
		if (cin.peek() == '\n')
		{
			cin.ignore();
		}
		//cin.ignore();
		getline(cin, filename);
	} while (!(filename.length() > 4 && ".txt" == (filename.substr(filename.length() - 4))));
	
	//file write stream object //open the file to write
	ofstream output(filename.c_str());

	/*start writing to the file
	 Note: The following is the order in which the game will get saved
	Round: 1

	Computer :
	Score : 0 / 0
	Hand : QH KH QC KD JH XH QS 9H XC JS QD KC
	Capture Pile :
	Melds:

	Human:
	Score: 0 / 22
	Hand : AH AC AS AD XH AC JH QC KD 9S 9D
	Capture Pile : XC JC
	Melds : 9H

	Trump Card : AH
	Stock : KS QH 9C 9S XD KC JD JS KS QD XS 9C JC KH AD 9D AS XS XD JD QS

	Next Player : Human*/
	output << "Round: " << numRounds << endl;
	for (int i = (listOfPlayers.size()-1); i >= 0 ; --i)
	{
		if (1 == i)
			output << endl << "Computer:" << endl;
		else
			output << endl << "Human:" << endl;

		//scores
		output << "\tScore: " << listOfPlayers[i]->getGameScore() <<" / "<< listOfPlayers[i]->getRoundScore() << endl;
		//hand cards
		output << "\tHand:";
		//temporary vector to store hlist of cards

		//for hand cards
		vector<Card*> tempCards = (listOfPlayers[i]->getHandCards());
		for (int j = 0; j < tempCards.size(); j++)
		{
			output << " " << tempCards[j]->getCardFace() << tempCards[j]->getCardSuit();
		}

		output << endl;
		output << "\tCapture Pile:";
		//for the player's capture pile
		tempCards = listOfPlayers[i]->getCaptureCards();
		for (int j = 0; j < tempCards.size(); j++)
		{
			output << " " << tempCards[j]->getCardFace() << tempCards[j]->getCardSuit();
		}

		//for the player's meld pile
		output << endl;
		output << "\tMelds:";
		output << (listOfPlayers[i]->getMeldString()).substr(0, (listOfPlayers[i]->getMeldString()).length()-1);
		output << endl;
	}

	//round information
	output << "Trump card:";

	output << " "<<currentRound->getTrumpCard()->getCardFace() << currentRound->getTrumpCard()->getCardSuit() << endl;

	//Stock Cards:
	output << endl << "Stock:";

	//get all cards in stock pile
	deque<Card*> stockCards = currentRound->getRoundDeck()->getStockCards();

	//output into the file
	for (int i = 0; i < stockCards.size(); i++) {
		output << " " << stockCards[i]->getCardFace() << stockCards[i]->getCardSuit();
	}

	//save next player
	output <<endl<< "Next player:";

	output << " " << (currentRound->getNextPlayer() ? "Computer" : "Human");

	//close file
	output.close();

	//gratitude
	cout << "Game saved"<<endl;
	cout << "Thanks for playing." << endl;
	cout << "Come back anytime." << endl;

	return;

}