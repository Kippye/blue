#include <main.h>
#include <editor_tile.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/* TODO LIST:
 * // BUGS //
 * [X] fix weird flipped texture selection
 * [ ] fix eggfault when minimizing using taskbar button
 * // UI //
 * [X] add io.WantCaptureMouse and io.WantCaptureKeyboard checks to ignore input when file dialogues are open
 * [X] load content folder textures as imagebuttons on a grid
 * [X] style ui
 * [X] information gui that shows stuff like the amount of tiles etc
 * [ ] toggles for popup menus
 * [ ] editor (tool selector, etc) panel
 * [ ] status gui that shows what process is currently done (mostly useful for stuff like reloading textures, opening or saving a BLF file, etc)
 * [ ] toggle visible grid (UI?)
 * [?] beautify file dialog
 * [?] set default file dialog sort mode to name ascending
 * // EDITOR //
 * [X] only listen to editor movement commands when no imgui window is focused
 * [X] basic drawing of tiles on a grid
 * [X] non-snapped drawing?
 * [ ] draw with selected texture (if none is selected, maybe copy invisible texture to selected content folder and use that)
 * [ ] delete tiles with right click
 * [ ] selection tools like area, by type, etc
 * [ ] pressing delete or backspace will delete every selected tile
 * [ ] move tool
 * [ ] toggle overlapping tiles (there will be a bounds check if it's true)
 * [ ] selecting tiles will show properties for that tile (for multiple selection you can mostly only set them all to the same one)
 * // OTHER //
 * [X] fix up editor tile class
 * [X] only recreate transforms array when a tile is modified (use some kind of event system)
 * [X] really just fix the fucking texture loading system
 * [ ] save user data / preferences:
 * --- could be divided up into parts like Gui prefs, editor prefs, etc that are then combined into one file DEBUG A LOT!
 * --- // Gui //
 * --- [ ] save file dialog sorting modes, maybe a base directory as well
 * [ ] more options when loading textures (format or smth)
 * [ ] proper input for repeating mouse button presses
 * [ ] add some way to convert from texture to atlas coordinates and reverse
 * [ ] some kind of timer system in render?
 * [?] create an event system ✌
*/

class Tile;
class Location;
class Visuals;

Program program;

Program::Program()
{
	// setup and initialize components
	windowManager.initialize();
	input.setup();
	render.setup();
	gui.guiInit(windowManager.window);

	// TEMP
	std::vector<std::string>& filesInContent = fileSystem.getInDir(fileSystem.contentDir.c_str());
	//program.gui.tileTextures = program.textureLoader.loadTextures(filesInContent);
	gui.tileTextures = textureLoader.loadTextures(filesInContent, fileSystem.contentDir);
	gui.textureAtlas = textureLoader.loadTextureAtlas(filesInContent, fileSystem.contentDir);
	render.textureAtlas = gui.textureAtlas;

	//render.textures[0] = textureLoader.loadTexture_ID("crate.png");
	gui.textureTest = textureLoader.loadTexture("logograss.png", fileSystem.contentDir);

	gui.window = &windowManager;

	std::vector<Tile>& _tiles = editor.tiles;
	unsigned int texture = textureLoader.loadTexture_ID("crate.png");

	//~ for (int x = 0; x < 4; x++)
	//~ {
		//~ for (int y = 3; y >= 0; y--)
		//~ {
			//~ editor.add_tile(_tiles.emplace_back(Location(glm::vec4(x, y, 0.0, 0.0)), Visuals(true, glm::vec2(x, y))));
		//~ }
	//~ }
	//~ for (int x = 0; x < 200000; x++)
	//~ {
		//~ _tiles.emplace_back(Location(glm::vec4(x, 0.0, 0.0, 0.0)), Visuals(true, glm::vec2(0, 0)));
	//~ }
	//~ render.add_to_render_list(_tiles);
}

void Program::loop()
{
	while (!glfwWindowShouldClose(windowManager.window))
	{
		input.processInput(windowManager.window);
		// update the camera view direction (not really needed but eh)
		camera.updateView();
		render.render();
		glfwPollEvents();
	}

	render.terminate();
	windowManager.terminate();
}

int main()
{
	program.loop();
	return 0;
}
