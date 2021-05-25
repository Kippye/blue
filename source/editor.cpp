#include <main.h>
#include <editor.h>
#include <iostream>
#include <algorithm>
#include <iterator>

class Program;
extern Program program;

Editor::Editor(){}

Tool Editor::getTool()
{
	return selectedTool;
}

void Editor::setTool(Tool tool)
{
	selectedTool = tool;
}

Tile* Editor::positionToTile(glm::vec4 &pos)
{
	// TODO: function body
	return nullptr;// LocationToTile[pos];
}

void Editor::updateToolPos(glm::vec2 &mousePos)
{
	// TODO: do some shit with the mouse pos (move the "drawing cursor")
	if (!program.gui.guiHovered && !program.fileSystem.contextOpen)
	{
		toolPos = mousePos;
	}
}

void Editor::tool_use()
{
	switch(selectedTool)
	{
		case (Place):
			// place the tile
			// TODO: drag drawing
			// TODO: box drawing
			// TODO: optimize draw updates
			if (!program.gui.guiHovered && !program.fileSystem.contextOpen)
			{
				glm::vec4 pos = program.camera.screen_to_world(toolPos);
				pos = program.input.ctrl_down ? program.windowManager.round_to_grid(pos) : pos;
				add_tile(tiles.emplace_back(Location(pos), Visuals(true, placementTileTexture)));
				std::cout << "used place tool!" << std::endl;
			}
		break;
	};
}

void Editor::tool_use_secondary()
{
	switch(selectedTool)
	{
		case (Place):
			for (int i = 0; i < tiles.size(); i++)
			{
				if (tiles[i].location.box.contains_position(program.camera.screen_to_world(toolPos)))
				{
					remove_tile(tiles[i], i);
					return; // return so we only remove 1 tile each click, otherwise shit would be pretty weird, innit?
				}
			}
		break;
	};
}

void Editor::add_tile(Tile &tile)
{
	program.render.add_to_render_list(tile);
}

void Editor::remove_tile(Tile &tile, int index)
{
	program.render.remove_from_render_list(tile, index);
	// remove from the tiles vector as well
	if (index != -1)
	tiles.erase(std::begin(tiles) + index);
}
