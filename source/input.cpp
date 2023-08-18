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

	if (program.windowManager.hasFocus && program.gui.tileTextures.size() > 0)
	{
		program.editor.updateToolPos(mousePos);
	}
	if (program.gui.guiHovered)
	{
		program.windowManager.setCursor(NORMAL);
	}
	else
	{
		program.windowManager.setCursor(DRAW);
	}

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
	if (program.gui.tileTextures.size() > 0)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			// did the cursor or the camera move?
			if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 0.0f)
			{
				// has the repetition delay passed?
				if (program.render.mouseRepeat)
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
					{
						program.editor.tool_use();
					}

					program.render.mouseButtonDelay = 0.0f;
				}
				// if the cursor stopped for a while and then moved again, restart the delay
				else if (program.render.mouseButtonDelay > 100.0f)
				{
					program.render.mouseButtonDelay = 0.0f;
				}
			}
		}
		/// drag deleting
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			// did the cursor or the camera move?
			if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 0.0f)
			{
				// has the repetition delay passed?
				if (program.render.mouseRepeat)
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

					program.render.mouseButtonDelay = 0.0f;
				}
				// if the cursor stopped for a while and then moved again, restart the delay
				else if (program.render.mouseButtonDelay > 100.0f)
				{
					program.render.mouseButtonDelay = 0.0f;
				}
			}
		}
	}

	/// check any situations in which we would not want to control the camera or send inputs to other listeners
	if (program.gui.guiWantKeyboard) { return; }

	glm::vec3 cameraPosBefore = program.camera.cameraPos;

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

	if (cameraPosBefore == program.camera.cameraPos)
	{
		program.camera.lastMovement = glm::vec3(0.0f);
	}

	// reset mouse movement as it only updates when the mouse is ACTUALLY moved
	mouseMovement = glm::vec2(0.0f);
}

void Input::key_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (program.file_system.contextOpen || program.gui.guiWantKeyboard) { return; }
		switch (key)
		{
			/// select tools with keys
			case GLFW_KEY_1:
				if (program.gui.guiWantKeyboard || program.gui.tileTextures.size() == 0) { return; }
				program.editor.setTool(SELECT);

				if (ctrl_down)
				{
					program.editor.select_all();
				}
			break;
			case GLFW_KEY_2:
				if (program.gui.tileTextures.size() == 0) { return; }
				program.editor.setTool(PLACE);
			break;
			case GLFW_KEY_3:
				if (program.gui.tileTextures.size() == 0) { return; }
				program.editor.setTool(BOX);
				break;
			// and so on...
			case GLFW_KEY_BACKSPACE: // delete
				program.editor.delete_selection();
			break;
			case GLFW_KEY_DELETE: // delete also
				program.editor.delete_selection();
			break;
			case GLFW_KEY_B: // push to back (DEPRECATED)
				if (ctrl_down)
				{
					program.editor.push_selection_to_back();
				}
			break;
			case GLFW_KEY_T:
				if (ctrl_down)
				{
					program.editor.select_by_texture(program.editor.nextTile.visuals.textureName);
				}
			break;
			case GLFW_KEY_Z: // undo
				if (ctrl_down)
				{
					program.editor.undo();
				}
			break;
			case GLFW_KEY_Y: // redo
				if (ctrl_down)
				{
					program.editor.redo();
				}
			break;
			case GLFW_KEY_X: // cut
				if (ctrl_down)
				{
					program.editor.cut_selection();
				}
			break;
			case GLFW_KEY_C: // copy
				if (ctrl_down)
				{
					program.editor.copy_selection();
				}
			break;
			case GLFW_KEY_V: // paste
				if (ctrl_down)
				{
					program.editor.paste();
				}
			break;
			/// program manipulators
			case GLFW_KEY_ESCAPE: // exit the program
				glfwSetWindowShouldClose(window, true);
			break;
			/// shortcut keys
			case GLFW_KEY_N: 
				if (ctrl_down) // create a new empty file
				{
					if (program.render.textureAtlas == nullptr)
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = true;
					}
					else
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = false;
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
				}
			break;
			case GLFW_KEY_I:
				if (ctrl_down) // pop up open dialog
				{
					if (program.render.textureAtlas == nullptr)
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = true;
					}
					else
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = false;
						program.gui.openFileDialog(OPEN);
					}
				}
			break;
			case GLFW_KEY_O:
				if (ctrl_down) // save
				{
					if (program.render.textureAtlas == nullptr)
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = true;
					}
					else
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = false;
						if (shift_down) // save as
						{
							program.gui.openFileDialog(SAVE_AS);
						}
						program.gui.openFileDialog(SAVE);
					}
				}
			break;
			case GLFW_KEY_R:
				if (ctrl_down) // reload textures
				{
					if (program.render.textureAtlas == nullptr)
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = true;
					}
					else
					{
						program.gui.popupToggles[CONTENT_LACK_WARNING] = false;
						program.file_system.updateTextures();
					}
				}
			case GLFW_KEY_G:
				program.editor.set_grid_visible(!program.editor.gridVisible);
			break;
			case GLFW_KEY_K:
				if (ctrl_down) // switch grid mode
				{
					program.editor.setAutosnap(!program.editor.getAutosnap());
				}
			break;
			case GLFW_KEY_L:
				if (ctrl_down) // switch overlap mode
				{
					program.editor.overlap = !program.editor.overlap;
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
		}
	}
}

void Input::mouse_button_event(GLFWwindow* window, int key, int action, int mods)
{
	if (program.file_system.contextOpen || program.gui.guiHovered || program.gui.tileTextures.size() == 0) { return; }

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			program.windowManager.hasFocus = true;
			program.render.mouseButtonDelay = 0.0f;
			program.editor.tool_use();
		}
		else if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			program.windowManager.hasFocus = true;
			program.render.mouseButtonDelay = 0.0f;
			program.editor.tool_use_secondary();
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case (GLFW_MOUSE_BUTTON_LEFT):
			lmb_down = false;
			break;
			case (GLFW_MOUSE_BUTTON_RIGHT):
			rmb_down = false;
			break;
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
		if (program.gui.tileTextures.size() > 0)
		{
			program.editor.updateToolPos(mousePos);
		}
	}
}

void Input::scroll_event(GLFWwindow* window, double xOffset, double yOffset)
{
	if (program.file_system.contextOpen || program.gui.guiHovered) { return; }

	program.camera.moveCamera(program.camera.movementFront * (float)(yOffset * program.camera.zoomSpeed * program.render.deltaTime));
}
