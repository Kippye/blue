#include <GLFW/glfw3.h>
#include <input.h>
#include <main.h>
#include <iostream>
#include <glm/gtx/norm.hpp>

class Program;
extern Program program;

void Input::setup(){ /* nothing to see here */ }

void Input::processInput(GLFWwindow* window)
{
	if (program.file_system.contextOpen) { return; }
	/// modifiers, wait why do i even have these? oh right cus there aint no modifiers here...
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		if (program.gui.guiHovered) { return; }
		shift_down = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if (program.gui.guiHovered) { return; }
		ctrl_down = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		if (program.gui.guiHovered) { return; }
		alt_down = true;
	}

	/// drawing
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// did the cursor or the camera move?
		if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 1.0f)
		{
			// has the repetition delay passed?
			if (program.render.mouse_repeat)
			{
				program.editor.tool_use();
				program.render.mouse_button_delay = 0.0f;
			}
			// if the cursor stopped for a while and then moved again, restart the delay
			else if (program.render.mouse_button_delay > 100.0f)
			{
				program.render.mouse_button_delay = 0.0f;
			}
		}
	}
	/// drag deleting
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// did the cursor or the camera move?
		if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 1.0f)
		{
			// has the repetition delay passed?
			if (program.render.mouse_repeat)
			{
				program.editor.tool_use_secondary();
				program.render.mouse_button_delay = 0.0f;
			}
			// if the cursor stopped for a while and then moved again, restart the delay
			else if (program.render.mouse_button_delay > 100.0f)
			{
				program.render.mouse_button_delay = 0.0f;
			}
		}
	}

	/// check any situations in which we would not want to control the camera or send inputs to other listeners
	if (program.gui.guiFocused) { return; }

	// TODO: make arrow keys move selected tiles instead
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // forwards
	{
		program.camera.moveCamera(program.camera.worldUp * program.render.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // backwards
	{
		program.camera.moveCamera(-program.camera.worldUp * program.render.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // strafe left
	{
		program.camera.moveCamera(-program.camera.cameraRight * program.render.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // strafe right
	{
		program.camera.moveCamera(program.camera.cameraRight * program.render.deltaTime);
	}
}

void Input::key_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			/// select tools with keys
			case GLFW_KEY_1:
				program.editor.setTool(SELECT);
			break;
			case GLFW_KEY_2:
				program.editor.setTool(PLACE);
			break;
			// and so on...
			/// program manipulators
			case GLFW_KEY_ESCAPE: // free mikey maus
				if (program.file_system.contextOpen || program.gui.guiFocused) { return; }
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				program.windowManager.hasFocus = false;
			break;
			case GLFW_KEY_BACKSPACE: // exit the program
				if (program.file_system.contextOpen || program.gui.guiFocused) { return; }
				glfwSetWindowShouldClose(window, true);
			break;
			/// shortcut keys that should work even when UI is focused
			case GLFW_KEY_I:
				if (program.file_system.contextOpen) { return; }
				if (ctrl_down) // pop up open dialog
				{
					program.gui.openFileDialog(OPEN);
				}
			break;
			case GLFW_KEY_O:
				if (program.file_system.contextOpen) { return; }
				if (ctrl_down) // save
				{
					if (shift_down) // save as
					{
						program.gui.openFileDialog(SAVE_AS);
					}
					program.gui.openFileDialog(SAVE);
				}
			break;
		}
	}

	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case (GLFW_KEY_LEFT_SHIFT):
			//if (program.file_system.contextOpen || program.gui.guiHovered) { return; }
			shift_down = false;
			break;
			case (GLFW_KEY_LEFT_CONTROL):
			//if (program.file_system.contextOpen || program.gui.guiHovered) { return; }
			ctrl_down = false;
			break;
			case (GLFW_KEY_LEFT_ALT):
			//if (program.file_system.contextOpen || program.gui.guiHovered) { return; }
			alt_down = false;
			break;
		}
	}
}

void Input::mouse_button_event(GLFWwindow* window, int key, int action, int mods)
{
	if (program.file_system.contextOpen || program.gui.guiHovered) { return; }

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			program.windowManager.hasFocus = true;
			program.render.mouse_button_delay = 0.0f;
			program.editor.tool_use();
		}
		else if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			program.windowManager.hasFocus = true;
			program.render.mouse_button_delay = 0.0f;
			program.editor.tool_use_secondary();
		}
	}
}

void Input::cursor_pos_event(GLFWwindow* window, double xPos, double yPos)
{
	if (program.windowManager.hasFocus)
	{
		mouseMovement.x = xPos - mousePos.x;
		mouseMovement.y = yPos - mousePos.y;
		mousePos.x = xPos;
		mousePos.y = yPos;
		firstMouseMovement = false;
		program.editor.updateToolPos(mousePos);
	}
}

void Input::scroll_event(GLFWwindow* window, double xOffset, double yOffset)
{
	if (program.file_system.contextOpen || program.gui.guiFocused) { return; }

	program.camera.moveCamera(program.camera.movementFront * (float)(yOffset * program.camera.zoomSpeed * program.render.deltaTime));
}
