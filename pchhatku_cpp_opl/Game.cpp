#include "Game.h"

//class constructor
Game::Game()
{
	// member variable initializations for constructor
	//creating player objects
	listOfPlayers.push_back(new Player());
	listOfPlayers.push_back(new Player());

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
		case 3: cout << "Closing Pinocle..." << endl;
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

	//display game info: winner scores, losers

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
	cout << "Game saved to a file" << endl;
}