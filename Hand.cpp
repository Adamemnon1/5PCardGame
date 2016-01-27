//
//  Hand.cpp
//  Lab2
//
//  Created by Carlie Abraham on 2/28/15.
//  Copyright (c) 2015 Carlie Abraham. All rights reserved.
//

#include "stdafx.h"
#include "Hand.h"
#include <algorithm>

//default constructor
Hand::Hand(){	//JZ: use base/member initialization list where possible, e.g. Hand::Hand() : hand() {}
	hand = {};
}

//copy constructor
Hand::Hand(const Hand & other){	//JZ: use base/member initialization list, e.g. Hand::Hand() : hand() {}
	hand = {};
	for (unsigned int i = index::zero; i < other.hand.size(); i++){
		hand.push_back(other.hand[i]);
	}
	//JZ: I believe vector has a copy constructor for this purpose.
}

//assignment operator that takes in a reference to a hand object,
//checks for self assignment, then clears the hand vector, then set the hand array to
//the values of the hand vector passed in
Hand & Hand::operator= (const Hand &other){
	if (this != &other){
		hand.clear();
		for (unsigned int i = index::zero; i < other.hand.size(); i++){
			hand.push_back(other.hand[i]);
		}
	}
	return *this;
}

//returns the size of hand
int Hand::size(){
	return hand.size();
}

//checks for equivalence - if the sizes are the different, they are not equivalent
//if the value or suit at any point in the vector is different from the other vector
//then the hands are not equal
bool Hand::operator==(const Hand &other) const{
	if (hand.size() == other.hand.size()){
		for (unsigned int i = index::zero; i<hand.size(); i++){
			if (hand[i].v != other.hand[i].v || hand[i].s != other.hand[i].s){
				return false;
			}
		}
	}
	else{
		return false;
	}
	return true;
	//JZ: note that you should also have just been able to use the vector's operator==:
	//return hand == other.hand
}

//less than operator - returns true if and only if the hand is less than the other hand passed in
bool Hand::operator< (const Hand & other) const{
	int s1 = hand.size();
	int s2 = other.hand.size();
	int minimum = min(s1, s2);
	for (int i = index::zero; i<minimum; i++){
		if (hand[i]<other.hand[i]){
			return true;
		}
		else if (hand[i].v == other.hand[i].v && hand[i].s == other.hand[i].s){
			continue;
		}
		else{
			return false;
		}
	}
	//could save some time by putting the hand.size() < other.hand.size() check at the beginning
	if (s1<s2){
		return true;
	}
	else{
		return false;
	}
}

//to string method that returns a string of all of the card definition strings in the hand
string Hand::toString() const{
	string cardString = "";
	for (unsigned int i = index::zero; i < hand.size(); ++i){
		Card card = hand[i];
		//JZ: leverage the toString() defined by your Card class...
		string suit;
		string value;
		switch (card.s){
		case suit::club: suit = "C"; break;
		case suit::diamond: suit = "D"; break;
		case suit::heart: suit = "H"; break;
		case suit::spade: suit = "S"; break;
		}
		switch (card.v){
		case value::ace: value = "A"; break;
		case value::two: value = "2"; break;
		case value::three: value = "3"; break;
		case value::four: value = "4"; break;
		case value::five: value = "5"; break;
		case value::six: value = "6"; break;
		case value::seven: value = "7"; break;
		case value::eight: value = "8"; break;
		case value::nine: value = "9"; break;
		case value::ten: value = "10"; break;
		case value::jack: value = "J"; break;
		case value::queen: value = "Q"; break;
		case value::king: value = "K"; break;
		}
		cardString += value;
		cardString += suit;
		cardString += " ";
		//JZ: see: shlemiel the painter algorithm; consider using some sort of stringbuilder for
		//repeated concatenations.
	}
	return cardString;
}

//insertion operator that prints out a string of card definitions that pertain to the hand to the output stream
ostream & operator<<(ostream & out, const Hand & h) {
	out << h.toString() << endl;
	return out;
}

//insertion operator that takes a card from a deck and inserts it into the Hand
//the cards are maintained in sorted order
Hand & operator<<(Hand & h, Deck & d){
	Card tempCard = d.deck[d.size() - 1];
	d.deck.pop_back();
	h.hand.push_back(tempCard);
	sort(h.hand.begin(), h.hand.end());
	return h;
}


//poker rank method that takes in two hands, then compares them according to their poker rank
//if the poker ranks are the same, there is a switch statement to compare the hands in further detail
bool poker_rank(const Hand & h1, const Hand & h2){
	vector<Card> hand1 = h1.hand;
	vector<Card> hand2 = h2.hand;

	//if the sizes are not correct, then an exception is thrown
	if (hand1.size() == gameOfPoker::sizeOfHand && hand2.size() == gameOfPoker::sizeOfHand){
		//determines ranks of hands, then compares them
		pokerRank r1 = determineRank(hand1);
		pokerRank r2 = determineRank(hand2);
		if (r1<r2){
			return false;
		}
		else if (r1>r2){
			return true;
		}
		else{
			//if the ranks are the same, a switch statement is used to call helper methods that compare hadns futher
			switch (r1){
			case pokerRank::noRankHand: {return compareNoRank(hand1, hand2); break; }
			case pokerRank::onePairHand: {return compareOnePair(hand1, hand2); break; }
			case pokerRank::twoPairHand: {return compareTwoPair(hand1, hand2); break; }
			case pokerRank::threeKindHand: {return compareThreeKind(hand1, hand2); break; }
			case pokerRank::straightHand: {return compareStraight(hand1, hand2); break; }
			case pokerRank::flushHand: {return compareFlush(hand1, hand2); break; }
			case pokerRank::fullHouseHand: {return compareFullHouse(hand1, hand2); break; }
			case pokerRank::fourKindHand: {return compareFourKind(hand1, hand2);  break; }
			case pokerRank::straightFlushHand: {return compareStraightFlush(hand1, hand2); break; }
			}
		}
	}
	else{
		throw errorMsg::handsNotCorrectSize;
	}
	return false;
}

//method that compares straights according to their highest card
//JZ: this *does not* actually have *any* logic beyond compareStraight
//moreover, because this does not consider suits, it's possible that
//compareStraightFlush(h1, h2) && compareStraightFlush(h2, h1) are equivalent
bool compareStraightFlush(vector<Card> h1, vector<Card> h2){
	//first determines if the hands have an ace present, and if so, if that ace is in a high striaght (10,J,Q,K,A) or a low striahgt (A,2,3,4,5)
	bool h1HasAce = h1[order::fifth].v == value::ace;
	bool h1HighStraight = false;
	if (h1HasAce){
		if (h1[order::fourth].v == value::king){
			h1HighStraight = true;
		}
	}
	bool h2HasAce = h2[order::fifth].v == value::ace;
	bool h2HighStraight = false;
	if (h2HasAce){
		if (h2[order::fourth].v == value::king){
			h2HighStraight = true;
		}
	}
	//if both hands have aces, this block compares what kind of aces and returns a boolean value accordingly
	if (h1HasAce && h2HasAce){
		if (h1HighStraight == true && h2HighStraight == true){
			return false;
		}
		else if (h1HighStraight == true && h2HighStraight == false){
			return true;
		}
		else if (h1HighStraight == false && h2HighStraight == true){
			return false;
		}
		else{
			return false;
		}

		//JZ: this expression could've been simplified significantly. At this point, equivalent to just:
		//return h1Highstraight && !h2HighStraight;
		//all other cases evaluate to false, as you want and expect.
	}
	//compares case if h1 has an ace, and h2 doesnt
	else if (h1HasAce == true && h2HasAce == false){
		//JZ: these '== true' checks are overcomplicated compared to just "if (h1HasAce && !h2HasAce)
		if (h1HighStraight){
			return true;
			//JZ: just return the expression
		}
		else{
			return false;
		}
	}
	//compares case if h2 has an ace, and h1 doesnt
	else if (h1HasAce == false && h2HasAce == true){
		if (h2HighStraight){
			//JZ: just return the expression
			return false;
		}
		else{
			return true;
		}
	}
	//compares case if no aces are present
	else{
		if (h1[order::fifth].v>h2[order::fifth].v){
			//JZ: just return the expression
			return true;
		}
		else{
			return false;
		}
	}
}
//method that compares straights according to their highest card
bool compareStraight(vector<Card> h1, vector<Card> h2){
	//first determines if the hands have an ace present, and if so, if that ace is in a high striaght (10,J,Q,K,A) or a low striahgt (A,2,3,4,5)
	bool h1HasAce = h1[order::fifth].v == value::ace;
	bool h1HighStraight = false;
	if (h1HasAce){
		if (h1[order::fourth].v == value::king){
			h1HighStraight = true;
		}
	}
	bool h2HasAce = h2[order::fifth].v == value::ace;
	bool h2HighStraight = false;
	if (h2HasAce){
		if (h2[order::fourth].v == value::king){
			h2HighStraight = true;
		}
	}
	//if both hands have aces, this block compares what kind of aces and returns a boolean value accordingly
	if (h1HasAce && h2HasAce){
		if (h1HighStraight == true && h2HighStraight == true){
			return false;  //h1[order::fifth].s>h1[order::fifth].s;
		}
		else if (h1HighStraight == true && h2HighStraight == false){
			return true;
		}
		else if (h1HighStraight == false && h2HighStraight == true){
			return false;
		}
		else{
			return false;//h1[order::fifth].s>h1[order::fifth].s;
		}
	}
	//compares case if h1 has an ace, and h2 doesnt
	else if (h1HasAce == true && h2HasAce == false){
		if (h1HighStraight){
			return true;
		}
		else{
			return false;
		}
	}
	//compares case if h2 has an ace, and h1 doesnt
	else if (h1HasAce == false && h2HasAce == true){
		if (h2HighStraight){
			return false;  //changed
		}
		else{
			return true; //changed
		}
	}
	//compares case if no aces are present
	else{
		if (h1[order::fifth].v>h2[order::fifth].v){
			return true;
		}
		else{
			return false;
		}
	}
}
//compares flushes by comparing the highest card. If the highest card is the same, it compares the next
//highest card, then the next highest card, etc.
bool compareFlush(vector<Card> h1, vector<Card> h2){
	for (int i = h1.size() - index::one; i >= index::zero; --i){
		if (h1[i].v>h2[i].v){
			return true;
		}
		else if (h1[i].v<h2[i].v){
			return false;
		}
		else{
			continue;
		}
	}
	return false;
}
//compares four of a kind, returns true if the value of the 4 cards that have the same
//value in h1 is higher that that of h2
bool compareFourKind(vector<Card> h1, vector<Card> h2){
	if (h1[order::third].v>h2[order::third].v){
		return true;
	}
	else{
		return false;
	}
}
//compares full house hands, first by the value of the three cards that have the same rank,
//and then the two cards that have the same rank
bool compareFullHouse(vector<Card> h1, vector<Card> h2){
	//first determines if the order of the full house has three of the same card, then two
	//or two of the same card, then three
	bool h1ThreeTwo = (h1[order::first].v == h1[order::second].v &&
		h1[order::second].v == h1[order::third].v &&
		h1[order::fourth].v == h1[order::fifth].v);
	bool h2ThreeTwo = (h2[order::first].v == h2[order::second].v &&
		h2[order::second].v == h2[order::third].v &&
		h2[order::fourth].v == h2[order::fifth].v);

	value h1HighestThree;
	value h1HighestTwo;
	if (h1ThreeTwo){
		h1HighestThree = h1[order::first].v;
		h1HighestTwo = h1[order::fifth].v;
	}
	else{
		h1HighestThree = h1[order::fifth].v;
		h1HighestTwo = h1[order::first].v;
	}

	value h2HighestThree;
	value h2HighestTwo;
	if (h2ThreeTwo){
		h2HighestThree = h2[order::first].v;
		h2HighestTwo = h2[order::fifth].v;
	}
	else{
		h2HighestThree = h2[order::fifth].v;
		h2HighestTwo = h2[order::first].v;
	}

	//compares highest three value, then if they are the same, returns the highest two value
	if (h1HighestThree>h2HighestThree){
		return true;
	}
	else if (h1HighestThree<h2HighestThree){
		return false;
	}
	else{
		return h1HighestTwo>h2HighestTwo;
	}
	return false;
}
//compares three of a kind hands, and returns true of h1 is greater than h2
bool compareThreeKind(vector<Card> h1, vector<Card> h2){
	value h1ThreeKind = h1[order::third].v;
	value h2ThreeKind = h2[order::third].v;
	if (h1ThreeKind>h2ThreeKind){
		return true;
	}
	else if (h1ThreeKind<h2ThreeKind){
		return false;
	}
	else{
		//compares hands with the value of the 3 cards with the same value being the same
		//first compiles a vector of values that are different from the h1/h2ThreeKind value
		vector<value> h1extraneous;
		vector<value> h2extraneous;
		for (int i = h1.size() - index::one; i >= index::zero; i--){
			if (h1[i].v != h1ThreeKind){
				h1extraneous.push_back(h1[i].v);
			}
		}
		for (int i = h2.size() - 1; i >= index::zero; i--){
			if (h2[i].v != h2ThreeKind){
				h2extraneous.push_back(h2[i].v);
			}
		}
		//compare values in the extraneous vector
		for (unsigned int i = index::zero; i<h1extraneous.size(); i++){
			if (h1extraneous[i]>h2extraneous[i]){
				return true;
			}
			else if (h1extraneous[i]<h2extraneous[i]){
				return false;
			}
			else{
				continue;
			}
		}
		return false;
	}
}
//compares two hands that have two pairs of two cards
//returns true of h1 has a higher ranked hand
bool compareTwoPair(vector<Card> h1, vector<Card> h2){
	vector<int> h1PairIndexes;
	vector<int> h2PairIndexes;
	//for loop to determine the values of the pairs in h1 and h2
	for (unsigned int i = index::zero; i < h1.size() - index::one; i++){
		if (h1[i].v == h1[i + index::one].v){
			h1PairIndexes.push_back(i);
		}
		if (h2[i].v == h2[i + index::one].v){
			h2PairIndexes.push_back(i);
		}
	}
	//compares the pairs within a hand
	value h1Pair1 = h1[h1PairIndexes[order::first]].v;
	value h1Pair2 = h1[h1PairIndexes[order::second]].v;
	value h1MaxPair;
	value h1MinPair;
	if (h1Pair1>h1Pair2){
		h1MaxPair = h1Pair1;
		h1MinPair = h1Pair2;
	}
	else{
		h1MaxPair = h1Pair2;
		h1MinPair = h1Pair1;
	}
	value h2Pair1 = h2[h2PairIndexes[order::first]].v;
	value h2Pair2 = h2[h2PairIndexes[order::second]].v;
	value h2MaxPair;
	value h2MinPair;
	if (h2Pair1>h2Pair2){
		h2MaxPair = h2Pair1;
		h2MinPair = h2Pair2;
	}
	else{
		h2MaxPair = h2Pair2;
		h2MinPair = h2Pair1;
	}
	//compares pairs between hands
	if (h1MaxPair>h2MaxPair){
		return true;
	}
	else if (h1MaxPair<h2MaxPair){
		return false;
	}
	else{
		if (h1MinPair>h2MinPair){
			return true;
		}
		else if (h1MinPair<h2MinPair){
			return false;
		}
		else{
			//if the pairs of pairs are the same between hands, the remaining card is compared
			value h1extraneousValue;
			for (unsigned int i = index::zero; i<h1.size(); i++){
				if (h1[i].v != h1Pair1 && h1[i].v != h1Pair2){
					h1extraneousValue = h1[i].v;
				}
			}
			value h2extraneousValue;
			for (unsigned int i = index::zero; i<h2.size(); i++){
				if (h2[i].v != h2Pair1 && h2[i].v != h2Pair2){
					h2extraneousValue = h2[i].v;
				}
			}
			return h1extraneousValue>h2extraneousValue;
		}
	}
	return false;
}
//compares hands that have one pair
bool compareOnePair(vector<Card> h1, vector<Card> h2){
	vector<int> h1PairIndexes;
	vector<int> h2PairIndexes;
	//for loop to determine the location of the pair
	for (unsigned int i = index::zero; i < h1.size() - index::one; i++){
		if (h1[i].v == h1[i + 1].v){
			h1PairIndexes.push_back(i);
			h1PairIndexes.push_back(i + 1);
		}
		if (h2[i].v == h2[i + 1].v){
			h2PairIndexes.push_back(i);
			h2PairIndexes.push_back(i + 1);
		}
	}
	value h1PairValue = h1[h1PairIndexes[order::first]].v;
	value h2PairValue = h2[h2PairIndexes[order::first]].v;
	//compares values of pair between ahnds
	if (h1PairValue>h2PairValue){
		return true;
	}
	else if (h1PairValue<h2PairValue){
		return false;
	}
	else{
		//if the pair was the same, the other values are compared, and returns true
		//if h1 has higher values than h2
		int h1index = h1.size() - 1;
		int h2index = h2.size() - 1;
		while (h1index >= index::zero && h2index >= index::zero){
			if (h1[h1index].v == h1PairValue){
				h1index--;
			}
			else if (h2[h2index].v == h2PairValue){
				h2index--;
			}
			else{
				if (h1[h1index].v>h2[h2index].v){
					return true;
				}
				else if (h1[h1index].v<h2[h2index].v){
					return false;
				}
				else{
					h1index--;
					h2index--;
				}
			}
		}
	}
	return false;
}
//compares hands with no rank by looking at the highest card in each hand
bool compareNoRank(vector<Card> h1, vector<Card> h2){
	for (int i = handCodes::handSize - index::one; i >= index::zero; --i){
		if (h1[i].v>h2[i].v){
			return true;
		}
		else if (h1[i].v<h2[i].v){
			return false;
		}
		else{
			continue;
		}
	}
	return false;
}
//function to determine poker rank of a hand
pokerRank determineRank(const vector<Card> & pokerHand){
	int size = pokerHand.size();
	if (size == gameOfPoker::sizeOfHand){
		bool flush, straight, fourKind, fullHouse, threeKind, twoPair, onePair;
		isFlushOrStraight(pokerHand, flush, straight);
		isFourKind(pokerHand, fourKind);
		isFullHouse(pokerHand, fullHouse);
		isThreeKind(pokerHand, threeKind);
		isTwoPairOrOnePair(pokerHand, twoPair, onePair);
		if (straight && flush){
			return pokerRank::straightFlushHand;
		}
		else if (fourKind){
			return pokerRank::fourKindHand;
		}
		else if (fullHouse){
			return pokerRank::fullHouseHand;
		}
		else if (flush){
			return pokerRank::flushHand;
		}
		else if (straight){
			return pokerRank::straightHand;
		}
		else if (threeKind){
			return pokerRank::threeKindHand;
		}
		else if (twoPair){
			return pokerRank::twoPairHand;
		}
		else if (onePair){
			return pokerRank::onePairHand;
		}
		else{
			return pokerRank::noRankHand;
		}
	}
	return pokerRank::noRankHand;
}
//helper function that determines poker rank of a Hand
pokerRank determineRank(const Hand & pHand){ // added this
	vector<Card> pokerHand = pHand.hand;
	int size = pokerHand.size();
	if (size == 5){//this needs to be changed - enum
		//JZ: whoops? magic number
		bool flush, straight, fourKind, fullHouse, threeKind, twoPair, onePair;
		isFlushOrStraight(pokerHand, flush, straight);
		isFourKind(pokerHand, fourKind);
		isFullHouse(pokerHand, fullHouse);
		isThreeKind(pokerHand, threeKind);
		isTwoPairOrOnePair(pokerHand, twoPair, onePair);
		if (straight && flush){
			return pokerRank::straightFlushHand;
		}
		else if (fourKind){
			return pokerRank::fourKindHand;
		}
		else if (fullHouse){
			return pokerRank::fullHouseHand;
		}
		else if (flush){
			return pokerRank::flushHand;
		}
		else if (straight){
			return pokerRank::straightHand;
		}
		else if (threeKind){
			return pokerRank::threeKindHand;
		}
		else if (twoPair){
			return pokerRank::twoPairHand;
		}
		else if (onePair){
			return pokerRank::onePairHand;
		}
		else{
			return pokerRank::noRankHand;
		}
	}
	return pokerRank::noRankHand;
}

//determine if the hand is a flush or a straight
void isFlushOrStraight(vector<Card> hand, bool &flush, bool &straight){
	flush = straight = true;
	//for each card in the hand, check to see if the next card has the same
	//suit, or has a rank one higher
	for (unsigned int i = 0; i < hand.size() - 1; i++){
		if (hand[i].s != hand[i + 1].s){
			flush = false;
		}
		if (hand[i].v != (hand[i + 1].v - 1)){
			straight = false;
		}
	}
	//special case involving Ace, two, three, four, five, which is a straight (with ace low)
	if (hand[order::first].v == value::two &&
		hand[order::second].v == value::three &&
		hand[order::third].v == value::four &&
		hand[order::fourth].v == value::five &&
		hand[order::fifth].v == value::ace){
		straight = true;
	}
}

//checks to see if there is four of a kind
void isFourKind(vector<Card> hand, bool & fourKind){
	fourKind = false;
	bool oneFour, fourOne;
	oneFour = (hand[order::first].v == hand[order::fourth].v);
	fourOne = (hand[order::second].v == hand[order::fifth].v);
	fourKind = oneFour || fourOne;
}

//check for full house
void isFullHouse(vector<Card> hand, bool & fullHouse){
	bool threeTwo, twoThree;
	threeTwo = (hand[order::first].v == hand[order::second].v &&
		hand[order::second].v == hand[order::third].v &&
		hand[order::fourth].v == hand[order::fifth].v);
	twoThree = (hand[order::first].v == hand[order::second].v &&
		hand[order::third].v == hand[order::fourth].v &&
		hand[order::fourth].v == hand[order::fifth].v);
	fullHouse = (threeTwo || twoThree);
}

//check for three of a kind
void isThreeKind(vector<Card> hand, bool & threeKind){
	bool threeOneOne, oneThreeOne, oneOneThree;
	threeOneOne = (hand[order::first].v == hand[order::second].v &&
		hand[order::second].v == hand[order::third].v &&
		hand[order::fourth].v != hand[order::first].v &&
		hand[order::fifth].v != hand[order::first].v &&
		hand[order::fourth].v != hand[order::fifth].v);

	oneThreeOne = (hand[order::second].v == hand[order::third].v &&
		hand[order::third].v == hand[order::fourth].v &&
		hand[order::first].v != hand[order::second].v &&
		hand[order::fifth].v != hand[order::second].v &&
		hand[order::first].v != hand[order::fifth].v);

	oneOneThree = (hand[order::third].v == hand[order::fourth].v &&
		hand[order::fourth].v == hand[order::fifth].v &&
		hand[order::first].v != hand[order::third].v &&
		hand[order::second].v != hand[order::third].v &&
		hand[order::first].v != hand[order::second].v);
	//JZ: or look for a run of length three...
	threeKind = (threeOneOne || oneThreeOne || oneOneThree);
}

//check for pairs
void isTwoPairOrOnePair(vector<Card> hand, bool & twoPair, bool & onePair){
	twoPair = onePair = false;
	int numPairs = pairs::noPairs;
	for (unsigned int i = index::zero; i < hand.size() - 1; i++){
		if (hand[i].v == hand[i + index::one].v){
			numPairs++;
		}
	}
	if (numPairs == pairs::twoPairs){
		twoPair = true;
	}
	if (numPairs == pairs::onePair){
		onePair = true;
	}
}

//retrieves the hand object at a location if its there
Card Hand::operator[](size_t loc) {
	if (loc >= hand.size()) {
		//JZ: interesting choice to throw a string
		throw ("Card not in Hand"); //card not there
	}
	return hand[loc];
}

//removes a card at a location if its there
void Hand::remove_card(size_t loc) {
	if (loc >= hand.size()) {
		throw ("No card at loc");
	}
	hand.erase(hand.begin() + loc);
}
