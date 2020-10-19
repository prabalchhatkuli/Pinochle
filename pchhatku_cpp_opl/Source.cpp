#include<iostream>
#include "Game.h"
#include "Deck.h"
using namespace std;

int main()
{
	//Initializing a game instance
	Game* game1 = new Game();

	//Starting the game
	game1->startGame();

	delete game1;

	system("pause");
	return 0;
}