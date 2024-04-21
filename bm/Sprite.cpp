#include "Lib.hpp"
#include "Sprite.hpp"

using namespace bm;

void bm::Sprite::draw(){
	bm::setShader(mat.shdr);
	bm::makeRenderStates();
	bgfx::setTexture(0, mat.sampler, mat.tex);
	bm::drawQuad( pos, size);
}

void bm::SpriteBatch::draw() {
	int nb = positions.size();
	bgfx::TransientVertexBuffer tvb;
	int maxVertices = 6 * nb;
	bgfx::allocTransientVertexBuffer(&tvb, maxVertices, PosUVColVertex::vtx_layout);
	PosUVColVertex* vtxData = (PosUVColVertex*)tvb.data;
	memset(vtxData, 0, maxVertices * sizeof(PosUVColVertex));

	int ofs = 0;
	for (int i = 0; i < nb; ++i) {
		auto& pos = positions[i];
		auto& col = colors[i];
		auto& sz = size[i];
		PosUVColVertex& v0 = *(vtxData + 0 + ofs);
		PosUVColVertex& v1 = *(vtxData + 1 + ofs);
		PosUVColVertex& v2 = *(vtxData + 2 + ofs);
		PosUVColVertex& v3 = *(vtxData + 3 + ofs);
		PosUVColVertex& v4 = *(vtxData + 4 + ofs);
		PosUVColVertex& v5 = *(vtxData + 5 + ofs);
		float w = sz.x;
		float h = sz.y;
		float depth = 0.01f;//because default render state is set to zless

		v0.setPos(bx::Vec3(pos.x + 0, pos.y + h, depth));	v0.setUV({ 0.f,		1.0f });
		v1.setPos(bx::Vec3(pos.x + 0, pos.y + 0, depth));	v1.setUV({ 0.0f,	0.0f });
		v2.setPos(bx::Vec3(pos.x + w, pos.y + 0, depth));	v2.setUV({ 1.0f,	0.0f });

		v3.setPos(bx::Vec3(pos.x + w, pos.y + 0, depth));	v3.setUV({ 1.f,		0.0f });
		v4.setPos(bx::Vec3(pos.x + w, pos.y + h, depth));	v4.setUV({ 1.0f,	1.0f });
		v5.setPos(bx::Vec3(pos.x + 0, pos.y + h, depth));	v5.setUV({ 0.0f,	1.0f });

		std::vector<PosUVColVertex*>vec = { &v0,&v1,&v2,&v3,&v4,&v5 };
		for (auto& v : vec)
			v->setCol(col);

		ofs += PosUVColVertex::vtx_layout.getStride();
	}

	bm::setShader(mat.shdr);
	bm::makeRenderStates();
	bgfx::setTexture(0, mat.sampler, mat.tex);
	bgfx::setVertexBuffer(0, &tvb, 0, maxVertices);
	bgfx::submit(0, mat.shdr);
}
