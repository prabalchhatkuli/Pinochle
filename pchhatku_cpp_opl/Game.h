#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
#include<iterator>
#include <time.h>
#include "Round.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"
#include <fstream>

using namespace std;

class Round;

class Game 
{
//Round is declared as friend so that it hass access to players and game properties
//This is done because the Round class needs to have access to 
//all of the private members of the Game class.
friend class Round;

public:
	//class constructor
	Game();

	//method for starting the game
	void startGame();

	//method for quitting the game
	void quitGame();

	//input validation method for menu inputs
	unsigned int validateMenuInput(unsigned int start, unsigned int end);

	//destructor
	~Game();

private:
	//member variable declarations
	//Player object pointer for human player and computer
	vector<Player*> listOfPlayers;

	//variable to store total number of rounds for the game
	unsigned int numRounds;

	//variable to store the winner of the last round - 0 -> human, 1-> computer
	unsigned int winnerLastRound;

	//pointer to the current round object
	Round *currentRound;

	//variables to store scores for human and computer
	vector<unsigned int> playerScores;

	//method to load a saved game from a file
	int loadGame();

	//method to save to a file
	void saveGame();
};
