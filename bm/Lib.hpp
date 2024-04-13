#pragma once
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

struct PosUVColVertex
{
	float		pos[4] = {};
	float		uv[4] = {};
	float		col[4] = {};

	static void init()
	{
		vtx_layout
			.begin()
			.add(bgfx::Attrib::Position, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexLayout vtx_layout;
};

namespace bm {
	void plop();
	bgfx::TextureHandle getPng(const char * texPath);
}