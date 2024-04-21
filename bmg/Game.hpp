#pragma once

namespace bmg {
	class Game : public bm::Game{
		typedef bm::Game Super;
	public:
		bm::Sprite			red;
		bm::SpriteBatch		bg;
	public:
							Game();
		virtual				~Game() {};

		static bmg::Game*	get();
		virtual void		update(double dt);
		virtual void		draw();
	};
}