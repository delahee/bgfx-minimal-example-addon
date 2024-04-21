#pragma once

struct GLFWwindow;

namespace bm {
	class Game {
	public:
		Game();
		virtual ~Game() {};

		virtual void input(GLFWwindow*,double dt);
		virtual void update(double);
		virtual void draw();

		static bm::Game* get();
	};
}