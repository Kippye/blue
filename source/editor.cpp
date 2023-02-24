#include <main.h>
#include <editor.h>
#include <iostream>
#include <algorithm>
#include <iterator>

// unneeded yet still here
#include <glm/gtx/string_cast.hpp>

class Program;
extern Program program;

Editor::Editor() 
{
}

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
		int index;
		Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
		resizeGizmo(index, nextTile.location.Size);
		placeCursor->visuals.TextureMode = nextTile.visuals.TextureMode;
		placeCursor->visuals.Opacity = 1;
		placeCursor->visuals.TextureSize = nextTile.visuals.TextureSize;
		updateGizmoVisuals(index);
	}
	else
	{
		if (placeCursorID != -1)
		{
			int index;
			Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
			resizeGizmo(index, glm::vec2(0.0f, 0.0f));
			placeCursor->visuals.Opacity = 0;
			updateGizmoVisuals(index);
		}
	}

	selectedTool = tool;
}

bool Editor::getAutosnap()
{
	return autosnap;
}

void Editor::setAutosnap(bool to)
{
	autosnap = to;
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

Gizmo* Editor::positionToGizmo(glm::vec4 &pos, int &index, GizmoType acceptedTypes, bool grid)
{
	// TODO: use an optimized function when the open blf file is in grid only mode
	//~ if (grid){}

	for (index = gizmos.size() - 1; index >= 0; index--)
	{
		if (acceptedTypes & gizmos[index].type && gizmos[index].location.box.contains_position(gizmos[index].location.Position, pos))
		{
			return &gizmos[index];
		}
	}
	return nullptr;
}

Gizmo* Editor::ID_to_gizmo(int ID, int &index)
{
	// reverse loop so we get the top-most tile and also speed up removing recently placed tiles
	for (index = gizmos.size() - 1; index >= 0; index--)
	{
		if (gizmos[index].ID == ID)
		{
			return &gizmos[index];
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
	if (selectedTool == SELECT)
	{
		if (program.input.lmb_down && program.input.lmb_down_last)
		{
			glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);

			// get area size
			glm::vec2 areaSize = glm::vec2(abs(worldMousePos.x - dragBegin.x), abs(worldMousePos.y - dragBegin.y));
			// find the start position depending on drag start and end positions (because size be retarded like that)
			glm::vec4 boundingBoxPos = glm::vec4(0.0f);
			boundingBoxPos.x = std::min(worldMousePos.x, dragBegin.x);
			boundingBoxPos.y = std::min(worldMousePos.y, dragBegin.y);

			if (activeDraggerID != -1)
			{
				int tIndex;
				bool activeDraggerFound = false;
				E_Tile* tile = ID_to_tile(selection[0]->ID, tIndex);

				// REALLY TEMPY there are like no checks here whatsoever
				// things to take into account:
				// more than 1 tile selected
				// player DELETING the tile they are editing (could also be by creating a new file)
				// check if the active dragger is a resize dragger, rather than a move one
				for (int i = 0; i < sizeof(scaleDraggerGizmoIDs) / sizeof(int); i++)
				{
					if (activeDraggerFound) { break; }

					if (activeDraggerID == scaleDraggerGizmoIDs[i])
					{
						switch(i)
						{
							// TOP LEFT DRAGGER
							case 0:
							{
								glm::vec2 newSize;
								newSize.x = std::max(tile->location.Position.x + tile->location.Size.x - worldMousePos.x, 0.1f);
								newSize.y = std::max(worldMousePos.y - tile->location.Position.y, 0.1f);
								newSize = program.input.ctrl_down || autosnap ? mymath::round_to_grid(newSize) : newSize;
								glm::vec4 offset = glm::vec4(0.0f);
								offset.x = tile->location.Size.x - newSize.x;
								offset = program.input.ctrl_down || autosnap ? mymath::round_to_grid(offset) : offset;
								resizeTile(tIndex, newSize);
								moveTile(tIndex, tile->location.Position + offset);
								activeDraggerFound = true;
								break;
							}
							// TOP RIGHT DRAGGER
							case 1:
							{
								glm::vec2 newSize;
								newSize.x = std::max(worldMousePos.x - tile->location.Position.x, 0.1f);
								newSize.y = std::max(worldMousePos.y - tile->location.Position.y, 0.1f);
								newSize = program.input.ctrl_down || autosnap ? mymath::round_to_grid(newSize) : newSize;
								resizeTile(tIndex, newSize);
								activeDraggerFound = true;
								break;
							}
							// BOTTOM LEFT DRAGGER
							case 2:
							{
								glm::vec2 newSize;
								newSize.x = std::max(tile->location.Position.x + tile->location.Size.x - worldMousePos.x, 0.1f);
								newSize.y = std::max(tile->location.Position.y + tile->location.Size.y - worldMousePos.y, 0.1f);
								newSize = program.input.ctrl_down || autosnap ? mymath::round_to_grid(newSize) : newSize;
								glm::vec4 offset = glm::vec4(0.0f);
								offset.x = tile->location.Size.x - newSize.x;
								offset.y = tile->location.Size.y - newSize.y;
								offset = program.input.ctrl_down || autosnap ? mymath::round_to_grid(offset) : offset;
								resizeTile(tIndex, newSize);
								moveTile(tIndex, tile->location.Position + offset);
								activeDraggerFound = true;
								break;
							}
							// BOTTOM RIGHT DRAGGER
							case 3:
							{
								glm::vec2 newSize;
								newSize.x = std::max(worldMousePos.x - tile->location.Position.x, 0.1f);
								newSize.y = std::max(tile->location.Position.y + tile->location.Size.y - worldMousePos.y, 0.1f);
								newSize = program.input.ctrl_down || autosnap ? mymath::round_to_grid(newSize) : newSize;
								glm::vec4 offset = glm::vec4(0.0f);
								offset.y = tile->location.Size.y - newSize.y;
								offset = program.input.ctrl_down || autosnap ? mymath::round_to_grid(offset) : offset;
								resizeTile(tIndex, newSize);
								moveTile(tIndex, tile->location.Position + offset);
								activeDraggerFound = true;
								break;
							}
						}
					}
				}
				// REALLY TEMPY there are like no checks here whatsoever
				// check if the active dragger is a resize dragger, rather than a move one
				for (int i = 0; i < sizeof(moveDraggerGizmoIDs) / sizeof(int); i++)
				{
					if (activeDraggerFound) { break; }

					if (activeDraggerID == moveDraggerGizmoIDs[i])
					{
						switch(i)
						{
							// RIGHT CENTER DRAGGER (HORIZONTAL)
							case 0:
							{
								float newX = program.input.ctrl_down || autosnap ? mymath::round_to_grid(worldMousePos.x - tile->location.Size.x) : worldMousePos.x - tile->location.Size.x;
								moveTile(tIndex, glm::vec2(newX, tile->location.Position.y));
								activeDraggerFound = true;
								break;
							}
							// TOP CENTER DRAGGER (VERTICAL)
							case 1:
							{
								float newY = program.input.ctrl_down || autosnap ? mymath::round_to_grid(worldMousePos.y - tile->location.Size.y) : worldMousePos.y - tile->location.Size.y;
								moveTile(tIndex, glm::vec2(tile->location.Position.x, newY));
								activeDraggerFound = true;
								break;
							}
							// CENTER DRAGGER (BIAXIAL)
							case 2:
							{
								glm::vec2 newPosition = glm::vec3(worldMousePos) - tile->location.Size / 2.0f;
								newPosition = program.input.ctrl_down || autosnap ? mymath::round_to_grid(newPosition) : newPosition;
								moveTile(tIndex, newPosition);
								activeDraggerFound == true;
								break;
							}
						}
					}
				}
				toolPos = mousePos;
				return;
			}

			// draw the gizmo
			int index;
			Gizmo* dragGizmo = ID_to_gizmo(dragGizmoID, index);
			moveGizmo(index, boundingBoxPos);
			resizeGizmo(index, areaSize);

			std::vector<int> indices = {};
			std::vector<E_Tile*>& tilesInArea = *getTilesInArea(Bounding_Box(areaSize), boundingBoxPos, indices);

			// deselect tiles that were in the last selection area but not in the new one
			for (E_Tile* oldTile : lastSelectionArea)
			{
				bool isInNewArea = false;

				for (E_Tile* newTile : tilesInArea)
				{
					if (oldTile->ID == newTile->ID)
					{
						isInNewArea = true;
						break;
					}
				}

				if (isInNewArea == false)
				{
					for (int i = selection.size() - 1; i >= 0; i--) // loop through every selected tile to find a match
					{
						if (selection[i]->ID == oldTile->ID)
						{
							selection.erase(selection.begin() + i); // remove deselected tile from selection
						}
					}

					int oldTileIndex = 0;
					ID_to_tile(oldTile->ID, oldTileIndex);
					update_tile_selection(oldTile, oldTileIndex, false);
				}
			}

			for (int i = 0; i < tilesInArea.size(); i++)
			{
				E_Tile* tile = tilesInArea[i];
				
				if (tile->selected == false)
				{
					selection.push_back(tile);
					tile->selected = true;
					program.render.set_tile_selection(indices[i], true);
				}
			}

			lastSelectionArea = tilesInArea;
		}
		// drag ended
		else if (program.input.lmb_down == false && program.input.lmb_down_last == true)
		{
			// reset the active dragger gizmo
			if (activeDraggerID != -1)
			{
				int index;
				Gizmo* activeDraggerGizmo = ID_to_gizmo(activeDraggerID, index);
				if (activeDraggerGizmo->type == GizmoType_MoveDragger)
				{
					for (int i = 0; i < sizeof(moveDraggerGizmoIDs) / sizeof(int); i++)
					{
						if (activeDraggerID == moveDraggerGizmoIDs[i])
						{
							activeDraggerGizmo->visuals.Color = moveDraggerColors[i];
							break;
						}
					}
				}
				else
				{
					activeDraggerGizmo->visuals.Color = scaleDraggerColor;
				}
				activeDraggerID = -1;
				updateGizmoVisuals(index);
			}
			// hide the drag area gizmo
			int index;
			Gizmo* dragGizmo = ID_to_gizmo(dragGizmoID, index);
			resizeGizmo(index, glm::vec2(0.0f));
			lastSelectionArea = {};
		}
	}
	else if (selectedTool == PLACE)
	{
		if (placeCursorID != -1)
			moveGizmo(program.editor.ID_to_gizmo_index(placeCursorID), program.input.ctrl_down || autosnap ? mymath::floor_to_grid(program.camera.screen_to_world(mousePos)) : program.camera.screen_to_world(mousePos));
	}
	else if (selectedTool == BOX)
	{
		// drawing in primary mode
		if (program.input.lmb_down_last == true)
		{
			glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);

			// get area size
			glm::vec2 areaSize = glm::vec2(abs(worldMousePos.x - dragBegin.x), abs(worldMousePos.y - dragBegin.y));
			// find the start position depending on drag start and end positions (because size be retarded like that)
			glm::vec4 startPos = glm::vec4(0.0f);
			startPos.x = std::min(worldMousePos.x, dragBegin.x);
			startPos.y = std::min(worldMousePos.y, dragBegin.y);

			// apply snapping if needed
			startPos = program.input.ctrl_down || autosnap ? mymath::round_to_grid(startPos) : startPos;
			areaSize = program.input.ctrl_down || autosnap ? mymath::round_to_grid(areaSize) : areaSize;

			// still dragging
			if (program.input.lmb_down)
			{
				// update the gizmo
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
				moveGizmo(index, startPos);
				resizeGizmo(index, areaSize);
			}
			// just released lmb, draw the tile
			else
			{
				// not overlapping when not supposed to, place a tile
				if (overlap == true || checkForOverlaps(Bounding_Box(areaSize), startPos))
				{
					// place the tile
					add_tile(tiles.emplace_back(Location(startPos, glm::vec3(areaSize, 1.0f)), nextTile.physics, nextTile.visuals, nextTile.tags));
				}
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
				resizeGizmo(index, glm::vec2(0.0f));
			}
		}
		// drawing in the secondary mode
		else if (program.input.rmb_down_last == true)
		{
			glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);

			// get area size
			glm::vec2 areaSize = glm::vec2(abs(worldMousePos.x - dragBegin.x), abs(worldMousePos.y - dragBegin.y));
			// find the start position depending on drag start and end positions (because size be retarded like that)
			glm::vec4 startPos = glm::vec4(0.0f);
			startPos.x = std::min(worldMousePos.x, dragBegin.x);
			startPos.y = std::min(worldMousePos.y, dragBegin.y);

			// apply snapping if needed
			startPos = program.input.ctrl_down || autosnap ? mymath::round_to_grid(startPos) : startPos;
			areaSize = mymath::round_to_grid(areaSize);

			// still dragging
			if (program.input.rmb_down)
			{
				// update the gizmo
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
				// size = 2;2 area = 4;4 ts = 0.5; 0.5
				placeCursor->visuals.TextureSize.x = nextTile.location.Size.x / areaSize.x; 
				placeCursor->visuals.TextureSize.y = nextTile.location.Size.y / areaSize.y;
				placeCursor->visuals.TextureMode = TEXTUREMODE_STRETCH;
				updateGizmoVisuals(index); 
				moveGizmo(index, startPos);
				resizeGizmo(index, areaSize);
			}
			// just released rmb, draw the tiles
			else
			{
				std::vector<E_Tile> tilesToPlace = {};

				for (int y = 0; y < areaSize.y; y += nextTile.location.Size.y)
				{
					for (int x = 0; x < areaSize.x; x += nextTile.location.Size.x)
					{
						// check for overlaps if required
						if (overlap == false)
						{
							// not overlapping
							if (!checkForOverlaps(Bounding_Box(nextTile.location.Size), startPos + glm::vec4(x, y, 0.0f, 0.0f)))
							{
								E_Tile tile = E_Tile(Location(startPos + glm::vec4(x, y, 0.0f, 0.0f), nextTile.location.Size), nextTile.physics, nextTile.visuals, nextTile.tags);
								tilesToPlace.push_back(tile);
							}
						}
						else
						{
							E_Tile tile = E_Tile(Location(startPos + glm::vec4(x, y, 0.0f, 0.0f), nextTile.location.Size), nextTile.physics, nextTile.visuals, nextTile.tags);
							tilesToPlace.push_back(tile);
						}
					}
				}

				tiles.insert(tiles.end(), tilesToPlace.begin(), tilesToPlace.end());
				add_tile(tilesToPlace);
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
				resizeGizmo(index, glm::vec2(0.0f));
			}
		}
	}

	if (!program.gui.guiHovered && !program.file_system.contextOpen)
	{
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
			if (selection.size() == 1)
			{
				int index_;
				Gizmo* clickedGizmo = positionToGizmo(program.camera.screen_to_world(toolPos), index_, GizmoType_ResizeDragger | GizmoType_MoveDragger);

				if (clickedGizmo != nullptr)
				{
					// TEMP
					dragBegin = program.camera.screen_to_world(toolPos);
					activeDraggerID = clickedGizmo->ID;
					clickedGizmo->visuals.Color = clickedGizmo->visuals.Color * 2.0f;
					updateGizmoVisuals(index_);
					return;
				}
			}

			int index = 0;

			E_Tile* tile = positionToTile(program.camera.screen_to_world(toolPos), index);

			dragBegin = program.camera.screen_to_world(toolPos);

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
			pos = program.input.ctrl_down || autosnap ? mymath::floor_to_grid(pos) : pos;
			// check for overlaps if required
			if (overlap == false && checkForOverlaps(Bounding_Box(glm::vec2(nextTile.location.Size)), pos))
			{
				// overlapping when not supposed to, don't place a tile
				return;
			}
			// place the tile
			add_tile(tiles.emplace_back(Location(pos, nextTile.location.Size), nextTile.physics, nextTile.visuals, nextTile.tags));
			break;
		}
		case BOX:
		{
			dragBegin = program.camera.screen_to_world(toolPos);
			int index;
			Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
			placeCursor->visuals.atlasLocation = nextTile.visuals.atlasLocation;
			placeCursor->visuals.textureName = nextTile.visuals.textureName;
			placeCursor->visuals.TextureMode = TEXTUREMODE_STRETCH;
			placeCursor->visuals.Color.x = nextTile.visuals.Color.x;
			placeCursor->visuals.Color.y = nextTile.visuals.Color.y;
			placeCursor->visuals.Color.z = nextTile.visuals.Color.z;
			placeCursor->visuals.Opacity = nextTile.visuals.Opacity;
			updateGizmoVisuals(index);
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
				for (int i = selection.size() - 1; i >= 0; i--) // loop through every selected tile to find a match
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
			{
				remove_tile(index);
			}
			break;
		}
		case BOX:
		{
			dragBegin = program.camera.screen_to_world(toolPos);
			int index;
			Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
			placeCursor->visuals.atlasLocation = nextTile.visuals.atlasLocation;
			placeCursor->visuals.textureName = nextTile.visuals.textureName;
			placeCursor->visuals.TextureMode = TEXTUREMODE_STRETCH;
			placeCursor->visuals.Color.x = nextTile.visuals.Color.x;
			placeCursor->visuals.Color.y = nextTile.visuals.Color.y;
			placeCursor->visuals.Color.z = nextTile.visuals.Color.z;
			placeCursor->visuals.Opacity = nextTile.visuals.Opacity;
			updateGizmoVisuals(index);
			break;
		}
	};
}

void Editor::update_atlas_coords(TextureAtlas* atlas)
{
	std::map<std::string, glm::uvec4> updatedAtlasLocations = {};

	for (int i = 0; i < tiles.size(); i++)
	{
		glm::uvec4 atLocation;
		if (updatedAtlasLocations.find(tiles[i].visuals.textureName) == updatedAtlasLocations.end())
		{
			atLocation = program.textureLoader.getAtlasTextureCoords(atlas, tiles[i].visuals.textureName);
			updatedAtlasLocations[tiles[i].visuals.textureName] = atLocation;
		}
		else
		{
			atLocation = updatedAtlasLocations[tiles[i].visuals.textureName];
		}

		tiles[i].visuals.atlasLocation = atLocation;
		program.render.instanceAtlasData[i].x = atLocation.x;
		program.render.instanceAtlasData[i].y = atLocation.y;
		program.render.instanceAtlasData[i].z = atLocation.z;
		program.render.instanceAtlasData[i].w = atLocation.w;
	}

	for (int i = 0; i < gizmos.size(); i++)
	{
		glm::uvec4 atLocation;
		if (updatedAtlasLocations.find(gizmos[i].visuals.textureName) == updatedAtlasLocations.end())
		{
			atLocation = program.textureLoader.getAtlasTextureCoords(atlas, gizmos[i].visuals.textureName);
			updatedAtlasLocations[gizmos[i].visuals.textureName] = atLocation;
		}
		else
		{
			atLocation = updatedAtlasLocations[tiles[i].visuals.textureName];
		}
		std::cout << "Gizmo textureName: " << gizmos[i].visuals.textureName << std::endl;

		gizmos[i].visuals.atlasLocation = atLocation;
		program.render.GinstanceAtlasData[i].x = atLocation.x;
		program.render.GinstanceAtlasData[i].y = atLocation.y;
		program.render.GinstanceAtlasData[i].z = atLocation.z;
		program.render.GinstanceAtlasData[i].w = atLocation.w;
	}

	// update nextTile too...
	// std::cout << nextTile.visuals.textureName << std::endl;
	if (program.gui.tileTextures.size() > 0)
	{
		glm::uvec4 atLocation = program.textureLoader.getAtlasTextureCoords(atlas, nextTile.visuals.textureName);
		// when the nextTile has no texture, use the first one in the texture selector gui
		nextTile.visuals.textureName = program.gui.tileTextures[0]->path;
		nextTile.visuals.atlasLocation = atLocation == glm::uvec4(0) ? program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, nextTile.visuals.textureName) : atLocation;
		nextTile.visuals.atlasLocation = program.textureLoader.getAtlasTextureCoords(atlas, nextTile.visuals.textureName);
	}

	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_3);
	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_3);

	if (gridGizmoID == -1)
	{
		// only show grid if a grid texture was provided
		if (std::find(program.render.textureAtlas->textureFiles.begin(), program.render.textureAtlas->textureFiles.end(), "grid_blue.png") != program.render.textureAtlas->textureFiles.end())
		{
			Gizmo &gridGizmo = gizmos.emplace_back(
				Location(glm::vec4(-1000.0f, -1000.0f, 0.0f, 0.0f), glm::vec3(2000.0f, 2000.0f, 0.0f)),
				Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, "grid_blue.png"), "grid_blue.png", TEXTUREMODE_TILE, glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.75f),
				GizmoType_Indicator,
				/*_permanent*/ true
			);

			gridGizmoID = gridGizmo.ID;
			add_gizmo(gridGizmo); 
		}
	}
	if (placeCursorID == -1)
	{
		Gizmo& placeCursor = gizmos.emplace_back(
			Location(program.camera.screen_to_world(toolPos), nextTile.location.Size), 
			nextTile.visuals, 
			GizmoType_Indicator,
			/*_permanent*/ true
		);

		placeCursor.visuals.Opacity = 0;
		placeCursorID = placeCursor.ID;
		add_gizmo(placeCursor);
	}
	if (dragGizmoID == -1)
	{
		Gizmo &dragGizmo = gizmos.emplace_back(
			Location(glm::vec4(0.0f), glm::vec3(0.0f)),
			Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, "drag_blue.png"), "drag_blue.png", TEXTUREMODE_TILE, glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.75f),
			GizmoType_Indicator,
			/*_permanent*/ true
		);

		dragGizmoID = dragGizmo.ID;
		add_gizmo(dragGizmo); 
	}
	for (int i = 0; i < sizeof(scaleDraggerGizmoIDs) / sizeof(int); i++)
	{
		if (scaleDraggerGizmoIDs[i] == -1)
		{
			Gizmo &scaleDraggerGizmo = gizmos.emplace_back(
				Location(glm::vec4(0.0f), glm::vec3(0.2f)),
				Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, "dragger_blue.png"), "dragger_blue.png", TEXTUREMODE_STRETCH, glm::vec2(1.0f, 1.0f), scaleDraggerColor, 0.75f),
				GizmoType_ResizeDragger,
				/*_permanent*/ true
			);

			scaleDraggerGizmoIDs[i] = scaleDraggerGizmo.ID;
			add_gizmo(scaleDraggerGizmo); 
		}
	}

	const char* moveDraggerTextures[3] = {
		// "dragger_move_horizontal_blue.png",
		// "dragger_move_vertical_blue.png",
		// "dragger_move_biaxial_blue.png",
		"dragger_blue.png",
		"dragger_blue.png",
		"dragger_blue.png",
	};

	for (int i = 0; i < sizeof(moveDraggerGizmoIDs) / sizeof(int); i++)
	{
		if (moveDraggerGizmoIDs[i] == -1)
		{
			Gizmo &moveDraggerGizmo = gizmos.emplace_back(
				Location(glm::vec4(0.0f), glm::vec3(0.2f)),
				Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, moveDraggerTextures[i]), moveDraggerTextures[i], TEXTUREMODE_STRETCH, glm::vec2(1.0f, 1.0f), moveDraggerColors[i], 0.75f),
				GizmoType_MoveDragger,
				/*_permanent*/ true
			);

			moveDraggerGizmoIDs[i] = moveDraggerGizmo.ID;
			add_gizmo(moveDraggerGizmo); 
		}
	}
}

void Editor::moveTile(int index, glm::vec2 newPos)
{
	// snap position to the grid if needed
	if (autosnap)
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
	if (autosnap)
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
	program.render.instanceTextureData[index].x = visuals->TextureSize.x;
	program.render.instanceTextureData[index].y = visuals->TextureSize.y;
	program.render.instanceAtlasData[index].x = visuals->atlasLocation.x;
	program.render.instanceAtlasData[index].y = visuals->atlasLocation.y;
	program.render.instanceAtlasData[index].z = visuals->atlasLocation.z;
	program.render.instanceAtlasData[index].w = visuals->atlasLocation.w;
	program.render.instanceColorData[index].x = visuals->Color.x;
	program.render.instanceColorData[index].y = visuals->Color.y;
	program.render.instanceColorData[index].z = visuals->Color.z;
	program.render.instanceColorData[index].w = visuals->Opacity;
	program.render.instanceAdditionalData[index].x = visuals->TextureMode == TEXTUREMODE_TILE;

	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_ALL);

	setDirtiness(true);
}

void Editor::updateTileVisuals(unsigned int ID)
{
	int index = 0;
	E_Tile* tile = ID_to_tile(ID, index);
	updateTileVisuals(index);
}

/// GIZMOS
void Editor::moveGizmo(int index, glm::vec2 newPos, bool snappable)
{
	// snap position to the grid if needed
	if (autosnap && snappable)
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

void Editor::moveGizmo(unsigned int ID, glm::vec2 newPos, bool snappable)
{
	int index = 0;
	ID_to_gizmo(ID, index);
	moveGizmo(index, newPos, snappable);
}

void Editor::resizeGizmo(int index, glm::vec2 newSize, bool snappable)
{
	// snap even the size to the grid if needed
	if (autosnap && snappable)
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

void Editor::resizeGizmo(unsigned int ID, glm::vec2 newSize, bool snappable)
{
	int index = 0;
	ID_to_gizmo(ID, index);
	resizeGizmo(index, newSize, snappable);
}

void Editor::updateGizmoVisuals(int index)
{
	Visuals* visuals = &gizmos[index].visuals;
	program.render.GinstanceTextureData[index].x = visuals->TextureSize.x;
	program.render.GinstanceTextureData[index].y = visuals->TextureSize.y;
	program.render.GinstanceAtlasData[index].x = visuals->atlasLocation.x;
	program.render.GinstanceAtlasData[index].y = visuals->atlasLocation.y;
	program.render.GinstanceAtlasData[index].z = visuals->atlasLocation.z;
	program.render.GinstanceAtlasData[index].w = visuals->atlasLocation.w;
	program.render.GinstanceColorData[index].x = visuals->Color.x;
	program.render.GinstanceColorData[index].y = visuals->Color.y;
	program.render.GinstanceColorData[index].z = visuals->Color.z;
	program.render.GinstanceColorData[index].w = visuals->Opacity;
	program.render.GinstanceAdditionalData[index].x = visuals->TextureMode == TEXTUREMODE_TILE;

	program.render.updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE_ALL);
}

void Editor::updateGizmoVisuals(unsigned int ID)
{
	int index = 0;
	ID_to_gizmo(ID, index);
	updateGizmoVisuals(index);
}

void Editor::update_gizmos()
{
	// TODO: i could also just cache their indices and not get them every frame, since all the ones before them and them included are permanent
	// resize dragger gizmos
	if (selectedTool == SELECT && selection.size() == 1)
	{
		E_Tile* tile = selection[0];

		int scaleDraggerIndices[4] = { 0, 0, 0, 0 };

		for (int i = 0; i < sizeof(scaleDraggerGizmoIDs) / sizeof(int); i++)
		{
			Gizmo* scaleDragger = ID_to_gizmo(scaleDraggerGizmoIDs[i], scaleDraggerIndices[i]);
			if (scaleDragger->visuals.Opacity < 0.1f)
			{
				scaleDragger->visuals.Opacity = 0.75f;
				updateGizmoVisuals(scaleDraggerIndices[i]);
			}
		}

		// TL
		moveGizmo(scaleDraggerIndices[0], glm::vec3(tile->location.Position) + glm::vec3(0.0f, tile->location.Size.y, 0.0f) - gizmos[scaleDraggerIndices[0]].location.Size / 2.0f);		
		// TR
		moveGizmo(scaleDraggerIndices[1], glm::vec3(tile->location.Position) + tile->location.Size - gizmos[scaleDraggerIndices[0]].location.Size / 2.0f);		
		// BL
		moveGizmo(scaleDraggerIndices[2], glm::vec3(tile->location.Position) - gizmos[scaleDraggerIndices[0]].location.Size / 2.0f);		
		// BR
		moveGizmo(scaleDraggerIndices[3], glm::vec3(tile->location.Position) + glm::vec3(tile->location.Size.x, 0.0f, 0.0f) - gizmos[scaleDraggerIndices[0]].location.Size / 2.0f);

		int moveDraggerIndices[3] = { 0, 0, 0 };

		for (int i = 0; i < sizeof(moveDraggerGizmoIDs) / sizeof(int); i++)
		{
			Gizmo* moveDragger = ID_to_gizmo(moveDraggerGizmoIDs[i], moveDraggerIndices[i]);
			if (moveDragger->visuals.Opacity < 0.1f)
			{
				moveDragger->visuals.Opacity = 0.75f;
				updateGizmoVisuals(moveDraggerIndices[i]);
			}
		}

		// RIGHT CENTER (HORIZONTAL)
		moveGizmo(moveDraggerIndices[0], glm::vec3(tile->location.Position) + glm::vec3(tile->location.Size.x, tile->location.Size.y / 2.0f, 0.0f) - gizmos[scaleDraggerIndices[1]].location.Size / 2.0f);		
		// TOP CENTER (VERTICAL)
		moveGizmo(moveDraggerIndices[1], glm::vec3(tile->location.Position) + glm::vec3(tile->location.Size.x / 2.0f, tile->location.Size.y, 0.0f) - gizmos[scaleDraggerIndices[0]].location.Size / 2.0f);		
		// MIDDLE (BIAXIAL)
		moveGizmo(moveDraggerIndices[2], glm::vec3(tile->location.Position) + tile->location.Size / 2.0f - gizmos[scaleDraggerIndices[0]].location.Size / 2.0f);		
	}
	// hide dragger gizmos
	else
	{
		for (int i = 0; i < sizeof(scaleDraggerGizmoIDs) / sizeof(int); i++)
		{
			if (scaleDraggerGizmoIDs[i] != -1)
			{
				int index;
				Gizmo* scaleDragger = ID_to_gizmo(scaleDraggerGizmoIDs[i], index);
				scaleDragger->visuals.Opacity = 0.0f;
				updateGizmoVisuals(index);
			}
		}
		for (int i = 0; i < sizeof(moveDraggerGizmoIDs) / sizeof(int); i++)
		{
			if (moveDraggerGizmoIDs[i] != -1)
			{
				int index;
				Gizmo* moveDragger = ID_to_gizmo(moveDraggerGizmoIDs[i], index);
				moveDragger->visuals.Opacity = 0.0f;
				updateGizmoVisuals(index);
			}
		}
	}
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
