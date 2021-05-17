#include <main.h>
#include <editor_tile.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/* TODO LIST:
 * // BUGS //
 * [ ] fix eggfault when minimizing using taskbar button
 * // UI //
 * [X] add io.WantCaptureMouse and io.WantCaptureKeyboard checks to ignore input when file dialogues are open
 * [X] load content folder textures as imagebuttons on a grid
 * [X] style ui
 * [X] information gui that shows stuff like the amount of tiles etc
 * [ ] void getGuiEvents
 * [ ] editor (tool selector, etc) panel
 * [ ] status gui that shows what process is currently done (mostly useful for stuff like reloading textures, opening or saving a BLF file, etc)
 * [?] beautify file dialog
 * [?] set default file dialog sort mode to name ascending
 * // EDITOR //
 * [X] only listen to editor movement commands when no imgui window is focused
 * [ ] basic drawing of tiles on a grid
 * [ ] draw with selected texture (if none is selected, maybe copy invisible texture to selected content folder and use that)
 * [ ] delete tiles with right click
 * [ ] selection tools like area, by type, etc
 * [ ] pressing delete or backspace will delete every selected tile
 * [ ] move tool
 * [ ] selecting tiles will show properties for that tile (for multiple selection you can mostly only set them all to the same one)
 * [ ] non-snapped drawing?
 * // OTHER //
 * [ ] Save user data / preferences:
 * --- could be divided up into parts like Gui prefs, editor prefs, etc that are then combined into one file DEBUG A LOT!
 * --- // Gui //
 * --- [ ] save file dialog sorting modes, maybe a base directory as well
 * [ ] more options when loading textures (format or smth)
 * [ ] only recreate transforms array when a tile is modified (use some kind of event system)
 * [ ] really just fix the fucking texture loading system
 * [ ] fix up editor tile class
 * [ ] add some way to convert from texture to atlas coordinates and reverse
*/

class Editor_Tile;
class Transforms;
class Visuals;

Program program;

Program::Program()
{
	// setup and initialize components
	windowManager.initialize();
	render.setup();
	input.setup();
	gui.guiInit(windowManager.window);

	//render.textures[0] = textureLoader.loadTexture_ID("crate.png");
	gui.textureTest = textureLoader.loadTexture("logograss.png", fileSystem.contentDir);
	render.textures[0] = gui.textureTest->ID;

	gui.window = &windowManager;

	std::vector<Editor_Tile>& tiles = levelView.currentTiles;
	unsigned int texture = textureLoader.loadTexture_ID("crate.png");

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			tiles.emplace_back(Transforms(glm::vec3(x, y, 0.0)), Visuals(true, glm::vec2(0.0f)));
		}
	}
}

void Program::loop()
{
	while (!glfwWindowShouldClose(windowManager.window))
	{
		gui.addEditorGui();
		input.processInput(windowManager.window);
		render.render();
		glfwPollEvents();
	}

	program.render.terminate();
	windowManager.terminate();
}

int main()
{
	program.loop();
	return 0;
}
