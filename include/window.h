#pragma once

extern "C" {
#include <glad.h>
}
#include <GLFW/glfw3.h>

#include <functional>

class Window
{
	public:
		GLFWwindow* window;
		float SCREEN_WIDTH = 800.0f;
		float SCREEN_HEIGHT = 600.0f;
		float lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
		bool firstMouseMovement = true;
		bool hasFocus = true;
		bool shouldClose = false;

	void initialize();
	void terminate();
	// window events
	std::function<void(Window*, int, int)> framebuffer_size_callback;
	std::function<void(Window*, int)> window_focus_callback;
	std::function<void(Window*)> window_close_callback;
	// input events
	std::function<void(Window*, int, int, int, int)> key_callback;
	std::function<void(Window*, int, int, int)> mouse_button_callback;
	std::function<void(Window*, double, double)> cursor_pos_callback;
	std::function<void(Window*, double, double)> scroll_callback;
};
