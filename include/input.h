#pragma once

#include <glm/glm.hpp>

class Input
{
	private:
		bool firstMouseMovement = true;
	public:
		glm::vec2 mousePos;
		glm::vec2 mouseMovement;
		bool ctrl_down = false, shift_down = false, alt_down = false;

		void setup();
		void mouse_button_repeat(GLFWwindow* window);
		void processInput(GLFWwindow* window);

		void key_event(GLFWwindow* window, int key, int scancode, int action, int mods);
		void mouse_button_event(GLFWwindow* window, int button, int action, int mods);
		void cursor_pos_event(GLFWwindow* window, double xPos, double yPos);
		void scroll_event(GLFWwindow* window, double xOffset, double yOffset);
};
