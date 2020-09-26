#include "Game.h"

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

	cout << "The Game class is initialized"<<endl;
}

//start game
void Game::startGame()
{
	cout << "///** PINOCHLE **\\\\\\" << endl;

	//declaring and initializing menu selection variables
	unsigned int menuSelection = 0;

	//variable to determine whether to start another round
	bool addRound = true;

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
		case 2 : cout << "ATTENTION:::load game option incomplete" << endl;
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
		currentRound = new Round(this);

		//start the round
		currentRound->startRound();

		//end of single round loop
		
		//asking user for another round

		cout << "Want to start another round?" << endl;
		cout << "1) Yes, Let's start another round." << endl;
		cout << "2) No, I am good for today." << endl;

		//input menu option and validate
		menuSelection = validateMenuInput(1, 2);

		//if selected 1 continue the game and start another round
		if (1 == menuSelection)
		{
			numRounds++;  //increment number of rounds
			cout << endl << "Starting round: " << numRounds << endl << endl;
		}
		//deleting the round object
		currentRound->displayDeck();
		delete currentRound;

	} while (1 == menuSelection);

}


//validation function
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

//quit game
void Game::quitGame()
{
	cout << "quit game option chosen" << endl;

	vector<unsigned int> totalScoresForPlayers;

	unsigned int winnerIndex = 0;

	for (unsigned int i = 0; i < listOfPlayers.size(); i++)
	{
		listOfPlayers[i]->addToGameScore();
	}

	for (unsigned int i = 0; i < listOfPlayers.size(); i++)
	{
		if (listOfPlayers[i]->getGameScore() > listOfPlayers[winnerIndex]->getGameScore())
			winnerIndex = i;
	}

	cout << "The winner is :" << winnerIndex ? "Computer" : "Human";
	

	exit(0);
}

//quit game
void Game::getGameInfo()
{
	cout << "get game info option chosen" << endl;
}

void Game::loadGame()
{
	cout << "Game started loaded from file"<<endl;
}

void Game::saveGame()
{
	cout << "Starting to save game to a file" << endl;
	
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

	//start writing to the file

	//Round: 1

	//Computer :
	//Score : 0 / 0
	//Hand : QH KH QC KD JH XH QS 9H XC JS QD KC
	//Capture Pile :
	//Melds:

	//Human:
	//Score: 0 / 22
	//Hand : AH AC AS AD XH AC JH QC KD 9S 9D
	//Capture Pile : XC JC
	//Melds : 9H

	//Trump Card : AH
	//Stock : KS QH 9C 9S XD KC JD JS KS QD XS 9C JC KH AD 9D AS XS XD JD QS

	//Next Player : Human

	output << "Round: " << numRounds << endl;
	for (unsigned int i = 0; i < listOfPlayers.size(); i++)
	{
		if (1 == i)
			output << endl << "Computer:" << endl;
		else
			output << endl << "Human" << endl;

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
		output << listOfPlayers[i]->getMeldString();
		output << endl;
				
	}

	//round information
	output << "trump card:";

	output << " "<<currentRound->getTrumpCard()->getCardFace() << currentRound->getTrumpCard()->getCardFace() << endl;

	//Stock Cards:
	output << endl << "Stock Cards";

	//get all cards in stock pile
	deque<Card*> stockCards = currentRound->getRoundDeck()->getStockCards();

	//output into the file
	for (int i = 0; i < stockCards.size(); i++) {
		output << " " << stockCards[i]->getCardFace() << stockCards[i]->getCardSuit();
	}

	//save next player
	output <<endl<< "next player:" << endl;

	output << " " << currentRound->getNextPlayer() ? "Computer" : "Human";

	output.close();

	cout << "Game saved";

	exit(0);

}