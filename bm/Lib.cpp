#include <cstdio>
#include <cctype>

#include "Lib.hpp"
#include "bgfx/bgfx.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bgfx::VertexLayout PosUVColVertex::vtx_layout;

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
