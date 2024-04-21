#include "Lib.hpp"
#include "Math.hpp"
#include "Game.hpp"

using namespace bm;
Game::Game(){
}

void Game::update(double){
}

void Game::draw(){
}

static bm::Game* s_game = 0;
void bm::Game::input(GLFWwindow*, double )
{
}
bm::Game* bm::Game::get() {
	if (!s_game) s_game = new bm::Game();
	return s_game;
}
