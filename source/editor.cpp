#include <main.h>
#include <editor.h>
#include <iostream>
#include <algorithm>
#include <iterator>

// unneeded yet still here
#include <glm/gtx/string_cast.hpp>

class Program;
extern Program program;

Editor::Editor() {}

Tool Editor::getTool()
{
	return selectedTool;
}

void Editor::setTool(Tool tool)
{
	// having things selected while using the place tool could fuck things up big time
	if (selection.size() > 0 && (tool == SELECT ^ tool == MOVE))
	{
		deselect_all();
	}

	if (tool == PLACE)
	{
		Gizmo& placeCursor = gizmos.emplace_back(Location(program.camera.screen_to_world(toolPos), nextTile.location.Size), nextTile.visuals);
		placeCursorID = placeCursor.ID;
		add_gizmo(placeCursor);
	}
	else
	{
		if (placeCursorID != -1)
		{
			int index;
			ID_to_gizmo(placeCursorID, index);
			remove_gizmo(index);
			placeCursorID = -1;
		}
	}

	selectedTool = tool;
}

GRID_MODE Editor::getGridMode()
{
	return gridMode;
}

void Editor::setGridMode(GRID_MODE newGridMode)
{
	// if the full grid mode is enabled, round every tile to the grid
	if (newGridMode == GRID_MODE_FULL)
	{
		for (int i = tiles.size() - 1; i >= 0; i--)
		{
			glm::vec4 newPos = mymath::floor_to_grid(tiles[i].location.Position);
			tiles[i].location.Position = newPos;
			// update visuals
			program.render.instanceTransformData[i].x = newPos.x;
			program.render.instanceTransformData[i].y = newPos.y;
		}

		program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);
	}

	gridMode = newGridMode;
}

bool Editor::getDirtiness()
{
	return dirty;
}

void Editor::setDirtiness(bool to)
{
	dirty = to;

	program.windowManager.setTitle(program.windowManager.title.c_str());
}

bool Editor::checkForOverlaps(Bounding_Box &box, glm::vec4 &pos)
{
	for (int index = tiles.size() - 1; index >= 0; index--)
	{
		if (tiles[index].location.box.overlaps(tiles[index].location.Position, pos, box))
		{
			return true;
		}
	}

	return false;
}

E_Tile* Editor::positionToTile(glm::vec4 &pos, int &index, bool grid)
{
	// TODO: use an optimized function when the open blf file is in grid only mode
	//~ if (grid){}

	// reverse loop so we get the top-most tile and also speed up removing recently placed tiles
	for (index = tiles.size() - 1; index >= 0; index--)
	{
		if (tiles[index].location.box.contains_position(tiles[index].location.Position, pos))
		{
			return &tiles[index]; // return so we only get 1 tile each click, otherwise shit would be pretty weird, innit?
		}
	}
	return nullptr;
}

// for area tools like box selection
std::vector<E_Tile*>* Editor::getTilesInArea(Bounding_Box area, glm::vec4 &pos, std::vector<int> &indices)
{
	std::vector<E_Tile*>* tilesInArea = new std::vector<E_Tile*>{};
	for (int i = tiles.size() - 1; i >= 0; i--)
	{
		if (area.contains(pos, tiles[i].location.Position, tiles[i].location.box))
		{
			indices.push_back(i);
			tilesInArea->push_back(&tiles[i]);
		}
	}

	return tilesInArea;
}

E_Tile* Editor::ID_to_tile(int ID, int &index)
{
	// reverse loop so we get the top-most tile and also speed up removing recently placed tiles
	for (index = tiles.size() - 1; index >= 0; index--)
	{
		if (tiles[index].ID == ID)
		{
			return &tiles[index]; // return so we only get 1 tile each click, otherwise shit would be pretty weird, innit?
		}
	}
	return nullptr;
}

int Editor::ID_to_gizmo_index(int ID)
{
	// reverse loop so we get the top-most tile and also speed up removing recently placed tiles
	for (int i = gizmos.size() - 1; i >= 0; i--)
	{
		if (gizmos[i].ID == ID)
		{
			return i;
		}
	}
	return -1;
}

Gizmo* Editor::ID_to_gizmo(int ID, int &index)
{
	// reverse loop so we get the top-most tile and also speed up removing recently placed tiles
	for (index = gizmos.size() - 1; index >= 0; index--)
	{
		if (gizmos[index].ID == ID)
		{
			return &gizmos[index]; // return so we only get 1 tile each click, otherwise shit would be pretty weird, innit?
		}
	}
	return nullptr;
}

void Editor::update_tile_selection(E_Tile* tile, int index, bool to)
{
	tile->selected = to;
	program.render.set_tile_selection(index, to);
}

void Editor::update_tile_selection(int ID, bool to)
{
	int index = 0;
	E_Tile* tile = ID_to_tile(ID, index);
	update_tile_selection(tile, index, to);
}

void Editor::select_by_texture(std::string textureName)
{
	// i dont think it's necessary to keep the rest selected when doing this
	deselect_all();

	for (int i = tiles.size() - 1; i >= 0; i--)
	{
		if (tiles[i].visuals.textureName == textureName)
		{
			selection.push_back(&tiles[i]);
			update_tile_selection(&tiles[i], i, true);
		}
	}
}

void Editor::push_selection_to_back()
{
	for (int i = selection.size() - 1; i >= 0; i--)
	{
		int index = 0;
		ID_to_tile(selection[i]->ID, index);

		std::rotate(tiles.begin(), tiles.begin() + index, tiles.end());
		std::rotate(program.render.instanceTransformData.begin(), program.render.instanceTransformData.begin() + index, program.render.instanceTransformData.end());
		std::rotate(program.render.instanceAdditionalData.begin(), program.render.instanceAdditionalData.begin() + index, program.render.instanceAdditionalData.end());
	}

	program.render.updateInstanceArray();
}

void Editor::deselect_all()
{
	if (selection.size() == 0) { return; } // don't waste time if we've got nothing to find

	std::vector<int> indices = {};

	for (int i = tiles.size() - 1; i >= 0; i--)
	{
		if (tiles[i].selected)
		{
			indices.push_back(i);
			tiles[i].selected = false;
		}
	}
	selection.clear();
	program.render.set_tile_selection(indices, false);
}

void Editor::delete_all()
{
	std::vector<int> IDs = {};

	selection.clear();

	for (int i = tiles.size() - 1; i >= 0; i--)
	{
		tiles[i];
		remove_tile(i);
	}
}

void Editor::delete_selection()
{
	if (selection.size() == 0) { return; }
	// TODO: optimize...
	std::vector<int> IDs = {};

	for (int i = selection.size() - 1; i >= 0; i--)
	{
		IDs.push_back(selection[i]->ID);
	}

	selection.clear();

	for (int j = IDs.size() - 1; j >= 0; j--)
	{
		int index = 0;
		ID_to_tile(IDs[j], index);
		remove_tile(index);
	}

	setDirtiness(true);
}

void Editor::clear_tags()
{
	for (int i = 0; i < MAX_TAGS; i++)
	{
		tags[i].assign("NA");
	}
}

void Editor::reset_next_tile()
{
	nextTile.location.Size.x = DEFAULT_TILE.location.Size.x;
	nextTile.location.Size.y = DEFAULT_TILE.location.Size.y;
	nextTile.location.Angle = DEFAULT_TILE.location.Angle;

	nextTile.physics.CollisionsEnabled = DEFAULT_TILE.physics.CollisionsEnabled;
	nextTile.physics.Static = DEFAULT_TILE.physics.Static;
	nextTile.physics.Bounce = DEFAULT_TILE.physics.Bounce;
	nextTile.physics.Density = DEFAULT_TILE.physics.Density;
	nextTile.physics.Friction = DEFAULT_TILE.physics.Friction;

	program.gui.se.currentTextureModeSelection = DEFAULT_TILE.visuals.TextureMode;
	nextTile.visuals.TextureMode = DEFAULT_TILE.visuals.TextureMode;
	nextTile.visuals.TextureSize.x = DEFAULT_TILE.visuals.TextureSize.x;
	nextTile.visuals.TextureSize.y = DEFAULT_TILE.visuals.TextureSize.y;
	nextTile.visuals.Color.x = DEFAULT_TILE.visuals.Color.x;
	nextTile.visuals.Color.y = DEFAULT_TILE.visuals.Color.y;
	nextTile.visuals.Color.z = DEFAULT_TILE.visuals.Color.z;
	nextTile.visuals.Opacity = DEFAULT_TILE.visuals.Opacity;

	for (int i = 0; i < sizeof(nextTile.tags) / sizeof(bool); i++)
	{
		nextTile.tags[i] = false;
	}

	if (placeCursorID != -1)
	{
		int index;
		Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
		resizeGizmo(index, nextTile.location.Size);
		placeCursor->visuals.TextureMode = nextTile.visuals.TextureMode;
		placeCursor->visuals.TextureSize.x = nextTile.visuals.TextureSize.x;
		placeCursor->visuals.TextureSize.y = nextTile.visuals.TextureSize.y;
		placeCursor->visuals.Color.x = nextTile.visuals.Color.x;
		placeCursor->visuals.Color.y = nextTile.visuals.Color.y;
		placeCursor->visuals.Color.z = nextTile.visuals.Color.z;
		placeCursor->visuals.Opacity = nextTile.visuals.Opacity;
		updateGizmoVisuals(index);
	}
}

void Editor::updateToolPos(glm::vec2 &mousePos)
{
	if (!program.gui.guiHovered && !program.file_system.contextOpen)
	{
		if (selectedTool == SELECT && program.input.lmb_down && program.input.lmb_down_last)
		{
			glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);
			glm::vec4 worldCachedMousePos = program.camera.screen_to_world(cachedToolPos);
			//deselect_all();

			std::vector<int> indices = {};
			
			glm::vec2 dragArea = glm::vec2(worldMousePos.x - worldCachedMousePos.x, worldMousePos.y - worldCachedMousePos.y);
			glm::vec4 boundingBoxPos = worldCachedMousePos;

			if (worldMousePos.x < worldCachedMousePos.x)
			{
				dragArea.x = worldCachedMousePos.x - worldMousePos.x;
				boundingBoxPos.x -= worldCachedMousePos.x - worldMousePos.x;
			}

			if (worldMousePos.y < worldCachedMousePos.y)
			{
				dragArea.y = worldCachedMousePos.y - worldMousePos.y;
				boundingBoxPos.y -= worldCachedMousePos.y - worldMousePos.y;
			}

			std::vector<E_Tile*>& tilesInArea = *getTilesInArea(Bounding_Box(dragArea), boundingBoxPos, indices);

			if (tilesInArea.size() > 0)
			{
				for (E_Tile* tile : tilesInArea)
				{
					if (tile->selected == false)
					{
						selection.push_back(tile);
						tile->selected = true;
						int index;
						ID_to_tile(tile->ID, index);
						program.render.set_tile_selection(index, true);
					}
				}
				// selection.assign(tilesInArea.begin(), tilesInArea.end());

				// for (E_Tile* tile : selection)
				// {
				// 	tile->selected = true;
				// }

				// program.render.set_tile_selection(indices, true);
			}
		}
		else if (selectedTool == PLACE)
		{
			if (placeCursorID != -1)
				moveGizmo(program.editor.ID_to_gizmo_index(placeCursorID), program.input.ctrl_down || gridMode != GRID_MODE_NORMAL ? mymath::floor_to_grid(program.camera.screen_to_world(mousePos)) : program.camera.screen_to_world(mousePos));
		}
		else if (selectedTool == BOX)
		{
			// just released lmb, draw the tile(s)
			if (program.input.lmb_down == false && program.input.lmb_down_last == true)
			{
				glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);
				glm::vec4 worldCachedMousePos = program.camera.screen_to_world(cachedToolPos);

				// get area size
				glm::vec2 areaSize = glm::vec2(abs(worldMousePos.x - worldCachedMousePos.x), abs(worldMousePos.y - worldCachedMousePos.y));
				// find the start position depending on drag start and end positions (because size be retarded like that)
				glm::vec4 startPos = glm::vec4(0.0f);
				startPos.x = std::min(worldMousePos.x, worldCachedMousePos.x);
				startPos.y = std::min(worldMousePos.y, worldCachedMousePos.y);

				// apply snapping if needed
				startPos = program.input.ctrl_down || gridMode != GRID_MODE_NORMAL ? mymath::round_to_grid(startPos) : startPos;
				areaSize = program.input.ctrl_down || gridMode != GRID_MODE_NORMAL ? mymath::round_to_grid(areaSize) : areaSize;

				// create tile or tiles
				std::cout << nextTile.visuals.textureName << std::endl;
				if (nextTile.visuals.textureName == "" || areaSize == glm::vec2(0.0f, 0.0f)) return;

				// check for overlaps if required
				if (overlapMode == OVERLAP_NEVER || (overlapMode == OVERLAP_FREE && (program.input.ctrl_down || gridMode != GRID_MODE_NORMAL)))
				{
					// overlapping when not supposed to, don't place a tile
					if (checkForOverlaps(Bounding_Box(areaSize), startPos)) { return; }
				}

				// place the tile
				add_tile(tiles.emplace_back(Location(startPos, glm::vec3(areaSize, 1.0f)), nextTile.physics, nextTile.visuals, nextTile.tags));
			}
			// just released rmb, draw the tiles
			else if (program.input.rmb_down == false && program.input.rmb_down_last == true)
			{
				glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);
				glm::vec4 worldCachedMousePos = program.camera.screen_to_world(cachedToolPos);

				// get area size
				glm::vec2 areaSize = glm::vec2(abs(worldMousePos.x - worldCachedMousePos.x), abs(worldMousePos.y - worldCachedMousePos.y));
				// find the start position depending on drag start and end positions (because size be retarded like that)
				glm::vec4 startPos = glm::vec4(0.0f);
				startPos.x = std::min(worldMousePos.x, worldCachedMousePos.x);
				startPos.y = std::min(worldMousePos.y, worldCachedMousePos.y);

				// apply snapping if needed
				startPos = program.input.ctrl_down || gridMode != GRID_MODE_NORMAL ? mymath::round_to_grid(startPos) : startPos;
				areaSize = mymath::round_to_grid(areaSize);

				// create tile or tiles
				if (nextTile.visuals.textureName == "" || areaSize == glm::vec2(0.0f, 0.0f)) return;

				std::vector<E_Tile> tilesToPlace = {};

				for (int y = 0; y < areaSize.y; y++)
				{
					for (int x = 0; x < areaSize.x; x++)
					{
						// check for overlaps if required
						if (overlapMode == OVERLAP_NEVER || (overlapMode == OVERLAP_FREE && (program.input.ctrl_down || gridMode != GRID_MODE_NORMAL)))
						{
							// overlapping when not supposed to, don't place a tile
							if (!checkForOverlaps(Bounding_Box(glm::vec2(1.0f)), startPos + glm::vec4(x, y, 0.0f, 0.0f)))
							{
								E_Tile tile = E_Tile(Location(startPos + glm::vec4(x, y, 0.0f, 0.0f), glm::vec3(1.0f)), nextTile.physics, nextTile.visuals, nextTile.tags);
								tilesToPlace.push_back(tile);
							}
						}
						else
						{
							E_Tile tile = E_Tile(Location(startPos + glm::vec4(x, y, 0.0f, 0.0f), glm::vec3(1.0f)), nextTile.physics, nextTile.visuals, nextTile.tags);
							tilesToPlace.push_back(tile);
						}
					}
				}

				tiles.insert(tiles.end(), tilesToPlace.begin(), tilesToPlace.end());
				add_tile(tilesToPlace);
			}
		}

		toolPos = mousePos;
	}
}

// executed when a tool is used by pressing GLFW_MOUSE_BUTTON_LEFT or for some tools by holding down LMB (drawing with PLACE for example)
void Editor::tool_use()
{
	if (program.gui.guiHovered || program.file_system.contextOpen) { return; }

	switch(selectedTool)
	{
		case SELECT:
		{
			cachedToolPos = toolPos;

			int index = 0;

			E_Tile* tile = positionToTile(program.camera.screen_to_world(toolPos), index);

			// clicked on empty space
			if (tile == nullptr)
			{
				// not holding shift - deselect all tiles
				if (program.input.shift_down == false)
				{
					deselect_all();
				}
				return;
			}
			// clicked on a tile
			else
			{
				// not holding shift, replace entire selection with tile
				if (program.input.shift_down == false)
				{
					if (selection.size() == 1)
					{
						selection[0]->selected = false;
						update_tile_selection(selection[0]->ID, false);
						selection.pop_back();
					}
					else
					{
						deselect_all();
					}
				}
				
				// only add unselected tile to selection
				if (tile->selected == false)
				{
					selection.push_back(tile);
					update_tile_selection(tile, index, true);
				}
			}
			break;
		}
		case PLACE:
		{
			if (nextTile.visuals.textureName == "") return;
			// get target position
			glm::vec4 pos = program.camera.screen_to_world(toolPos);
			pos = program.input.ctrl_down || gridMode != GRID_MODE_NORMAL ? mymath::floor_to_grid(pos) : pos;
			// check for overlaps if required
			if (overlapMode == OVERLAP_NEVER || (overlapMode == OVERLAP_FREE && (program.input.ctrl_down || gridMode != GRID_MODE_NORMAL)))
			{
				// overlapping when not supposed to, don't place a tile
				if (checkForOverlaps(Bounding_Box(glm::vec2(nextTile.location.Size)), pos)) { return; }
			}
			// place the tile
			add_tile(tiles.emplace_back(Location(pos, nextTile.location.Size), nextTile.physics, nextTile.visuals, nextTile.tags));
			break;
		}
		case BOX:
		{
			cachedToolPos = toolPos;
			break;
		}
	};
}

// executed when a tool's secondary function is used by pressing GLFW_MOUSE_BUTTON_RIGHT or for some tools by holding down RMB (erasing with PLACE for example)
void Editor::tool_use_secondary()
{
	if (program.gui.guiHovered || program.file_system.contextOpen) { return; }

	switch(selectedTool)
	{
		case SELECT:
		{
			int index = 0;
			E_Tile* tile = positionToTile(program.camera.screen_to_world(toolPos), index);

			if (tile == nullptr) { return; }
			if (tile->selected) // tile was already selected, deselect it
			{
				for (int i = selection.size() - 1; i >= 0; i--) // loop through every tile to find a match
				{
					if (selection[i]->ID == tile->ID)
					{
						selection.erase(selection.begin() + i); // remove deselected tile from selection
					}
				}

				update_tile_selection(tile, index, false);
				return;
			}
			// tile wasn't selected before, add to selection
			selection.push_back(tile);
			update_tile_selection(tile, index, true);
			break;
		}
		case PLACE:
		{
			int index = 0;
			E_Tile* tile = positionToTile(program.camera.screen_to_world(toolPos), index);
			if (tile != nullptr)
			remove_tile(index);
			break;
		}
		case BOX:
		{
			cachedToolPos = toolPos;
			break;
		}
	};
}

void Editor::update_atlas_coords(TextureAtlas* atlas)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		// std::cout << "ac: " << tiles[i].visuals.atlasCoords.x << tiles[i].visuals.atlasCoords.y << std::endl;
		glm::vec2 atCoords = program.textureLoader.getAtlasTextureCoords(atlas, tiles[i].visuals.textureName);
		// std::cout << "acafter: " << atCoords.x << atCoords.y << std::endl;
		std::cout << tiles[i].visuals.textureName << std::endl;
		// i really dont know how to handle it being -1 but this will do for now...
		if (atCoords != glm::vec2(-1.0f))
		{
			tiles[i].visuals.atlasCoords = atCoords;
			program.render.instanceTextureData[i].x = atCoords.x;
			program.render.instanceTextureData[i].y = atCoords.y;
		}
		else
		{
			tiles[i].visuals.atlasCoords = glm::vec2(0.0f, 0.0f);
			program.render.instanceTextureData[i].x = 0.0f;
			program.render.instanceTextureData[i].y = 0.0f;
		}
	}

	for (int i = 0; i < gizmos.size(); i++)
	{
		// std::cout << "ac: " << tiles[i].visuals.atlasCoords.x << tiles[i].visuals.atlasCoords.y << std::endl;
		glm::vec2 atCoords = program.textureLoader.getAtlasTextureCoords(atlas, gizmos[i].visuals.textureName);
		// std::cout << "acafter: " << atCoords.x << atCoords.y << std::endl;
		std::cout << gizmos[i].visuals.textureName << std::endl;
		// i really dont know how to handle it being -1 but this will do for now...
		if (atCoords != glm::vec2(-1.0f))
		{
			gizmos[i].visuals.atlasCoords = atCoords;
			program.render.GinstanceTextureData[i].x = atCoords.x;
			program.render.GinstanceTextureData[i].y = atCoords.y;
		}
		else
		{
			gizmos[i].visuals.atlasCoords = glm::vec2(0.0f, 0.0f);
			program.render.GinstanceTextureData[i].x = 0.0f;
			program.render.GinstanceTextureData[i].y = 0.0f;
		}
	}

	// update nextTile too...
	// std::cout << nextTile.visuals.textureName << std::endl;
	glm::vec2 atCoords = program.textureLoader.getAtlasTextureCoords(atlas, nextTile.visuals.textureName);
	nextTile.visuals.atlasCoords = atCoords == glm::vec2(-1.0f) ? glm::vec2(0.0f, 0.0f) : atCoords;
	nextTile.visuals.textureName = program.textureLoader.getAtlasTexturePath(atlas, nextTile.visuals.atlasCoords);
	nextTile.visuals.atlasCoords = program.textureLoader.getAtlasTextureCoords(atlas, nextTile.visuals.textureName);

	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_2);
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_2);

	if (gridGizmoID == -1)
	{
		// only show grid if a grid texture was provided
		if (std::find(program.render.textureAtlas->textureFiles.begin(), program.render.textureAtlas->textureFiles.end(), "grid.png") != program.render.textureAtlas->textureFiles.end())
		{
			Gizmo &gridGizmo = gizmos.emplace_back(
				Location(glm::vec4(-1000.0f, -1000.0f, 0.0f, 0.0f), glm::vec3(2000.0f, 2000.0f, 0.0f)),
				Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, "grid.png"), "grid.png", TEXTUREMODE_TILE, glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.75f)
			);

			add_gizmo(gridGizmo); 
			gridGizmoID = gridGizmo.ID;
		}
	}
}

void Editor::moveTile(int index, glm::vec2 newPos)
{
	// snap position to the grid if needed
	if (gridMode != GRID_MODE_NORMAL)
	{
		newPos = mymath::floor_to_grid(newPos);
	}

	tiles[index].location.Position.x = newPos.x;
	tiles[index].location.Position.y = newPos.y;
	// update visuals
	program.render.instanceTransformData[index].x = newPos.x;
	program.render.instanceTransformData[index].y = newPos.y;
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);

	setDirtiness(true);
}

void Editor::moveTile(unsigned int ID, glm::vec2 newPos)
{
	int index = 0;
	ID_to_tile(ID, index);
	moveTile(index, newPos);
}

void Editor::resizeTile(int index, glm::vec2 newSize)
{
	// snap even the size to the grid if needed
	if (gridMode != GRID_MODE_NORMAL)
	{
		newSize = mymath::floor_to_grid(newSize);
	}

	tiles[index].location.Size.x = newSize.x;
	tiles[index].location.Size.y = newSize.y;
	tiles[index].location.box.update_size(newSize);
	// update visuals
	program.render.instanceTransformData[index].z = newSize.x;
	program.render.instanceTransformData[index].w = newSize.y;
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);

	setDirtiness(true);
}

void Editor::resizeTile(unsigned int ID, glm::vec2 newSize)
{
	int index = 0;
	ID_to_tile(ID, index);
	resizeTile(index, newSize);
}

void Editor::rotateTile(int index, double newRotation)
{
	tiles[index].location.Angle = newRotation;
	// TODO: if i make tiles have visible rotation, i would probably also have to make their bounding boxes have rotation 💀
	// TODO: update instance data when rotation affects rendering
	setDirtiness(true);
}

void Editor::rotateTile(unsigned int ID, double newRotation)
{
	int index = 0;
	E_Tile* tile = ID_to_tile(ID, index);

	rotateTile(index, newRotation);
}

void Editor::updateTileVisuals(int index)
{
	// TODO: update when TextureSize is made to have a visible effect
	Visuals* visuals = &tiles[index].visuals;
	program.render.instanceTextureData[index].x = visuals->atlasCoords.x;
	program.render.instanceTextureData[index].y = visuals->atlasCoords.y;
	program.render.instanceTextureData[index].z = visuals->TextureSize.x;
	program.render.instanceTextureData[index].w = visuals->TextureSize.y;
	program.render.instanceColorData[index].x = visuals->Color.x;
	program.render.instanceColorData[index].y = visuals->Color.y;
	program.render.instanceColorData[index].z = visuals->Color.z;
	program.render.instanceColorData[index].w = visuals->Opacity;
	program.render.instanceAdditionalData[index].x = visuals->TextureMode == TEXTUREMODE_TILE;
	// TODO: make it just update both at once
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_2);
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_3);
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_4);

	setDirtiness(true);
}

void Editor::updateTileVisuals(unsigned int ID)
{
	int index = 0;
	E_Tile* tile = ID_to_tile(ID, index);
	updateTileVisuals(index);
}

/// GIZMOS
void Editor::moveGizmo(int index, glm::vec2 newPos)
{
	// snap position to the grid if needed
	if (gridMode != GRID_MODE_NORMAL)
	{
		newPos = mymath::floor_to_grid(newPos);
	}

	gizmos[index].location.Position.x = newPos.x;
	gizmos[index].location.Position.y = newPos.y;
	// update visuals
	program.render.GinstanceTransformData[index].x = newPos.x;
	program.render.GinstanceTransformData[index].y = newPos.y;
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_1);
}

void Editor::moveGizmo(unsigned int ID, glm::vec2 newPos)
{
	int index = 0;
	ID_to_gizmo(ID, index);
	moveGizmo(index, newPos);
}

void Editor::resizeGizmo(int index, glm::vec2 newSize)
{
	// snap even the size to the grid if needed
	if (gridMode != GRID_MODE_NORMAL)
	{
		newSize = mymath::floor_to_grid(newSize);
	}

	gizmos[index].location.Size.x = newSize.x;
	gizmos[index].location.Size.y = newSize.y;
	gizmos[index].location.box.update_size(newSize);
	// update visuals
	program.render.GinstanceTransformData[index].z = newSize.x;
	program.render.GinstanceTransformData[index].w = newSize.y;
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_1);
}

void Editor::resizeGizmo(unsigned int ID, glm::vec2 newSize)
{
	int index = 0;
	ID_to_gizmo(ID, index);
	resizeGizmo(index, newSize);
}

void Editor::updateGizmoVisuals(int index)
{
	// TODO: update when TextureSize is made to have a visible effect
	Visuals* visuals = &gizmos[index].visuals;
	program.render.GinstanceTextureData[index].x = visuals->atlasCoords.x;
	program.render.GinstanceTextureData[index].y = visuals->atlasCoords.y;
	program.render.GinstanceTextureData[index].z = visuals->TextureSize.x;
	program.render.GinstanceTextureData[index].w = visuals->TextureSize.y;
	program.render.GinstanceColorData[index].x = visuals->Color.x;
	program.render.GinstanceColorData[index].y = visuals->Color.y;
	program.render.GinstanceColorData[index].z = visuals->Color.z;
	program.render.GinstanceColorData[index].w = visuals->Opacity;
	program.render.GinstanceAdditionalData[index].x = visuals->TextureMode == TEXTUREMODE_TILE;
	// TODO: make it just update both at once
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_2);
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_3);
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_4);

	setDirtiness(true);
}

void Editor::updateGizmoVisuals(unsigned int ID)
{
	int index = 0;
	ID_to_gizmo(ID, index);
	updateGizmoVisuals(index);
}

void Editor::update_gizmos()
{
}

void Editor::add_tile(E_Tile &tile)
{
	program.render.add_to_render_list(tile);
	setDirtiness(true);
}

void Editor::add_tile(std::vector<E_Tile> &tiles)
{
	program.render.add_to_render_list(tiles);
	setDirtiness(true);
}

void Editor::remove_tile(int index)
{
	// remove from the tiles vector as well
	if (index != -1)
	{
		program.render.remove_from_render_list(index);
		tiles.erase(std::begin(tiles) + index);
		setDirtiness(true);
	}
}

void Editor::remove_tile(std::vector<int> &indices)
{
	if (indices.size() == 0) { return; } // don't dirty if we do nothing lol

	program.render.remove_from_render_list(indices);

	for (int index : indices)
	{
		// remove from the tiles vector as well
		if (index != -1)
			tiles.erase(std::begin(tiles) + index);
	}

	setDirtiness(true);
}

void Editor::add_gizmo(Gizmo &gizmo)
{
	program.render.add_gizmo_to_render_list(gizmo);
}

void Editor::remove_gizmo(int index)
{
	// remove from the gizmos vector as well
	if (index != -1)
	{
		program.render.remove_gizmo_from_render_list(index);
		gizmos.erase(std::begin(gizmos) + index);
	}
}
