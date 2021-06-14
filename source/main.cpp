#include <main.h>
#include <editor_tile.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/* TODO LIST:
 * // BUGS //
 * [X] fix weird flipped texture selection
 * [X] fix pointer vector issue when deleting selection
 * [X?] fix eggfault when minimizing using taskbar button
 * // UI //
 * [X] add io.WantCaptureMouse and io.WantCaptureKeyboard checks to ignore input when file dialogues are open
 * [X] load content folder textures as imagebuttons on a grid
 * [X] style ui
 * [X] information gui that shows stuff like the amount of tiles etc
 * [X] toggles for popup menus
 * [X] editor (tool selector, etc) panel
 * [X] section (or separate window) at the bottom of properties panel that lets you set the settings on newly placed tiles
 * [X] grid mode toggle (if tiles have already been placed, they will be snapped to the grid)
 * [?] make fitting window paddings per window
 * [?] toggle visible grid (UI?) (bottom bar?)
 * [C] [replaced by tile selector] some kind of dropdown for texture selection in the properties panel
 * [C] status gui that shows what process is currently done (mostly useful for stuff like reloading textures, opening or saving a BLF file, etc)
 * [C] beautify file dialog
 * [C] set default file dialog sort mode to name ascending
 * // EDITOR //
 * [X] only listen to editor movement commands when no imgui window is focused
 * [X] basic drawing of tiles on a grid
 * [X] non-snapped drawing?
 * [X] draw with selected texture (if none is selected, maybe copy invisible texture to selected content folder and use that)
 * [X] delete tiles with right click
 * [X] select tools with number keys
 * [X] additional properties on tiles (collisions, staticity, size etc, etc, etc)
 * [X] bool in shaders to change color of tiles when they are selected
 * [X] pressing delete or backspace will delete every selected tile
 * [X] toggle overlapping tiles (there will be a bounds check if it's true) (toggle in bottom bar)
 * [X] selecting tiles will show properties for that tile (for multiple selection you can mostly only set them all to the same one)
 * [X] in-editor modifiable ignore list for filenames and / or directories
 * [X] selection tools like area [X], by type [X], etc
 * [1] support for storing tags in tiles (could be one string which is limiting or somehow a map of tags and their names)
 * [C] resize tool ‍GRRRRR
 * [C] undo, redo, if at all doable
 * [C] move tool (with arrow keys & move camera with it)
 * // OTHER //
 * [X] fix up editor tile class
 * [X] only recreate transforms array when a tile is modified (use some kind of event system)
 * [X] really just fix the fucking texture loading system
 * [X] proper input for repeating mouse button presses
 * [X] save user data / preferences:
 * --- could be divided up into parts like Gui prefs, editor prefs, etc that are then combined into one file DEBUG A LOT!
 * --- // Gui //
 * --- [X] save file dialog sorting modes, maybe a base directory as well
 * [X] add some way to convert from texture to atlas coordinates and reverse
 * [?] more options when loading textures (format or smth)
 * [C] create an event system ✌
 * [C] some kind of timer system in render?
 *
 */
/* [X] RELEASE 1.0:
 *    [X] save configs (mostly content folder)
 *    [X] convert tiles to and from blf files
 *    [X] save / open blf files
 */

class E_Tile;
class Location;
class Visuals;

Program program;

Program::Program()
{
	// setup and initialize components
	file_system.tryLoadConfigs();

	windowManager.initialize();
	input.setup();
	render.setup();

	// load UI textures
	file_system.loadGUITextures();
	std::cout << "checking content dir, which is: " << file_system.contentDir << std::endl;

	file_system.updateTextures();

	gui.guiInit(&windowManager);

	// TEMP all testing stuff
	std::vector<E_Tile>& _tiles = editor.tiles;

	//~ for (int y = 0; y < 4; y++)
	//~ {
		//~ for (int x = 0; x < 4; x++)
		//~ {
			//~ _tiles.emplace_back(Location(glm::vec4(x * 12, y * 12, 0.0, 0.0), glm::vec3(size, size, 1.0f)), Physics(), Visuals(glm::vec2(x, y), TEXTUREMODE_TILE));
			//~ size = size == 10 ? 1 : size + 1;
		//~ }
	//~ }
	//~ render.add_to_render_list(_tiles);
	//~ for (int x = 0; x < 10; x++)
	//~ {
		//~ _tiles.emplace_back(Location(glm::vec4(x, 0.0, 0.0, 0.0), glm::vec3(1.0f, 1.0f, 1.0f)), Physics(false, true), Visuals(textureLoader.getAtlasTextureCoords(render.textureAtlas, "box.png"), "box.png", TEXTUREMODE_TILE));
		//~ //_tiles.emplace_back(Location(glm::vec4(x, 0.0, 0.0, 0.0), glm::vec3(1.0f, 1.0f, 1.0f)), Physics(false, true), Visuals(textureLoader.getAtlasCoords(render.textureAtlas, 0), render.textureAtlas->textureFiles[0], TEXTUREMODE_TILE));
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
	file_system.trySaveConfigs();
}

int main()
{
	program.loop();
	return 0;
}
