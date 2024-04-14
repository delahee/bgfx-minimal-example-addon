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

//MVP
void bm::makeMVP() { //keep it to identity as we don't have a camera system
	//bgfx::setTransform model

	float* arr = proj.mat.val;
	//bx::mtxOrtho(arr, 0, 1280, 720, 0, 0, 1, 0, false);
	//bgfx::setViewTransform(0, &view.mat, &proj.mat);
	//bgfx::setProjection proj
}

void bm::makeRenderStates()
{
	// uint64_t  _state = 0
	//| BGFX_STATE_RGB_WRITE
	//	| BGFX_STATE_ALPHA_WRITE
	//	| BGFX_STATE_DEPTH_WRITE
	//	| BGFX_STATE_DEPTH_TEST_LESS
	//	| BGFX_STATE_MSAA
	//	;
}

void bm::drawTri()
{
	//set vb
	//set texture
	//shader submit
}

void bm::drawQuad()
{
}

//DRAW QUAD
