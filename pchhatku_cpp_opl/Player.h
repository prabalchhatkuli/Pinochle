#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include <map>
#include <unordered_map>
#include <string>
#include "Card.h"

using namespace std;

class Player
{
public:
	//class constructor
	Player();

	//display player cards
	void displayPlayerCards(bool);

	//method for input validation
	unsigned int validateMenuInput(unsigned int, unsigned int);

	//method to make a move including menu choices
	virtual unsigned int play(vector<Card*>, Card*) = 0;

	//method to add roundscore to total score
	void addRoundToGameScore(){playerScore += playerRoundScore;}

	//method to get game score
	unsigned int getGameScore() { return playerScore; }

	//method to get round Score
	unsigned int getRoundScore() { return playerRoundScore; }

	//method to get all cards in the hand
	vector<Card*> getHandCards() { return playerHand; }

	//method to get all capture cards for the player
	vector<Card*> getCaptureCards() { return playerCapturePile; }

	//method to set player scores
	void setPlayerScores(unsigned int game, unsigned int round) { playerScore = game; playerRoundScore = round; }

	//method to set player hand cards
	void setPlayerHand(vector<string> cards);

	//method to set meld pile
	void setMeldPile(vector<vector<string>>, Card*);

	//method to set played Cards
	void setCapturePile(vector<string> cards);

	//method to get all the player's cards in a string
	string getMeldString();

	//method to call a meld
	virtual void callMeld(Card*) = 0;

	//method to evaluate the meld made by player
	unsigned int evaluateMeld(Card*);

	//method to add to the player's Hand
	void addToHand(Card* newCard) { playerHand.push_back(newCard); }

	//method to add to capture pile
	void addToCapturePile(Card* captureCard) { playerCapturePile.push_back(captureCard); }

	//method to add to the round score
	void addToRoundScore(int score) { playerRoundScore += score; }

	//method to make a move
	//virtual void makeMove() = 0;

	//method to reset all information stored for the user for a round
	void resetRoundInfo() { playerHand.clear();
							meldPile.clear();
							playedCards.clear();
							playerRoundScore = 0;
							cardToMeldMap.clear();
							meldToCardMap.clear();
							playerCapturePile.clear();}

	//method to reset capture pile

	//method to process played cards
	void processPlayedCards();

	//get the cards chosen by user during the move
	vector<Card*> getPlayedCards() { return playedCards; }


	//clear the played cards
	void clearPlayedCards() { playedCards.clear(); }


	//**********tactics for help and computer player

	//method to call a meld
	void decideMeld(Card*);

	//returns the cheapest card that will win the turn
	Card* getCheapestCard(Card*, Card*);

	//method to get a lead card
	Card* getTacticalCard(Card*);

	//method to get a list of playable cards
	vector<Card*> findPlayableCards(Card*, Card*);

	//compare a two cards
	bool compareTwoCards(Card*, Card*, Card*);

	//find total possible meld score from the combination of all cards in a list
	unsigned int findPossibleScores(vector<Card*>, Card*);

	//function to start recursion
	void findCombinations(vector<Card*>, unsigned int, unsigned int, unsigned int&, Card*);

	//utility function to recurse and evaluate scores
	void utilityForMeldCombinations(vector<Card*>, unsigned int, unsigned int, unsigned int, vector<Card*>, unsigned int, unsigned int&, Card*);

protected:
	//variable declarations
	string playerName;

	//constants
	const map<int, string> MELDS= {
		{1,"flush "},{2,"royal marriage"},{3,"marriage"},{4,"dix"},{5,"four Aces"},{6,"four Kings"},{7,"four Queens"},{8,"four Jacks"},{9,"Pinochle"}
	};

	const map<unsigned int, unsigned int> MELD_POINTS = {
		{1, 150},{2, 40},{3, 20},{4, 10},{5, 100},{6,80},{7, 60},{8, 40},{9, 40}
	};

	//player hand list
	vector<Card*> playerHand;

	//player score
	unsigned int playerScore;

	//player's score for the round
	unsigned int playerRoundScore;

	//Note that: a player's playable card can be either in the capturePile or the meldPile and hand
	//capture pile
	vector<Card*> playerCapturePile;

	//meld pile: cards used for meld
	vector<Card*> meldPile;

	//melds
	//variable to store the melds and corresponding cards for those melds

	map<unsigned int, vector<vector<Card*>>> meldToCardMap;	//what cards does a meld have
	unordered_map<Card*, vector<unsigned int>> cardToMeldMap;  //which melds does a card belong to


	//chosen cards: when a player chooses one/many card at a move/meld
	vector<Card*> playedCards;

	//vector to store the list of possible melds for a list of cards
	vector<pair<vector<Card*>, unsigned int>> listOfPossibleMelds;

	//virtual methods necessary for human and computer
	//method to make a move


};