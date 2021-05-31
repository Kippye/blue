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

E_Tile* Editor::positionToTile(glm::vec4 &pos, int &index, bool grid)
{
	// TODO: use an optimized function when the open blf file is in grid only mode
	//~ if (grid){}

	// reverse loop so we get the top-most tile and also speed up removing recently placed tiles
	for (index = tiles.size() - 1; index >= 0; index--)
	{
		if (tiles[index].location.box.contains_position(pos))
		{
			return &tiles[index]; // return so we only get 1 tile each click, otherwise shit would be pretty weird, innit?
		}
	}
	return nullptr;
}

// for area tools like box selection
std::vector<E_Tile*>& Editor::getTilesInArea(Bounding_Box area, std::vector<int> &indices)
{
	std::vector<E_Tile*>* tilesInArea = new std::vector<E_Tile*>();
	// TODO func body
	return *tilesInArea;
}

void Editor::updateToolPos(glm::vec2 &mousePos)
{
	// TODO: do some shit with the mouse pos (move the "drawing cursor")
	if (!program.gui.guiHovered && !program.file_system.contextOpen)
	{
		toolPos = mousePos;
	}
}

void Editor::tool_use()
{
	if (program.gui.guiHovered || program.file_system.contextOpen) { return; }

	switch(selectedTool)
	{
		case SELECT:
		{
			// TODO: drag select, multi select
			int index = 0;
			E_Tile* tile = positionToTile(program.camera.screen_to_world(toolPos), index);

			if (program.input.ctrl_down) // additive selection
			{
				std::cout << "what? why?" << std::endl; // DEBUG: so this just runs once fsr?
				if (tile->selected) // tile was already selected, deselect it
				{
					selection.erase(selection.begin() + index);
					tile->selected = false;
					return;
				}
				// tile wasn't selected before, add to selection
				selection.push_back(tile);
				tile->selected = true;
			}
			else
			{
				if (tile == nullptr) // selected empty space, deselect all tiles
				{
					for (int i = selection.size() - 1; i == 0; i++)
					{
						selection[i]->selected = false;
						selection.pop_back();
					}
					return;
				}
				if (selection.size() == 1) // replace selection
				{
					selection[0] = tile;
				}
				else // create selection
				{
					selection.push_back(tile);
				}
				tile->selected = true;
			}
			break;
		}
		case PLACE:
		{
			// place the tile
			// TODO: box drawing
			// TODO: optimize draw updates
			glm::vec4 pos = program.camera.screen_to_world(toolPos);
			pos = program.input.ctrl_down ? program.windowManager.round_to_grid(pos) : pos;
			add_tile(tiles.emplace_back(Location(pos), Visuals(true, placementTileTexture)));
			std::cout << "used place tool!" << std::endl;
			break;
		}
	};
}

void Editor::tool_use_secondary()
{
	switch(selectedTool)
	{
		case SELECT:
		{
			// TODO: remove a single tile from the selection without affecting the rest
		break;
		}
		case PLACE:
		{
			int index = 0;
			E_Tile* tile = positionToTile(program.camera.screen_to_world(toolPos), index);
			if (tile != nullptr)
			remove_tile(*tile, index);
			break;
		}
	};
}

void Editor::add_tile(E_Tile &tile)
{
	program.render.add_to_render_list(tile);
}

void Editor::remove_tile(E_Tile &tile, int index)
{
	program.render.remove_from_render_list(tile, index);
	// remove from the tiles vector as well
	if (index != -1)
	tiles.erase(std::begin(tiles) + index);
}
