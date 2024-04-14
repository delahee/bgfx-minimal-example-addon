#pragma once

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include "Math.hpp"

struct PosUVColVertex{
	float		pos[3];
	float		uv[2];
	float		col[4];

	static void init()
	{
		vtx_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
			.end();
	};

	inline void setPos(const bx::Vec3& v) {
		pos[0] = v.x;
		pos[1] = v.y;
		pos[2] = v.z;
	};

	inline void setUV(const bx::Vec3& v) {
		uv[0] = v.x;
		uv[1] = v.y;
	};

	inline void setCol(const bm::Vec4& v) {
		col[0] = v.x;
		col[1] = v.y;
		col[2] = v.z;
		col[3] = v.w;
	};

	static bgfx::VertexLayout vtx_layout;
};

namespace bm {
	void				plop();
	bgfx::TextureHandle getPng(const char * texPath);
	bgfx::ProgramHandle	getSpriteShader();

	static bm::Matrix4	model;
	static bm::Matrix4	view;
	static bm::Matrix4	proj;
	void				clear();
	void				makeMVP(int bbW, int bbH);
	void				makeRenderStates();
	void				setShader(bgfx::ProgramHandle s);

	void				drawTri(); // pos size pivot?
	void				drawLine(); // pos pos
	void				drawCircle(); // pos radius 
	void				drawQuad(); // pos size pivot?

	
}