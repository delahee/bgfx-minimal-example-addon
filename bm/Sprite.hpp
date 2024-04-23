#pragma once 

#include <vector>
#include "Math.hpp"

namespace bm {

	struct Material {
		Tex					tex;
		bgfx::UniformHandle sampler;
		bgfx::ProgramHandle shdr;
	};

	struct Sprite {
		Vec2			pos = Vec2(0);
		Vec2			size = Vec2(32);
		Vec2			pivot = Vec2(0);
		Material		mat;
		
		void			draw();
	};

	struct SpriteBatch {
		std::vector<Vec2>	position;
		std::vector<Vec2>	size;
		std::vector<Vec4>	uv;
		std::vector<Vec4>	color;
		Material			mat;

		void				reserve(int nb);
		void				resize(int nb);
		void				draw();
	};
}