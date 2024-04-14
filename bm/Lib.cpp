#include <cstdio>
#include <cctype>

#include <bx/bx.h>
#include <bgfx/bgfx.h>


#include "Lib.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void bm::plop(){
	printf("toto");
}

bgfx::TextureHandle bm::getPng(const char * texPath) {
	FILE* c = fopen(texPath, "rb");
	int w = 0;
	int h = 0;
	int chans = 0;
	int desired = 4;
	if (!c)
		return {};
	uint8_t* bytes = stbi_load_from_file(c, &w, &h, &chans, desired);
	uint32_t sz = w * h * chans;
	fclose(c);

	const bgfx::Memory * mem = bgfx::alloc(sz);
	memcpy(mem->data, bytes,sz);
	auto hdl = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_POINT, mem);
	free(bytes);
	return hdl;
}

///SHADER SECTION


bgfx::VertexLayout PosUVColVertex::vtx_layout;

#include <bgfx/embedded_shader.h>
#include "shdr/vs_sprite.bin.h"
#include "shdr/fs_sprite.bin.h"

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_sprite),
	BGFX_EMBEDDED_SHADER(fs_sprite),

	BGFX_EMBEDDED_SHADER_END()
};

bgfx::ProgramHandle bm::getSpriteShader() {
	PosUVColVertex::init();

	bgfx::RendererType::Enum type = bgfx::getRendererType();

	bgfx::ShaderHandle vsh = bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_sprite");
	bgfx::ShaderHandle fsh = bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_sprite");

	// Create program from shaders.
	auto m_program = bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
	return m_program;
}

//initial view is black with alpha 0 and depth 0
void bm::clear(){
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff, 0.0f);
}

void bm::makeMVP( int bbW, int bbH ) { //keep it to identity as we don't have a camera system
	float* varr = view.mat.val;
	float* parr = proj.mat.val;
	bx::mtxOrtho(parr, 0, bbW, bbH, 0, 0, 1, 0, false);
	bgfx::setViewTransform(0, varr, parr);
}

void bm::makeRenderStates(){
	int state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_CULL_CW;//write colors
	state |= BGFX_STATE_DEPTH_TEST_GEQUAL | BGFX_STATE_MSAA | BGFX_STATE_WRITE_Z;//be aa  and z consistent
	bgfx::setState(state);
}


static bgfx::ProgramHandle shdr = {};
void bm::setShader(bgfx::ProgramHandle s) {
	shdr = s;
}

void bm::drawTri(){
	bgfx::TransientVertexBuffer tvb;
	int maxVertices = 3;
	bgfx::allocTransientVertexBuffer(&tvb, maxVertices, PosUVColVertex::vtx_layout);
	PosUVColVertex* vtxData = (PosUVColVertex*)tvb.data;
	memset(vtxData, 0, maxVertices * sizeof(PosUVColVertex));
	PosUVColVertex& v0 = *(vtxData + 0);
	PosUVColVertex& v1 = *(vtxData + 1);
	PosUVColVertex& v2 = *(vtxData + 2);
	float w = 200.0f;
	float depth = 0.01f;//because default render state is set to zless
	v0.setPos(bx::Vec3(0, 0.5 * w, depth));
	v2.setPos(bx::Vec3(0.5 * w, 0, depth));
	v1.setPos(bx::Vec3(0, 0, depth));

	v0.setCol( bm::white );
	v1.setCol( bm::magenta );
	v2.setCol( bm::white );
	bgfx::setVertexBuffer(0, &tvb, 0, maxVertices);
	bgfx::submit(0, shdr);
}

void bm::drawQuad()
{
}



//DRAW QUAD
