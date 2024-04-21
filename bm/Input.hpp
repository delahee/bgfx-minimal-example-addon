#pragma once 

#include <cctype>
#include <functional>

struct GLFWwindow;
namespace bm {

	//event callback are subpar, prefer device polling unless you have a way faster thread
	class Input {
	public:
		static Input*	me;
		GLFWwindow*		win = 0;

		std::function<void(int k, int s, int ac, int mods)>			onKey;
		std::function<void(unsigned int scancode)>					onChar;
		std::function<void(int x, int y)>							onMousePos;
		std::function<void(int x, int y, int bt, int ac, int mods)> onMouseButton;
	public:
		Input(GLFWwindow* win );
	protected:
		static void keyCb(GLFWwindow* _window, int32_t _key, int32_t _scancode, int32_t _action, int32_t _mods);
		static void charCb(GLFWwindow* _window, uint32_t _scancode);
		static void cursorPosCb(GLFWwindow* _window, double _mx, double _my);
		static void mouseButtonCb(GLFWwindow* _window, int32_t _button, int32_t _action, int32_t _mods);
	};
}


