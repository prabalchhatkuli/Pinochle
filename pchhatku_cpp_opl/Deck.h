#pragma once
#include<iostream>
#include<vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <deque>
#include "Card.h"

using namespace std;

class Deck
{
public:
	//class contructor
	Deck();

	//randomly suffle the deck
	void shuffleDeck();

	//for testing only: Displays the current deck
	void DisplayDeck();

	//destructor
	~Deck();

	//Deal card method
	Card* dealCard();

	//number of cards in deck
	unsigned int numberOfCards() {
		return listOfCards.size();
	}

private:
	//constant arrays for the deck
	const int FACES[6] = { 9, 11, 12, 13, 10, 1 };
	const char SUITS[4] = {'H','S','C','D'};

	//cards in deck
	deque<Card*> listOfCards;

	//cards out of deck
	deque<Card*> dealtCards;
};