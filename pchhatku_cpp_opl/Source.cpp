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


	/*---------------Deck Class Tests--------------------*/

	//Deck* deck1 = new Deck();
	//cout << "Display 1:" << endl << endl;
	//deck1->DisplayDeck();

	//deck1->shuffleDeck();

	//cout << "Display 1 after shuffle:" << endl << endl;
	//deck1->DisplayDeck();

	//Card* displaycard;
	//for (int i = 0; i < 5; i++)
	//{
	//	displaycard = deck1->dealCard();
	//	cout << "the dealth card is " << displaycard->getCardFace() << " " << displaycard->getCardSuit() << endl;
	//}

	//deck1->DisplayDeck();

	//delete deck1;


	system("pause");
	return 0;
}