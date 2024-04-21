#include "Lib.hpp"
#include "Math.hpp"
#include "Game.hpp"

using namespace bm;
Game::Game(){
}

void Game::update(double dt){
}

void Game::draw(){
}

static bm::Game* s_game = 0;
bm::Game* bm::Game::get() {
	if (!s_game) s_game = new bm::Game();
	return s_game;
}
