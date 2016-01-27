/*
Authors: Carlie Abraham, Brian Lam, Adam Sandor
Date: 4/5/15
Lab: Lab 3.
Purpose: The Game begins and ends the poker game.
It also keeps track of the players involved.

*/

#include "stdafx.h"
#include "card.h"
#include "game.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "fivecarddraw.h"
#include "GameSettings.h"

using namespace std;

shared_ptr<Game> Game::gamePointer = nullptr;

//JZ: use base/member initialization list, i.e. Game::Game() : deck(), vp() {}
Game::Game() {
	deck = {};
	vp = {};

}

shared_ptr<Game> Game::instance() {
	if (gamePointer == nullptr){
		throw(err_noinstance);
	}
	else{
		return gamePointer;
	}
}

void Game::start_game(const string & g) {
	if (!(gamePointer == nullptr)) {
		throw(err_gamestarted);
	}
	if (g != game_fivecarddraw) {
		//JZ: consider making this a case-insensitive comparison
		throw(err_unknowngame);
	}
	gamePointer = make_shared<FiveCardDraw>();
}

void Game::stop_game() {
	if (gamePointer == nullptr) {
		throw(err_nogameprogress);
	}
	shared_ptr<Game> temp;
	gamePointer = temp;
}

void Game::add_player(const string & plName) {
	for (shared_ptr<Player> sp : vp) {
		if (sp->name == plName) {
			throw(err_playeralreadyplaying);
		}
	}

	Player pl = Player(plName);
	shared_ptr<Player> spp = make_shared<Player>(pl);
	vp.push_back(spp);
}

shared_ptr<Player> Game::find_player(const string & plName) {
	for (shared_ptr<Player> sp : vp) {
		string na = sp->name;
		if (na == plName) {
			return sp;
		}
	}

	shared_ptr<Player> spp;
	return spp;
}

Game::~Game() {
}

int Game::playerCount(){
	return vp.size();
}
