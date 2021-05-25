#include <GLFW/glfw3.h>

#include <main.h>
class Program;
extern Program program;
#include <input.h>

#include <iostream>

void Input::setup()
{

}

void Input::processInput(GLFWwindow* window)
{
	if (program.fileSystem.contextOpen) { return; }
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
	// check any situations in which we would not want to control the camera or send inputs to other listeners
	if (program.gui.guiFocused) { return; }

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forwards
	{
		program.camera.moveCamera(program.camera.worldUp * program.render.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backwards
	{
		program.camera.moveCamera(-program.camera.worldUp * program.render.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // strafe left
	{
		program.camera.moveCamera(-program.camera.cameraRight * program.render.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // strafe right
	{
		program.camera.moveCamera(program.camera.cameraRight * program.render.deltaTime);
	}
	//~ if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) // jump
	//~ {
		//~ camera.moveCamera(camera.worldUp * deltaTime);
	//~ }
	//~ if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // depress
	//~ {
		//~ camera.moveCamera(-camera.worldUp * deltaTime);
	//~ }
}

void Input::key_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case (GLFW_KEY_ESCAPE): // free mikey maus
			if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			program.windowManager.hasFocus = false;
			break;
			case (GLFW_KEY_BACKSPACE): // exit the program
			if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }
			glfwSetWindowShouldClose(window, true);
			break;
		}
	}

	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case (GLFW_KEY_LEFT_SHIFT):
			if (program.fileSystem.contextOpen || program.gui.guiHovered) { return; }
			shift_down = false;
			break;
			case (GLFW_KEY_LEFT_CONTROL):
			if (program.fileSystem.contextOpen || program.gui.guiHovered) { return; }
			ctrl_down = false;
			break;
			case (GLFW_KEY_LEFT_ALT):
			if (program.fileSystem.contextOpen || program.gui.guiHovered) { return; }
			alt_down = false;
			break;
		}
	}
}

void Input::mouse_button_event(GLFWwindow* window, int key, int action, int mods)
{
	if (program.fileSystem.contextOpen || program.gui.guiHovered) { return; }

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			program.windowManager.hasFocus = true;
			program.editor.tool_use();
		}
		else if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			program.editor.tool_use_secondary();
		}
	}
}

void Input::cursor_pos_event(GLFWwindow* window, double xPos, double yPos)
{
	if (program.windowManager.hasFocus)
	{
		mousePos.x = xPos;
		mousePos.y = yPos;
		firstMouseMovement = false;
		program.editor.updateToolPos(mousePos);
	}
}

void Input::scroll_event(GLFWwindow* window, double xOffset, double yOffset)
{
	if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }

	program.camera.moveCamera(program.camera.movementFront * (float)(yOffset * program.camera.zoomSpeed * program.render.deltaTime));
}
