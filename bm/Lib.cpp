#include <cstdio>
#include <cctype>
#include <vector>

#include <bx/bx.h>
#include <bgfx/bgfx.h>

#include "Lib.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <unordered_map>
#include <string>

void bm::plop(){
	printf("toto");
}

static std::unordered_map<std::string, Tex>  texCache;

Tex bm::getPng(const char * texPath) {
	if (texCache.find(texPath) != texCache.end())
		return texCache[texPath];

	FILE* c = 0;
	fopen_s(&c,texPath, "rb");
	int w = 0;
	int h = 0;
	int chans = 0;
	int desired = 4;
	if (!c) {
		printf("no tex content");
		return { {}, {0,0} };
	}
	uint8_t* bytes = stbi_load_from_file(c, &w, &h, &chans, desired);
	uint32_t sz = w * h * desired;
	fclose(c);

	const bgfx::Memory * mem = bgfx::alloc(sz);
	memcpy(mem->data, bytes,sz);
	auto hdl = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_POINT, mem);
	free(bytes);
	Tex t = texCache[texPath] = { hdl, Vec2( w,h ) };
	return t;
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
static bool submitAndDiscard = true;
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

	bx::Vec3 pos = { 150,150,0 };
	float w = 500.0f;
	float depth = 0.01f;//because default render state is set to zless
	
	v0.setPos({ pos.x + 0,			pos.y + 0.5f * w, depth });
	v1.setPos({ pos.x + 0,			pos.y + 0, depth });
	v2.setPos({ pos.x + 0.5f * w,	pos.y + 0, depth });

	v0.setUV({0.f, 2.0f});
	v1.setUV({0.0f,0.0f});
	v2.setUV({2.0f,0.0f});

	v0.setCol( bm::white );
	v1.setCol( bm::magenta );
	v2.setCol( bm::white );
	
	bgfx::setVertexBuffer(0, &tvb, 0, maxVertices);
	submit();
}

void bm::submit() {
	if(submitAndDiscard)
		bgfx::submit(0, shdr);
	else 
		bgfx::submit(0, shdr,0, BGFX_DISCARD_NONE);
}

void bm::drawCircle(Vec2 a, float radius, float th, int nbSegments, Vec4 col){
	float pi = 3.14159;
	if (nbSegments <= 0)	nbSegments = std::ceil(radius * pi * 2 / 2);
	if (nbSegments < 3)		nbSegments = 3;

	submitAndDiscard = false;
	const float angle = pi * 2 / nbSegments;
	for (int i = 0; i < nbSegments; i++) {
		auto a0 = i * angle;
		auto a1 = (i + 1) * angle;
		auto a0x = a.x + cosf(a0) * radius;
		auto a0y = a.y + sinf(a0) * radius;
		auto a1x = a.x + cosf(a1) * radius;
		auto a1y = a.y + sinf(a1) * radius;
		drawLine({ a0x, a0y }, { a1x, a1y }, th,col);
	}
	submitAndDiscard = true;
	bgfx::submit(0, shdr);
}

void bm::drawCircle(Vec2 a, float radius, float thicc, int nbSegments){
	drawCircle(a, radius, thicc, nbSegments, bm::white);
}

void bm::drawLine(Vec2 a, Vec2 b, float th ) {
	drawLine(a, b, th, Vec4(1,1,1,1));
}

void bm::drawLine(Vec2 a, Vec2 b, float th,Vec4 col){
	bgfx::TransientVertexBuffer tvb;
	int maxVertices = 6;
	bgfx::allocTransientVertexBuffer(&tvb, maxVertices, PosUVColVertex::vtx_layout);
	PosUVColVertex* vtxData = (PosUVColVertex*)tvb.data;
	memset(vtxData, 0, maxVertices * sizeof(PosUVColVertex));
	PosUVColVertex& v0 = *(vtxData + 0);
	PosUVColVertex& v1 = *(vtxData + 1);
	PosUVColVertex& v2 = *(vtxData + 2);
	PosUVColVertex& v3 = *(vtxData + 3);
	PosUVColVertex& v4 = *(vtxData + 4);
	PosUVColVertex& v5 = *(vtxData + 5);
	float depth = 0.01f;//because default render state is set to zless
	float pi = 3.14159;
	float angle = atan2(b.y - a.y, b.x - a.x);
	float xd = cosf(angle);
	float yd = sinf(angle);

	float tX = cosf(angle - pi * 0.5f) * th * 0.5f;
	float tY = sinf(angle - pi * 0.5f) * th * 0.5f;

	float bX = cosf(angle + pi * 0.5f) * th * 0.5f;
	float bY = sinf(angle + pi * 0.5f) * th * 0.5f;

	v0.setPos(bx::Vec3( a.x + bX,	a.y + bY,	depth)); v0.setUV({ 0.f, 1.0f });	
	v1.setPos(bx::Vec3( a.x + tX,	a.y + tY,	depth)); v1.setUV({ 0.0f,0.0f });	
	v2.setPos(bx::Vec3( b.x + tX,	b.y + tY,	depth)); v2.setUV({ 1.0f,0.0f });	

	v3.setPos(bx::Vec3(b.x + tX, b.y + tY, depth)); v3.setUV({ 1.0f,0.0f });		
	v4.setPos(bx::Vec3(b.x + bX, b.y + bY, depth));	v4.setUV({ 1.0f,1.0f });		
	v5.setPos(bx::Vec3(a.x + bX, a.y + bY, depth)); v5.setUV({ 0.f, 1.0f });		

	if (false) {
		v0.setCol({ 1,0,0,1 }); //bl	
		v1.setCol({ 0,1,0,1 }); //tl
		v2.setCol({ 0,0,1,1 }); //tr

		v3.setCol({ 0,0,1,1 }); //tr
		v4.setCol({ 1,0,1,1 }); //br	
		v5.setCol({ 1,0,0,1 }); //bl	
	}
	else {
		std::vector<PosUVColVertex*>vec = { &v0,&v1,&v2,&v3,&v4,&v5 };
		for (auto& v : vec)
			v->setCol(col);
	}

	bgfx::setVertexBuffer(0, &tvb, 0, maxVertices);
	submit();
}


void bm::drawQuad(Vec2 pos, Vec2 sz){
	drawQuad(pos, sz, Vec4(1, 1, 1, 1));
}

void bm::drawQuad(Vec2 pos, Vec2 sz, Vec4 col, bool flippedX){
	bgfx::TransientVertexBuffer tvb;
	int maxVertices = 6;
	bgfx::allocTransientVertexBuffer(&tvb, maxVertices, PosUVColVertex::vtx_layout);
	PosUVColVertex* vtxData = (PosUVColVertex*)tvb.data;
	memset(vtxData, 0, maxVertices * sizeof(PosUVColVertex));
	PosUVColVertex& v0 = *(vtxData + 0);
	PosUVColVertex& v1 = *(vtxData + 1);
	PosUVColVertex& v2 = *(vtxData + 2);
	PosUVColVertex& v3 = *(vtxData + 3);
	PosUVColVertex& v4 = *(vtxData + 4);
	PosUVColVertex& v5 = *(vtxData + 5);
	float w = sz.x;
	float h = sz.y;
	float depth = 0.01f;//because default render state is set to zless

	v0.setPos(bx::Vec3(pos.x + 0,	pos.y + h,	depth)); v0.setUV({ 0.f,	1.0f });
	v1.setPos(bx::Vec3(pos.x + 0,	pos.y + 0,	depth)); v1.setUV({ 0.0f,	0.0f });
	v2.setPos(bx::Vec3(pos.x + w,	pos.y + 0,	depth)); v2.setUV({ 1.0f,	0.0f });
	v3.setPos(bx::Vec3(pos.x + w,	pos.y + 0,	depth)); v3.setUV({ 1.f,	0.0f });
	v4.setPos(bx::Vec3(pos.x + w,	pos.y + h,	depth)); v4.setUV({ 1.0f,	1.0f });
	v5.setPos(bx::Vec3(pos.x + 0,	pos.y + h,	depth)); v5.setUV({ 0.0f,	1.0f });

	std::initializer_list<PosUVColVertex*>vec = { &v0,&v1,&v2,&v3,&v4,&v5 };
	for (auto& v : vec) {
		v->setCol(col);
		if (flippedX) {
			Vec2 uv{ v->getUV() };
			v->setUV({1.0f - uv.x, uv.y});
		}
	}

	bgfx::setVertexBuffer(0, &tvb, 0, maxVertices);
	bgfx::submit(0, shdr);
}

void bm::drawQuad(Vec2 pos, Vec2 sz, Vec4 col){
	bm::drawQuad(pos, sz, col, false);
}

#include <chrono>

double bm::stamp(){
	std::chrono::nanoseconds ns =
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
	return ns.count() / 1000000000.0;
}

Tex::Tex(bgfx::TextureHandle _hdl, Vec2 _size) : hdl(_hdl), size(_size){
}

void trace(const char* str){
	printf("%s\n", str);
}
