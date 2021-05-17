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
	// check any situations in which we would not want to control the camera or send inputs to other listeners
	if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }

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
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		program.camera.currentCameraSpeed = program.camera.CAMERA_SPEED * 2.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		program.camera.currentCameraSpeed = program.camera.CAMERA_SPEED * 0.25f;
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
	if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case (GLFW_KEY_ESCAPE): // free mikey maus
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			program.windowManager.hasFocus = false;
			break;
			case (GLFW_KEY_BACKSPACE): // exit the program
			glfwSetWindowShouldClose(window, true);
			break;
		}
	}

	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case (GLFW_KEY_LEFT_SHIFT):
			program.camera.currentCameraSpeed = program.camera.CAMERA_SPEED;
			break;
			case (GLFW_KEY_LEFT_CONTROL):
			program.camera.currentCameraSpeed = program.camera.CAMERA_SPEED;
			break;
		}
	}
}

void Input::mouse_button_event(GLFWwindow* window, int key, int action, int mods)
{
	if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }

	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		program.windowManager.hasFocus = true;
	}
}

void Input::cursor_pos_event(GLFWwindow* window, double xPos, double yPos)
{
	if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }

	if (program.windowManager.hasFocus)
	{
		Input::mouseX = xPos;
		Input::mouseY = yPos;
		Input::firstMouseMovement = false;
	}
	// the window is focused and mouse movements should let us look around
	//~ if (hasFocus)
	//~ {
		//~ float xOffset = (xPos - lastX) * program->camera.cameraSensitivity;
		//~ float yOffset = (lastY - yPos) * program->camera.cameraSensitivity;
		//~ lastX = xPos;
		//~ lastY = yPos;

		//~ program->camera.yaw += xOffset;
		//~ program->camera.pitch += yOffset;
	//~ }
}

void Input::scroll_event(GLFWwindow* window, double xOffset, double yOffset)
{
	if (program.fileSystem.contextOpen || program.gui.guiFocused) { return; }

	program.camera.moveCamera(program.camera.movementFront * (float)(yOffset * program.camera.zoomSpeed * program.render.deltaTime));

	//~ camera.FOV = std::clamp(camera.FOV - (float)yOffset, 1.0f, 45.0f);
}
