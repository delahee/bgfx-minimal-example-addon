
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Input.hpp"

bm::Input* bm::Input::me = 0;

#define BX_UNUSED(e) 
void bm::Input::keyCb(GLFWwindow* _window, int32_t _key, int32_t _scancode, int32_t _action, int32_t _mods){
	if (Input::me->onKey)
		Input::me->onKey(_key, _scancode, _action, _mods);
}

void bm::Input::charCb(GLFWwindow* _window, uint32_t _scancode){
	if (!_scancode)
		return;
	if(Input::me->onChar)
		Input::me->onChar(_scancode);
}

void bm::Input::cursorPosCb(GLFWwindow* _window, double _mx, double _my){
	if(Input::me->onMousePos)
		Input::me->onMousePos((int32_t)_mx , (int32_t)_my );
}

void bm::Input::mouseButtonCb(GLFWwindow* _window, int32_t _button, int32_t _action, int32_t _mods){
	BX_UNUSED(_mods);
	double mx, my;
	if(Input::me->onMouseButton)
		Input::me->onMouseButton(mx,my,_button, _action, _mods);
}

bm::Input::Input(GLFWwindow * _win) : win(_win){
	me = this;
	glfwSetKeyCallback(win, keyCb);
	glfwSetCharCallback(win, charCb);
	glfwSetCursorPosCallback(win, cursorPosCb);
	glfwSetMouseButtonCallback(win, mouseButtonCb);
}
