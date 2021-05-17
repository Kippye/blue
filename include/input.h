#pragma once

class Input
{
	public:
		float mouseX, mouseY;
		bool firstMouseMovement = true;

		void setup();
		void processInput(GLFWwindow* window);

		void key_event(GLFWwindow* window, int key, int scancode, int action, int mods);
		void mouse_button_event(GLFWwindow* window, int button, int action, int mods);
		void cursor_pos_event(GLFWwindow* window, double xPos, double yPos);
		void scroll_event(GLFWwindow* window, double xOffset, double yOffset);
};
