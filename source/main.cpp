#include <main.h>
#include <editor_tile.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/* TODO LIST:
 * // BUGS //
 * [X] fix weird flipped texture selection
 * [X] fix pointer vector issue when deleting selection
 * [X?] fix eggfault when minimizing using taskbar button
 * [X] When a tile is deselected by clicking an empty space, it for some reason makes every other tile visually selected
 * [X] placement cursor doesn't use TextureMode
 * [X] switching from placement cursor to select and back to place = crash
 * [X] repeatedly selecting the place tool creates duplicate cursors
 * [X] overlap avoidance doesn't even work correctly
 * [X] at first, a pretty random texture is selected, rather than the first one in the texture selector
 * [X] having a larger amount of textures in content causes FPS to drop drastically
 * [X] tiling inaccuracies when using larger atlases & textures
 * [X] reload textures + select place tool = grid disappears
 * [X] crash when loading a file but there are no textures - "fixed" by making doing so impossible
 * [X] grid texture doesn't seen to be actually loaded from the program's textures
 * [X?] reloading textures needs to be made more stable
 * [X] box selection additive doesn't work
 * [X] drag gizmo not reset when switching from box draw to drag select -  NOTE: maybe just make 2 separate gizmos?
 * [X] make the drag select do size calculation the same way as box place
 * [X] can i reuse the indices vector in drag selection?
 * [X] box selection and draw gizmo doesn't disappear when the drag is ended over gui
 * [X] box drag can draw wayyy too tiny tiles
 * [X] TEXTUREMODE_TILE gets offset quite strangely only on the Y axis
 * [X] having 1 tile selected and placing more causes a crash -- tested pre-gizmos, it was a pre-existing issue! - fixed by just making selecting a different tool deselect all tiles
 * [X] handle the tile that is being resized / moved being deleted by the user (causes crash) 
 * [X] fix box place gizmo size / tiling 
 * [X] if tiles are placed with box draw and then nextTile (size) is modified, it causes the gizmo to become visible again
 * [X] properties gui gets REALLY messed up sometimes, example: copy 2 tiles, paste them a bunch of times
 * [X] pasting a tile while manipulating one using gizmos causes a freeze-crash - fixed, the issue was pointers in the selection vector being invalidated by the tiles vector reallocating after push_back
 * [X] box placement primary mode places more than 1 tile -- related to above issue -- this is caused by the mouse button weirdly clicking again, causing dragBegin to change and a 0;0 tile (or even multiple) to be created -- crappily fixed
 * [X] the box placement bug also happens for box selection - i am still not sure if this is caused by my mouse or the program... this is now CONFIRMED to be a bug with my mouse that the program just doesn't (have to) account for
 * [X] saving any ignores causes a crash, at least fix this even if ignores are kind of broken anyway
 * [X] changing grid size while no content is loaded causes a freeze-crash - fixed by making it not editable in this case
 * [X] clicking in the editor view without a content folder loaded causes a crash OBVIOUSLY...
 * [X] drag placement doesn't work with the camera anymore
 * [X] push to back is VERY broken, either add functionality to Z position or fix it - it's messing with textures and crap
 * [X] ignore inputs if ignore popup is focused
 * [ ] Angle is not copied
 * [ ] box draw start position doesn't properly snap to grid
 * [ ] fix weird bug where box-selecting a single tile while hovering kind of over a gizmo makes the tile get moved using a gizmo (caused by broken mouse??)
 * [ ] place cursor (and probably the actual place position too) is too far behind mouse cursor
 * [ ] fix box place primary mode TEXTUREMODE_TILE gizmo having the same kind of texture tiling offset issue as every tile did before if the gizmo is used in negative directions - fixing this would probably require using some additional instance data...
 * [ ] grid texture tiles wrong with some (mostly odd-numbered) grid sizes (i.e. 7, 9, 21) 
 * [ ] when a tile has no texture, its texture in the properties panel is some random one, not the missing texture
 * [ ] somehow typing a tag made blue crash?? depends on typing speed for some reason
 * [?] weirdly just moving around placing things caused a crash
 * [?] editor cursor glitches weirdly and sometimes just doesn't show
 * [?] crash when a tile is selected and textures are reloaded then a tile is placed
 * [C] can't save directories that contain specific characters to config - it doesn't seem to be caused by characters
 * // UI // 
 * [X] add io.WantCaptureMouse and io.WantCaptureKeyboard checks to ignore input when file dialogues are open
 * [X] load content folder textures as imagebuttons on a grid
 * [X] style ui
 * [X] information gui that shows stuff like the amount of tiles etc
 * [X] toggles for popup menus
 * [X] editor (tool selector, etc) panel
 * [X] section (or separate window) at the bottom of properties panel that lets you set the settings on newly placed tiles
 * [X] grid mode toggle (if tiles have already been placed, they will be snapped to the grid)
 * [X] set default file dialog sort mode to name ascending
 * [X] grid settings under editor tab? - opacity, toggle ALSO make it toggle by pressing G
 * [ ] texture selector scrollbar disappears when active
 * [ ] hide "i" display from properties, keep ID
 * [ ] make a new gui style
 * [ ] beautify file dialog
 * [ ] look for a font to use?
 * [?] make fitting window paddings per window
 * [C] [replaced by tile selector] some kind of dropdown for texture selection in the properties panel
 * [C] status gui that shows what process is currently done (mostly useful for stuff like reloading textures, opening or saving a BLF file, etc)
 * // EDITOR //
 * [X] box draw tool: shortcut 3, click to select start and end point, will use the texturemode of the next tile, right click to fill the area with 1x1 tiles
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
 * [X] support for storing tags in tiles (could be one string which is limiting or somehow a map of tags and their names)
 * [X] place cursor should support (and show) grid snap
 * [X] figure out how to draw gizmos (new shaders for them?)
 * [X] improve grid mode and overlap mode:
 * 		- only really 2 of each are required: overlap always or never, grid snap normal or automatic
 * 		- this means they could just be made toggles instead of enums: Auto-snap, Overlap
 * [X?] make tiles that have lost their texture use missing.png (either loaded from the content folder or stored in /textures or hardcoded) but maybe keep the textureName
 * [X] create gizmos for drag selection boxes
 * [X] fix box selection / placement (it weirdly follows the camera rather than staying in the same spot and changing size)
 * [X] move tool (with mouse)
 * [X] resize tool ‍GRRRRR
 * [X] create gizmos for box place preview
 * [X] box place secondary mode could be snapped to the size of a tile, since it won't place any half tiles anyway
 * [X] copy selection
 * [X] paste
 * [X] cut selection
 * [X] select all button (only visible for select tool, like select by texture) - shortcut == what? could be CTRL + A but it violates my rule of not using movement keys CTRL + 1?
 * [X] shortcuts for all editor actions? B - push to back, T - select by texture?
 * [X] buttons for all editor actions? it would fill up the empty space more but also increase ui complexity, i.e grid toggle button, copy, cut buttons (paste would not work properly)
 * [X?] variable grid size - works, might have some issues
 * [X] animate selected tile color with time sine, makes it more clear what is selected
 * [X] i think it would be a nice touch if i added a little animation / flash to the place cursor when a tile is placed (currently the kind of just has to trust that one was placed since one can't see the tile behind the cursor)
 * [X] display tile texture visually in properties for both singular selected tiles and the next tile options
 * [ ] Z positioning
 * [ ] button to disable the effect of opacity in-editor
 * [ ] zoom into the cursor position
 * [ ] undo, redo, if at all doable
 * [ ] explorer that shows a list of all the tiles, that can optionally be named as well and selected from here (should the names be saved somehow? to blue's files or into the blf?)
 * [ ] moving multiple tiles using handles
 * [ ] merge function - player can place a number of tiles with various sizes to create an organically shaped area (probably the ground). 
 * 		they can then use the merge function to merge those tiles (probably selected ones) into the largest possible tiles for optimization
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
 * [X] i realised i could just make the placement gizmo permanent and just change the opacity to 0 to hide it... sure, this would add some constant overhead, but remove unnecessary ifs and memory usage (for creation / destruction)
 * [X] properly support textures larger than 16x16 in the texture atlas
 * [X] properly support textures larger than 16x16 in rendering (i would have to get 2 sets of atlasCoords)
 * [X] optimize texture reloading - check what takes the longest and make it take less long (i think it's not the actual loading but rather applying the new textures)
 * [X] i could trade performance when placing for performance in many other operations by essentially having a map that maps tiles' IDs to their indices, which would be easy to update when tiles are placed but have to be almost entirely remade when they are removed
 * [X] add editor function implementations for groups of objects - it would speed up things like moving 14000 tiles by only performing 1 instance update
 * [ ] the camera class hasn't been updated in ages, i think it should be made so that the camera is "moved" every frame to update lastMovement, but the view is only updated if there is actual movement - also update input.cpp after this
 * [ ] it would be cleaner to create a specific bool for whether a content folder has been properly loaded than checking a random vector's size in the gui class
 * [ ] Z position functionality
 * [ ] support duplicate texture names by using paths internally (still, displaying only filenames)
 * [ ] separate actual rendering in render.cpp from other tasks it performs so rendering can be disabled for testing
 * [ ] a way to manage projects, kind of. the user could have like a list of BLF files that would be associated with one project and some others associated with another
 * 		they could then switch between projects and easily access all the needed BLF files and also get the correct content file. there could also be separate configuration files for projects (mostly for ignore lists & folders)
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
/* [X] RELEASE 1.1 RENDERING UPDATE:
 *	  [X] Fix tiling inaccuracy
 *	  [X] Add missing.png
 * 	  [X] Built-in grid.png?
 * 	  [C] Fix cursor bug?
*/
/* [X] RELEASE 1.2 TOOLS UPDATE:
 *    [X] Fix box selection & placement
 *	  [X] Add move tool
 *    [X] Add resize / scale tool
 *    [X] Add gizmos for all the tools
 *    [X] Add copy / cut / paste
 *    [X] Fix box place tool (the multi-clicking thing)
 * 	  [X] Select all button
 *    [X] Make move & resize more stable and clean
 */
/* [ ] RELEASE 1.3 GUI & TEXTURES UPDATE:
 *    [X] Placement feedback
 * 	  [X] Visual texture in properties panel
 *    [X] Fixed push to back so there's a temporary solution for Z positioning (in editor only)
 *    [X] Use a different, less debug-looking font
 *    [ ] Create a new gui style for pretty much everything
 *    [ ] Customize the file dialogue
 *	  [ ] Animate the GUI? These should probably be optional
			[ ] Textures from loaded content folder added one at a time to the texture selector
			[ ] Smoothly move stuff around
			[ ] Fade popups and such in / out rather than *popping* them, same goes for buttons that are shown / hidden
*/

class E_Tile;
class Location;
class Visuals;

Program program;

Program::Program()
{
	// setup and initialize components
	file_system.tryLoadConfigs();
	file_system.tryOpenLastFile();

	windowManager.initialize();
	input.setup();
	render.setup();

	// load textures
	file_system.loadGUITextures();
	file_system.updateTextures();
	gui.guiInit(&windowManager);
}

void Program::loop()
{
	while (quitProgram == false)
	{
		input.processInput(windowManager.window);
		//std::cout << "Processed input" << std::endl;
		// update the camera view direction (not really needed but eh)
		camera.updateView();
		//std::cout << "Updated camera view" << std::endl;
		if (glfwWindowShouldClose(windowManager.window))
		{
			programWillClose = quitProgram = true;
			// show a confirmation window if unsaved progress exists
			if (program.editor.getDirtiness() == true)
			{
				quitProgram = false;
				program.gui.popupToggles[SAVE_CONTEXT] = true;
			}
		}
		editor.update_gizmos();
		render.render();
		render.instanceDataUpdates = 0;
		//std::cout << "Rendered" << std::endl;
		glfwPollEvents();
		//std::cout << "Polled GLFW events" << std::endl;
	}
	
	std::cout << "Terminating program..." << std::endl;
	render.terminate();
	windowManager.terminate();
	file_system.trySaveConfigs();
}

int main()
{
	program.loop();
	return 0;
}
