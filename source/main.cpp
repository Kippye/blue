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
 * [X] toggles for popup menus
 * [X] editor (tool selector, etc) panel
 * [ ] make fitting window paddings per window
 * [ ] some kind of dropdown for texture selection in the properties panel
 * [ ] toggle visible grid (UI?) (bottom bar?)
 * [?] status gui that shows what process is currently done (mostly useful for stuff like reloading textures, opening or saving a BLF file, etc)
 * [?] beautify file dialog
 * [?] set default file dialog sort mode to name ascending
 * // EDITOR //
 * [X] only listen to editor movement commands when no imgui window is focused
 * [X] basic drawing of tiles on a grid
 * [X] non-snapped drawing?
 * [X] draw with selected texture (if none is selected, maybe copy invisible texture to selected content folder and use that)
 * [X] delete tiles with right click
 * [X] select tools with number keys
 * [ ] undo, redo, if at all doable
 * [ ] additional properties on tiles (collisions, staticity, size etc, etc, etc)
 * [ ] selection tools like area, by type, etc
 * [ ] pressing delete or backspace will delete every selected tile
 * [ ] move tool (with arrow keys & move camera with it)
 * [ ] resize tool ‍GRRRRR
 * [ ] toggle overlapping tiles (there will be a bounds check if it's true) (toggle in bottom bar)
 * [ ] selecting tiles will show properties for that tile (for multiple selection you can mostly only set them all to the same one)
 * // OTHER //
 * [X] fix up editor tile class
 * [X] only recreate transforms array when a tile is modified (use some kind of event system)
 * [X] really just fix the fucking texture loading system
 * [X] proper input for repeating mouse button presses
 * [X] save user data / preferences:
 * --- could be divided up into parts like Gui prefs, editor prefs, etc that are then combined into one file DEBUG A LOT!
 * --- // Gui //
 * --- [X] save file dialog sorting modes, maybe a base directory as well
 * [ ] more options when loading textures (format or smth)
 * [?] some kind of timer system in render?
 * [?] add some way to convert from texture to atlas coordinates and reverse
 * [?] create an event system ✌
 *
 */
/* TODO BEFORE RELEASE 1.0:
 * [X] save configs (mostly content folder)
 * [X] convert tiles to and from blf files
 * [X] save / open blf files
 */

class E_Tile;
class Location;
class Visuals;

Program program;

Program::Program()
{
	// setup and initialize components
	file_system.tryLoadConfigs();

	// TODO: load file that was opened last time blf_converter.load_file
	windowManager.initialize();
	input.setup();
	render.setup();

	// load UI textures
	file_system.loadGUITextures();
	std::cout << "checking content dir, which is: " << file_system.contentDir << std::endl;
	if (file_system.contentDir != "")
	{
		std::vector<std::string>& filesInContent = file_system.getInDir(file_system.contentDir.c_str());
		gui.tileTextures = textureLoader.loadTextures(filesInContent, file_system.contentDir);

		render.textureAtlas = file_system.loadContentAsAtlas();
	}

	gui.guiInit(&windowManager);

	// TEMP all testing stuff
	std::vector<E_Tile>& _tiles = editor.tiles;

	//~ for (int x = 0; x < 4; x++)
	//~ {
		//~ for (int y = 3; y >= 0; y--)
		//~ {
			//~ editor.add_tile(_tiles.emplace_back(Location(glm::vec4(x, y, 0.0, 0.0)), Visuals(true, glm::vec2(x, y))));
		//~ }
	//~ }
	for (int x = 0; x < 10000; x++)
	{
		_tiles.emplace_back(Location(glm::vec4(x, 0.0, 0.0, 0.0), glm::vec3(1, 2, 1.0f)), Visuals(true, glm::vec2(0, 0)));
	}
	render.add_to_render_list(_tiles);
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
	file_system.trySaveConfigs();
}

int main()
{
	program.loop();
	return 0;
}
