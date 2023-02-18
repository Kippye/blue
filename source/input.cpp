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

	if (program.windowManager.hasFocus)
		program.editor.updateToolPos(mousePos);
	if (program.gui.guiHovered)
		program.windowManager.setCursor(NORMAL);
	else
		program.windowManager.setCursor(DRAW);

	/// bools
	// modifiers, wait why do i even have these? oh right cus there aint no modifiers here...
	shift_down = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	ctrl_down = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
	alt_down = (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS);

	lmb_down_last = lmb_down;
	lmb_down = (!program.gui.guiHovered && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	rmb_down_last = rmb_down;
	rmb_down = (!program.gui.guiHovered && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	/// drawing
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// did the cursor or the camera move?
		if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 1.0f)
		{
			// has the repetition delay passed?
			if (program.render.mouse_repeat)
			{
				bool isToolIgnored = false;
				// not using a tool that shouldnt be repeated
				for (Tool tool : ignoreRepeatLMB)
				{
					if (tool == program.editor.getTool())
					{
						isToolIgnored = true;
					}
				}

				if (!isToolIgnored)
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
				bool isToolIgnored = false;
				// not using a tool that shouldnt be repeated
				for (Tool tool : ignoreRepeatRMB)
				{
					if (tool == program.editor.getTool())
					{
						isToolIgnored = true;
					}
				}

				if (!isToolIgnored)
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
	if (program.gui.guiWantKeyboard) { return; }

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
		if (program.file_system.contextOpen) { return; }
		switch (key)
		{
			/// select tools with keys
			case GLFW_KEY_1:
				if (program.gui.guiWantKeyboard) { return; }
				program.editor.setTool(SELECT);
			break;
			case GLFW_KEY_2:
				if (program.gui.guiWantKeyboard) { return; }
				program.editor.setTool(PLACE);
			break;
			case GLFW_KEY_3:
				if (program.gui.guiWantKeyboard) { return; }
				program.editor.setTool(BOX);
				break;
			// and so on...
			case GLFW_KEY_BACKSPACE: // delete
				if (program.gui.guiWantKeyboard) { return; }
				program.editor.delete_selection();
			break;
			case GLFW_KEY_DELETE: // delete
				if (program.gui.guiWantKeyboard) { return; }
				program.editor.delete_selection();
			break;
			/// program manipulators
			case GLFW_KEY_ESCAPE: // exit the program
				if (program.gui.guiFocused) { return; }
				glfwSetWindowShouldClose(window, true);
			break;
			/// shortcut keys that should work even when UI is focused
			case GLFW_KEY_N: 
				if (ctrl_down) // create a new empty file
				{
					// save changes first?
					if (program.editor.getDirtiness())
					{
						program.gui.popupToggles[SAVE_CONTEXT] = true;
					}
					else
					{
						program.file_system.startNewFile();
					}
				}
			break;
			case GLFW_KEY_I:
				if (ctrl_down) // pop up open dialog
				{
					program.gui.openFileDialog(OPEN);
				}
			break;
			case GLFW_KEY_O:
				if (ctrl_down) // save
				{
					if (shift_down) // save as
					{
						program.gui.openFileDialog(SAVE_AS);
					}
					program.gui.openFileDialog(SAVE);
				}
			break;
			case GLFW_KEY_G:
				// unnecessary safety check check
				if (program.editor.gridGizmoID != -1)
				{
					int index;
					Gizmo* gridGizmo = program.editor.ID_to_gizmo(program.editor.gridGizmoID, index);
					if (gridGizmo->visuals.Opacity > 0)
					{
						gridGizmo->visuals.Opacity = 0.0f;
					}
					else
					{
						// TODO: revert to value set in config instead?
						gridGizmo->visuals.Opacity = 0.75f;
					}
					program.editor.updateGizmoVisuals(index);
				}
			break;
			case GLFW_KEY_K:
				if (ctrl_down) // switch grid mode
				{
					program.gui.se.currentGridModeSelection = program.gui.se.currentGridModeSelection == 2 ? 0 : program.gui.se.currentGridModeSelection + 1;
					program.editor.setGridMode((GRID_MODE)program.gui.se.currentGridModeSelection);
				}
			break;
			case GLFW_KEY_L:
				if (ctrl_down) // switch overlap mode
				{
					program.editor.overlapMode = (int)program.editor.overlapMode == 2 ? OVERLAP_NEVER : (OVERLAP_MODE)((int)program.editor.overlapMode + 1);
				}
			break;
		}
	}

	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case (GLFW_KEY_LEFT_SHIFT):
			shift_down = false;
			break;
			case (GLFW_KEY_LEFT_CONTROL):
			ctrl_down = false;
			break;
			case (GLFW_KEY_LEFT_ALT):
			alt_down = false;
			break;
			case (GLFW_MOUSE_BUTTON_LEFT):
			lmb_down = false;
			break;
			case (GLFW_MOUSE_BUTTON_RIGHT):
			rmb_down = false;
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
	if (program.file_system.contextOpen || program.gui.guiHovered) { return; }

	program.camera.moveCamera(program.camera.movementFront * (float)(yOffset * program.camera.zoomSpeed * program.render.deltaTime));
}
