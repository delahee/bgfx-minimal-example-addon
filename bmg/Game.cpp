#include "Lib.hpp"
#include "Math.hpp"
#include "bm/Game.hpp"
#include "bm/Sprite.hpp"
#include "bmg/Game.hpp"

#include <GLFW/glfw3.h>

bmg::Game::Game(){
	auto texPixel = bm::getPng("bm/res/pixel.png");
	auto texPhi = bm::getPng("bm/res/phi_angry.png");
	auto texGround = bm::getPng("bm/res/ground.png");
	auto texRed = bm::getPng("bm/res/red.png");
	auto shdr = bm::getSpriteShader();
	auto texSampler = bgfx::createUniform("tex0", bgfx::UniformType::Sampler, 1);

	bm::SpriteBatch sprb;
	int nw = 64;
	int nh = 64;
	sprb.reserve(nw * nh);
	sprb.resize(nw * nh);
	for (int y = 0; y < nh; ++y) {
		for (int x = 0; x < nw; ++x) {
			int idx = x + y * nw;
			sprb.position[idx] = { 0.0f + 16 * 2 * x, 0.0f + 16 * 2 * y };
			sprb.size[idx] = { 16 * 2, 16 * 2 };
			sprb.uv[idx] = { 0, 0, 1, 1 };
			sprb.color[idx] = { 1, 1, 1, 1 };
		}
	}
	sprb.mat = { texGround, texSampler, shdr };

	bm::Sprite spr;
	spr.pos = { 100,100 };
	spr.pivot = { 0.5,0.5 };
	spr.size = { 64 * 2, 64 * 2 };
	spr.mat = { texRed, texSampler, shdr };

	bg = sprb;
	red = spr;
}

void bmg::Game::update(double dt){
}

void bmg::Game::draw(){
	bg.draw();
	red.draw();
}

static bmg::Game * s_game = 0;
bmg::Game* bmg::Game::get(){
	if(!s_game) s_game = new bmg::Game();
	return s_game;
}

void bmg::Game::input(GLFWwindow* window, double dt){
	if (glfwGetKey(window, GLFW_KEY_UP) != 0)
		red.pos.y -= 5 * 60 * dt;
	if (glfwGetKey(window, GLFW_KEY_DOWN) != 0)
		red.pos.y += 5 * 60 * dt;
	if (glfwGetKey(window, GLFW_KEY_LEFT) != 0) 
		red.pos.x -= 5 * 60 * dt;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) != 0)
		red.pos.x += 5 * 60 * dt;
}
