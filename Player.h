/*
Authors: Carlie Abraham, Brian Lam, Adam Sandor

Date: 4/5/15
Lab: Lab 3.

*/

#pragma once

#include <string>
#include <memory>
#include "Hand.h"
using namespace std;

class Hand;
enum winLoss{
	increment = 1,
	initialWin = 0,
	initialLoss = 0
};
enum fileError{
	error1,	//JZ: are these really helpful labels? These have special significance in your code
			//(are thrown when specific problems are encountered), so give them appropriate labels
			//e.g. "numWonNotInt"
	error2,
	error3
};

struct Player {
	string name;
	Hand hand;
	unsigned int won;
	unsigned int lost;

	Player(string plName);
};

ostream& operator<<(ostream& os, const Player& player);
bool poker_rankTemp(shared_ptr<Player> pl1, shared_ptr<Player> pl2);
