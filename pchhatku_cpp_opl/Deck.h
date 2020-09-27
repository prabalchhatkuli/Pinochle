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

	//set the deck with a list of cards
	void setDeck(vector<Card*> cards) { listOfCards.clear(); listOfCards = deque<Card*> (cards.begin(), cards.end());; }

	//for testing only: Displays the current deck
	void DisplayDeck();

	//destructor
	~Deck();

	//Deal card method
	Card* dealCard();

	//get the remaing listOfCards i.e. stock cards
	deque<Card*> getStockCards() { return listOfCards; }

	//number of cards in deck
	unsigned int numberOfCards() {
		return listOfCards.size();
	}

private:
	//constant arrays for the deck
	const char FACES[6] = { '9', 'J', 'Q', 'K', 'X', 'A' };
	const char SUITS[4] = {'H','S','C','D'};

	//cards in deck
	deque<Card*> listOfCards;

};