#pragma once

#include "glad_glfw.h"
#include <glm/glm.hpp>
#include <editor.h>
#include "window.h"

class Input
{
	private:
		Tool ignoreRepeatLMB[2] = { SELECT, BOX };
		Tool ignoreRepeatRMB[2] = { SELECT, BOX };
		bool firstMouseMovement = true;
	public:
		glm::vec2 mousePos;
		glm::vec2 mouseMovement;
		bool ctrl_down = false, shift_down = false, alt_down = false, lmb_down = false, rmb_down = false, lmb_down_last = false, rmb_down_last = false;
		const float mouseRepeatDelay = 0.05f;

		void setup(Window& window);
		void mouse_button_repeat(GLFWwindow* window);
		void processInput(GLFWwindow* window);

        std::function<void(int, int, int, int)> keyCallback;
        std::function<void(int, int, int)> mouseButtonCallback;
        std::function<void(double, double)> scrollCallback;
        std::function<void(double, double)> cursorPosCallback;

		void key_event(int key, int scancode, int action, int mods);
		void mouse_button_event(int button, int action, int mods);
		void cursor_pos_event(double xPos, double yPos);
		void scroll_event(double xOffset, double yOffset);
};
