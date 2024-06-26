/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <cstdio>
#include <string>
#include <iostream>
#include <filesystem>

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>
#include "logo.h"
#include "bm/Lib.hpp"
#include "bm/Sprite.hpp"
#include "bm/Game.hpp"
#include "bm/Input.hpp"

#include "bmg/Game.hpp"
#include "helloworld-game.h"

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
		s_showStats = !s_showStats;
}

int main(int argc, char **argv)
{
	std::cout << std::filesystem::current_path() << std::endl;
	// Create a GLFW window without an OpenGL context.
	glfwSetErrorCallback(glfw_errorCallback);
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(1280, 720, "helloworld-game", nullptr, nullptr);
	if (!window)
		return 1;
	glfwSetKeyCallback(window, glfw_keyCallback);
	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(window);
#endif
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	init.resolution.width = (uint32_t)width;
	init.resolution.height = (uint32_t)height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	if (!bgfx::init(init))
		return 1;
	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
	bm::plop();

	auto texPixel = bm::getPng("bm/res/pixel.png");
	auto texPhi = bm::getPng("bm/res/phi_angry.png");
	auto texGround = bm::getPng("bm/res/ground.png");
	auto texRed = bm::getPng("bm/res/red.png");
	auto shdr = bm::getSpriteShader();
	auto texSampler = bgfx::createUniform("tex0", bgfx::UniformType::Sampler, 1);

	bm::Input* input = new bm::Input(window);
	auto now = bm::stamp();
	auto dt = 0.15f;
	while (!glfwWindowShouldClose(window)) {
		auto ntime = bm::stamp();
		dt = ntime - now + 0.000001f;
		now = ntime;
		glfwPollEvents();
		// Handle window resize.
		int oldWidth = width, oldHeight = height;
		glfwGetWindowSize(window, &width, &height);
		if (width != oldWidth || height != oldHeight) {
			bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
			bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		}
		bm::clear();

		//dummy ping
		bgfx::touch(kClearView);

		if (false) {
			bgfx::dbgTextClear();
			bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
			//x y are in cell (8x8) units...
			bgfx::dbgTextPrintf(1, 1, 0x0f, "Press F1 to toggle stats.");
		}

		bm::makeMVP(width, height);

		auto game = bmg::Game::get();

		/*
		input->onKey = [=](int key, int scan, int ac, int mods) {
			bool isDown = ac == GLFW_PRESS || ac == GLFW_REPEAT;

			if (isDown && (key == GLFW_KEY_LEFT)) 
				game->red.pos.x -= 5;
			if (isDown && (key == GLFW_KEY_RIGHT))
				game->red.pos.x += 5;
			if (isDown && (key == GLFW_KEY_UP))
				game->red.pos.y -= 5;
			if (isDown && (key == GLFW_KEY_DOWN))
				game->red.pos.y += 5;
		};
		*/
		game->draw();
		game->input(window,dt);
		game->update(dt);
		
		bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();
		
	}
	bgfx::shutdown();
	glfwTerminate();
	return 0;
}
