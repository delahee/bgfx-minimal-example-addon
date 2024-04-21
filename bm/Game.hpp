#pragma once

namespace bm {
	class Game {
	public:
		Game();
		virtual ~Game() {};

		void update(double dt);
		void draw();

		static bm::Game* get();
	};
}