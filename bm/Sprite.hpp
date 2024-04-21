#pragma once 

#include <vector>
#include "Math.hpp"

namespace bm {

	struct Material {
		bgfx::TextureHandle tex;
		bgfx::UniformHandle sampler;
		bgfx::ProgramHandle shdr;
	};

	struct Sprite {
		Vec2			pos = Vec2(0);
		Vec2			size = Vec2(32);
		Material		mat;
		
		void			draw();
	};

	struct SpriteBatch {
		std::vector<Vec2>	positions;
		std::vector<Vec2>	size;
		std::vector<Vec4>	uv;
		std::vector<Vec4>	colors;
		Material			mat;

		void			draw();
	};
}