/*
Authors: Carlie Abraham, Brian Lam, Adam Sandor
Date: 4/5/15
Lab: Lab 3.
Purpose: This is the FiveCardDraw file. It takes care of
the actions that occur in the poker game and how the game resolves.

*/

#include "stdafx.h"
#include "FiveCardDraw.h" 
#include "FiveCardDrawSettings.h"


/*
Constructor that sets the deck to a standard of 52 cards
*/
FiveCardDraw::FiveCardDraw() {
	dealerPos = index::zero;
	discardDeck = {};
	for (int cardSuits = suit::club; cardSuits <= suit::spade; ++cardSuits) {
		for (int cardRanks = value::two; cardRanks <= value::ace; ++cardRanks) {
			value cv = static_cast<value>(cardRanks);
			suit cs = static_cast<suit>(cardSuits);
			Card tempCard;
			tempCard.s = cs;
			tempCard.v = cv;
			deck.add_card(tempCard);
		}
	}
}

/*
Asks the player which cards they would like to discard
*/
int FiveCardDraw::before_turn(Player & pl) {
	vector<int> cardsToBeDiscarded = {};
	bool loopy = true;
	bool error = false;
	cout << endl;
	cout << playerlabel << pl.name << endl;
	cout << handlabel << pl.hand << endl;
	while (loopy) {
		cardsToBeDiscarded = {};
		string cards;
		//asks for player to discard cards
		cout << whichcards << pl.name << liketodiscard << endl;
		cout << inputlabel;
		cin.clear();
		getline(cin, cards);
		cin.clear();
		bool confirm = false;
		//asks if the player really wants to discard nothing
		while (cards.size() == index::zero && confirm == false) {
			string option;
			cout << usurebro << endl;
			cin.clear();
			getline(cin, option);
			cin.clear();
			if (option == yesResponse) {
				confirm = true;
			}
			if (option == noResponse) {
				cout << whichcards << pl.name << liketodiscard << endl;
				cout << inputlabel;
				cin.clear();
				getline(cin, cards);
				cin.clear();
			}
		}
		stringstream ss(cards);
		int index;
		loopy = false;
		//checks input for valid indicies
		while (ss >> index && loopy == false){
			if (index < index::zero || index >= pl.hand.size()){
				cout << validindices << to_string(pl.hand.size()) << endl;
				loopy = true;
			}
			if (loopy == false){
				for (unsigned int i = index::zero; i < cardsToBeDiscarded.size(); i++){
					if (index == cardsToBeDiscarded[i]){
						loopy = true;
						cout << repeatedindex << endl;
					}
				}
			}
			if (loopy == false){
				cardsToBeDiscarded.push_back(index);
			}
		}
	}

	//discard cards
	sort(cardsToBeDiscarded.begin(), cardsToBeDiscarded.end()); //orders indexes from lowest to highest
	if (cardsToBeDiscarded.size() > handCodes::emptyHand){ //if no indicies entered, no deletion necesary
		for (int i = cardsToBeDiscarded.size() - index::one; i >= index::zero; i--){
			Card c = pl.hand[cardsToBeDiscarded[i]];
			discardDeck.add_card(c);
			pl.hand.remove_card(cardsToBeDiscarded[i]);
		}
	}
	return errorMsg::success;
}

/*
Adds stuff to the hands if they discard stuff
*/
int FiveCardDraw::turn(Player & pl) {
	while (pl.hand.size() < gameOfPoker::sizeOfHand){
		if (deck.size() <= index::zero){
			if (discardDeck.size() <= index::zero){
				return errorMsg::notEnoughCards;
			}
			else{
				pl.hand << discardDeck;
			}
		}
		else{
			pl.hand << deck;
			if (deck.size() == index::zero){	//JZ: :|
				discardDeck.shuffle();
			}
		}
	}
	return errorMsg::success;
}

/*
Prints the player name and hand after the turn
*/
int FiveCardDraw::after_turn(Player & pl){
	cout << playerlabel << pl.name << endl;
	cout << handlabel << pl.hand << endl;
	return errorMsg::success;
}

/*
Deals out cards to each person, then calls before_turn method
*/
int FiveCardDraw::before_round(){
	deck.shuffle();
	unsigned int numPlayers = vp.size();
	int cardsToBeDealed = numPlayers * gameOfPoker::sizeOfHand;
	unsigned int position = dealerPos + index::one;
	for (int i = index::zero; i < cardsToBeDealed; i++){

		if (position >= numPlayers){
			position = index::zero;
		}

		//Adds a card to the player's hand from the deck
		if (deck.size() > index::zero) {
			vp[position]->hand << deck;
		}
		else {
			throw errorMsg::noCardsLeft;
		}
		position++;

	}

	//calls before_turn method for each player
	for (unsigned int i = index::zero; i < vp.size(); i++){
		before_turn(*vp[i]);
	}

	return errorMsg::success;
}

/*
Iterates through each player and calls their
turn and after_turn methods
*/
int FiveCardDraw::round(){
	int position = dealerPos + index::one;
	int numPlayers = vp.size();
	for (unsigned int i = index::zero; i < vp.size(); i++){
		if (position >= numPlayers){
			position = index::zero;
		}
		try {
			int num = turn(*vp[position]);
			if (num == index::zero){
				after_turn(*vp[position]);
			}
			else{
				return num;
			}
			position++;
		}
		catch (errorMsg err) {
			throw err;
			//JZ: this is *bad* as it makes 'err' appear to have originated from
			//round() when in fact it probably originated from turn or after_turn.
			//don't catch something at all if you're going to rethrow it, or if
			//absolutely necessary just write 'throw;' 
		}
	}
	return errorMsg::success;
}

/*
Prints out everybody's info and gives the opportunity for
players to enter and leave the game
*/
int FiveCardDraw::after_round(){
	vector<shared_ptr<Player>> tempVP = vector<shared_ptr<Player>>(vp);
	sort(tempVP.begin(), tempVP.end(), poker_rankTemp);

	unsigned int lastPlIndex = tempVP.size() - index::one;

	tempVP[index::zero]->won += winLoss::increment;
	cout << tempVP[index::zero]->name << winround << endl << endl;

	for (unsigned int i = index::one; i <= lastPlIndex; ++i){
		tempVP[i]->lost += winLoss::increment;
	}

	for (shared_ptr<Player> p : tempVP) {
		cout << playerlabel << p->name << endl;
		cout << winlabel << p->won << endl;
		cout << losslabel << p->lost << endl;
		cout << handlabel << p->hand << endl;
	}
	string response;
	do{
		cout << leavegamebro << endl;
		cout << inputlabel;
		cin.clear();
		getline(cin, response);
		cin.clear();
		bool playerExists = true;
		if (response == yesResponse){
			cout << whosleaving << endl;
			string name;
			cout << inputlabel;
			cin.clear();
			getline(cin, name);
			cin.clear();
			shared_ptr<Player> tempPlayer = find_player(name);
			if (tempPlayer == nullptr){
				cout << whoutalkingbout << name << period << endl;
				playerExists = false;
			}
			if (playerExists){
				ofstream ofs;
				ofs.open(tempPlayer->name); //changed
				if (ofs.is_open()){
					ofs << tempPlayer->name << outerspace << tempPlayer->won << outerspace << tempPlayer->lost;
				}
				ofs.close();
				int index = index::negativeOne;
				for (unsigned int i = index::zero; i < vp.size(); i++){
					if (tempPlayer->name == vp[i]->name){
						index = i;
					}
				}
				vp.erase(vp.begin() + index);
			}
		}
	} while (response != noResponse);

	do{
		cout << whoscoming << endl;
		cout << inputlabel;
		cin.clear();
		getline(cin, response);
		cin.clear();
		if (response == yesResponse){
			cout << whatshisnamebro << endl;
			string playerName;
			cout << inputlabel;
			cin.clear();
			getline(cin, playerName);
			cin.clear();
			try{
				add_player(playerName);
			}
			catch (char const* message) {
				cout << message << endl;
			}
			catch (...){
				//JZ: *very* worried by this choice to silently swallow all other errors
				continue;
			}
		}
	} while (response != noResponse);

	dealerPos++;
	if (dealerPos >= vp.size()){
		dealerPos = index::one;
		//JZ: dealerPos never loops back to the first player.
	}

	for (unsigned int i = index::zero; i < vp.size(); i++){
		(*vp[i]).hand = {};
	}
	discardDeck = {};
	deck = {};
	for (int cardSuits = suit::club; cardSuits <= suit::spade; ++cardSuits) {
		for (int cardRanks = value::two; cardRanks <= value::ace; ++cardRanks) {
			value cv = static_cast<value>(cardRanks);
			suit cs = static_cast<suit>(cardSuits);
			Card tempCard;
			tempCard.s = cs;
			tempCard.v = cv;
			deck.add_card(tempCard);
		}
	}
	return errorMsg::success;
}
