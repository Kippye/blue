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
	// having things selected while using the place tool could mess things up big time, that hasn't been accounted for (yet) nor is it that useful
	if (selection.size() > 0 && tool != SELECT)
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

	if ((selectedTool == SELECT && tool != SELECT) || (selectedTool == BOX && tool != BOX))
	{
		if (program.input.lmb_down)
		{
			end_tool_drag(GLFW_MOUSE_BUTTON_LEFT);
		}
		if (program.input.rmb_down)
		{
			end_tool_drag(GLFW_MOUSE_BUTTON_RIGHT);
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

void Editor::set_grid_visible(bool to)
{
	// unnecessary safety check check
	if (program.editor.gridGizmoID != -1 && to != gridVisible)
	{
		int index;
		Gizmo* gridGizmo = program.editor.ID_to_gizmo(program.editor.gridGizmoID, index);
		gridGizmo->visuals.Opacity = to ? 0.75f : 0.0f;
		program.editor.updateGizmoVisuals(index);

		gridVisible = to;
	}
}

void Editor::set_perspective_enabled(bool to)
{
	perspectiveEnabled = to;
}

void Editor::update_grid_size(float newSize)
{
	mymath::gridSize = newSize;

	int index;
	Gizmo* gridGizmo = program.editor.ID_to_gizmo(program.editor.gridGizmoID, index);
	gridGizmo->visuals.TextureSize = glm::vec2(newSize);
	program.editor.updateGizmoVisuals(index);
}

bool Editor::checkForOverlaps(const Bounding_Box &box, const glm::vec4 &pos)
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

E_Tile* Editor::positionToTile(const glm::vec4 &pos, int &index)
{
	E_Tile* closestTile = nullptr;
	int closestTileIndex = 0;
	float closestTileZ = 0.0f;

	// reverse loop so we get the top-most tile
	for (index = tiles.size() - 1; index >= 0; index--)
	{
		if (tiles[index].location.box.contains_position(tiles[index].location.Position, pos))
		{
			if (closestTile == nullptr || tiles[index].location.Position.z > closestTileZ)
			{
				closestTile = &tiles.at(index);
				closestTileIndex = index;
				closestTileZ = tiles[index].location.Position.z;
			}
		}
	}
	index = closestTileIndex;
	return closestTile;
}

E_Tile* Editor::ID_to_tile(int ID, int &index)
{
	// reverse loop so we get the top-most tile
	for (index = tiles.size() - 1; index >= 0; index--)
	{
		if (tiles[index].ID == ID)
		{
			return &tiles[index];
		}
	}
	return nullptr;
}

int Editor::ID_to_tile_index(int ID)
{
	assert(tileIndices.find(ID) != tileIndices.end());

	return tileIndices[ID];
}

Gizmo* Editor::positionToGizmo(const glm::vec4 &pos, int &index, GizmoType acceptedTypes)
{
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

// returns the IDs of tiles in an area, also fills the given indices array with their indices
std::vector<unsigned int>* Editor::getTilesInArea(Bounding_Box area, glm::vec4 &pos, std::vector<int> &indices)
{
	std::vector<unsigned int>* tilesInArea = new std::vector<unsigned int>();
	for (int i = tiles.size() - 1; i >= 0; i--)
	{
		if (area.contains(pos, tiles[i].location.Position, tiles[i].location.box))
		{
			indices.push_back(i);
			tilesInArea->push_back(tiles[i].ID);
		}
	}

	return tilesInArea;
}

void Editor::update_tile_selection(int index, bool to)
{
	tiles[index].selected = to;
	program.render.set_tile_selection(index, to);
}

void Editor::update_tile_selection(unsigned int ID, bool to)
{
	update_tile_selection(ID_to_tile_index(ID), to);
}

void Editor::select_by_texture(std::string textureName)
{
	deselect_all();

	std::vector<int> selectedTileIndices = {};

	for (int i = tiles.size() - 1; i >= 0; i--)
	{
		if (tiles[i].visuals.textureName == textureName)
		{
			tiles[i].selected = true;
			selectedTileIndices.push_back(i);
			selection.push_back(&tiles[i]);
		}
	}

	program.render.set_tile_selection(selectedTileIndices, true);
}

void Editor::select_all()
{
	if (tiles.size() == 0) { return; }

	std::vector<int> indices = {};

	for (int i = 0; i < tiles.size(); i++)
	{
		if (tiles[i].selected == false)
		{
			indices.push_back(i);
			tiles[i].selected = true;
			selection.push_back(&tiles[i]);
		}
	}
	program.render.set_tile_selection(indices, true);
}

void Editor::deselect_all()
{
	if (selection.size() == 0) { return; } // don't waste time if we've got nothing to find

	std::vector<int> indices = {};

	for (int i = 0; i < selection.size(); i++)
	{
		selection[i]->selected = false;
		indices.push_back(ID_to_tile_index(selection[i]->ID));
	}

	selection.clear();
	program.render.set_tile_selection(indices, false);
}

// uses a premade vector of the tile indices of all selected tiles NOTE: the tiles deque must NOT be modified before calling the function
void Editor::deselect_all(std::vector<int>& indices)
{
	if (indices.size() == 0) { return; }

	for (int i = 0; i < indices.size(); i++)
	{
		tiles[indices[i]].selected = false;
	}

	selection.clear();
	program.render.set_tile_selection(indices, false);
}

void Editor::delete_all()
{
	remove_tile_all();
}

void Editor::delete_selection()
{
	if (selection.size() == 0) { return; }
	if (selection.size() == tiles.size()) { lastSelectionArea.clear(); remove_tile_all(); return; }

	std::vector<int> indices = {};
	std::unordered_set<int> indexSet = {};

	for (int i = 0; i < selection.size(); i++)
	{
		int index = ID_to_tile_index(selection[i]->ID);
		indices.push_back(index);
		indexSet.insert(index);
	}

	lastSelectionArea.clear();
	deselect_all(indices);

	remove_tile(indices, indexSet);

	setDirtiness(true);
}

void Editor::copy_selection()
{
	if (selection.size() == 0)
	{
		return;
	}
	copyBuffer.clear();

	glm::vec2 lowestPos = glm::vec2(selection[0]->location.Position.x, selection[0]->location.Position.y);

	for (int i = 0; i < selection.size(); i++)
	{
		if (selection[i]->location.Position.x < lowestPos.x)
		{
			lowestPos.x = selection[i]->location.Position.x;
		}
		if (selection[i]->location.Position.y < lowestPos.y)
		{
			lowestPos.y = selection[i]->location.Position.y;
		}
	}

	for (int i = 0; i < selection.size(); i++)
	{
		TileOptions tileCopyInfo;
		tileCopyInfo.location = selection[i]->location;
		tileCopyInfo.location.Position.x = tileCopyInfo.location.Position.x - lowestPos.x;
		tileCopyInfo.location.Position.y = tileCopyInfo.location.Position.y - lowestPos.y;
		tileCopyInfo.physics = selection[i]->physics;
		tileCopyInfo.visuals = selection[i]->visuals;
		for (int j = 0; j < MAX_TAGS; j++)
		{
			tileCopyInfo.tags[j] = selection[i]->tags[j];
		}
		copyBuffer.push_back(tileCopyInfo);
	}
}

void Editor::cut_selection()
{
	// not the most optimized (many more loops), but definitely the most clean way to do this
	copy_selection();
	delete_selection();
}

// TODO: add a new overload of add_tile that accepts a vector of TileOptions to speed this up significantly
void Editor::paste()
{
	glm::vec4 mouseWorldPos = program.camera.screen_to_world(program.input.mousePos);
	// i made both autosnap and holding shift down when pasting (to just make it easier) snap the start position to the grid
	mouseWorldPos = autosnap || program.input.shift_down ? mymath::floor_to_grid(mouseWorldPos) : mouseWorldPos;

	for (int i = 0; i < copyBuffer.size(); i++)
	{
		// positions in the copy buffer tiles are stored as offsets from 0; 0 (bottom left corner of the copy buffer tiles)
		add_tile(tiles.emplace_back(
			Location(mouseWorldPos + copyBuffer[i].location.Position, copyBuffer[i].location.Size, copyBuffer[i].location.Angle), 
			copyBuffer[i].physics, 
			copyBuffer[i].visuals, 
			copyBuffer[i].tags
		));
	}
}

// TODO Implement
void Editor::undo()
{
}

// TODO Implement
void Editor::redo()
{
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
	nextTile.location.Position.z = 0.0f;
	
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
		if (selectedTool == PLACE)
		{
			resizeGizmo(index, nextTile.location.Size);
		}
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

			if (selection.size() == 1 && activeDraggerID != -1 /* NOTE: might cause issues && lastSelectionArea.size() == 0 */)
			{
				bool activeDraggerFound = false;
				int tIndex = ID_to_tile_index(selection[0]->ID);
				E_Tile* tile = &tiles[tIndex];

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
								moveTile(tIndex, glm::vec3(newX, tile->location.Position.y, tile->location.Position.z));
								activeDraggerFound = true;
								break;
							}
							// TOP CENTER DRAGGER (VERTICAL)
							case 1:
							{
								float newY = program.input.ctrl_down || autosnap ? mymath::round_to_grid(worldMousePos.y - tile->location.Size.y) : worldMousePos.y - tile->location.Size.y;
								moveTile(tIndex, glm::vec3(tile->location.Position.x, newY,  tile->location.Position.z));
								activeDraggerFound = true;
								break;
							}
							// CENTER DRAGGER (BIAXIAL)
							case 2:
							{
								glm::vec2 newPosition = glm::vec3(worldMousePos) - tile->location.Size / 2.0f;
								newPosition = program.input.ctrl_down || autosnap ? mymath::round_to_grid(newPosition) : newPosition;
								moveTile(tIndex, glm::vec3(newPosition.x, newPosition.y, tile->location.Position.z));
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
			std::vector<unsigned int>* tileIDsInArea = getTilesInArea(Bounding_Box(areaSize), boundingBoxPos, indices);

			// TODO: could do a funny optimization here where if the new area's size is bigger than the last one's (in the same direction) then obviously all previously selected tiles are still in it

			std::vector<int> deselectedTileIndices = {};

			// deselect tiles that were in the last selection area but not in the new one
			for (unsigned int oldTileID : lastSelectionArea)
			{
				bool isInNewArea = false;

				for (unsigned int newTileID : *tileIDsInArea)
				{
					if (oldTileID == newTileID)
					{
						isInNewArea = true;
						break;
					}
				}

				if (isInNewArea == false)
				{
					for (int i = selection.size() - 1; i >= 0; i--) // loop through every selected tile to find a match
					{
						if (selection[i]->ID == oldTileID) // this previously selected tile is no longer selected
						{
							selection[i]->selected = false;
							selection.erase(selection.begin() + i); // remove deselected tile from selection
							break;
						}
					}

					deselectedTileIndices.push_back(ID_to_tile_index(oldTileID));
				}
			}

			program.render.set_tile_selection(deselectedTileIndices, false);

			// select tiles in the area that weren't selected before
			for (int tileIndex : indices)
			{
				E_Tile* selectedTile = &tiles[tileIndex];	
				if (selectedTile->selected == false)
				{
					selectedTile->selected = true;
					selection.push_back(selectedTile);
				}
			}

			program.render.set_tile_selection(indices, true);

			lastSelectionArea = *tileIDsInArea;
		}
		// drag ended
		else if (program.input.lmb_down == false && program.input.lmb_down_last == true)
		{
			end_tool_drag(GLFW_MOUSE_BUTTON_LEFT);
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
			if (program.input.ctrl_down || autosnap)
			{
				glm::vec2 dragBeginSnapped = mymath::floor_to_grid(dragBegin); 
				startPos.x = startPos.x < dragBeginSnapped.x ? mymath::floor_to_grid(startPos.x) : dragBeginSnapped.x;
				areaSize.x = startPos.x < dragBeginSnapped.x ? mymath::ceil_to_grid(dragBeginSnapped.x - startPos.x) : mymath::ceil_to_grid(areaSize.x);
				startPos.y = startPos.y < dragBeginSnapped.y ? mymath::floor_to_grid(startPos.y) : dragBeginSnapped.y;
				areaSize.y = startPos.y < dragBeginSnapped.y ? mymath::ceil_to_grid(dragBeginSnapped.y - startPos.y) : mymath::ceil_to_grid(areaSize.y);
			}

			float minSize = program.input.ctrl_down || autosnap ? 1.0f : minBoxDrawSize;

			if (startPos.x < (program.input.ctrl_down || autosnap ? mymath::round_to_grid(dragBegin.x) : dragBegin.x))
			{
				startPos.x = dragBegin.x - startPos.x < minSize ? dragBegin.x - minSize : startPos.x;
			}
			if (startPos.y < (program.input.ctrl_down || autosnap ? mymath::round_to_grid(dragBegin.y) : dragBegin.y))
			{
				startPos.y = dragBegin.y - startPos.y < minSize ? dragBegin.y - minSize : startPos.y;
			}

			// still dragging
			if (program.input.lmb_down)
			{
				// update the gizmo
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);

				moveGizmo(index, startPos);
				resizeGizmo(index, glm::max(areaSize, glm::vec2(minSize)));
			}
			// just released lmb, draw the tile
			else
			{
				if (abs(dragBegin.x - worldMousePos.x) > minBoxDrawSize / 2.0f && abs(dragBegin.y - worldMousePos.y) > minBoxDrawSize / 2.0f)
				{
					
					// not overlapping when not supposed to, place a tile
					if (overlap == true || checkForOverlaps(Bounding_Box(areaSize), startPos))
					{
						// place the tile
						startPos.z = nextTile.location.Position.z;
						add_tile(tiles.emplace_back(Location(startPos, glm::max(glm::vec3(areaSize, 1.0f), glm::vec3(minSize, minSize, 1.0f))), nextTile.physics, nextTile.visuals, nextTile.tags));
					}
				}
				// safety thing, should have little to no effect
				program.input.lmb_down_last = false;
				end_tool_drag(GLFW_MOUSE_BUTTON_LEFT);
			}
		}
		// drawing in the secondary mode
		else if (program.input.rmb_down_last == true)
		{
			glm::vec4 worldMousePos = program.camera.screen_to_world(mousePos);

			// find the start position depending on drag start and end positions (because size be retarded like that)
			glm::vec4 startPos = glm::vec4(0.0f);
			startPos.x = std::min(worldMousePos.x, dragBegin.x);
			startPos.y = std::min(worldMousePos.y, dragBegin.y);
			glm::vec4 endPos = glm::vec4(0.0f);
			endPos.x = std::max(worldMousePos.x, dragBegin.x);
			endPos.y = std::max(worldMousePos.y, dragBegin.y);

			// apply snapping TO THE DRAG START POINT if needed
			if (program.input.ctrl_down || autosnap)
			{
				glm::vec2 dragBeginSnapped = mymath::floor_to_grid(dragBegin); 
				startPos.x = startPos.x < dragBeginSnapped.x ? startPos.x : dragBeginSnapped.x;
				startPos.y = startPos.y < dragBeginSnapped.y ? startPos.y : dragBeginSnapped.y;
				endPos.x = startPos.x < dragBeginSnapped.x ? dragBeginSnapped.x : endPos.x;
				endPos.y = startPos.y < dragBeginSnapped.y ? dragBeginSnapped.y : endPos.y;
			}

			startPos.x = startPos.x < dragBegin.x ? mymath::floor_to_grid(startPos.x, nextTile.location.Size.x) : startPos.x;
			startPos.y = startPos.y < dragBegin.y ? mymath::floor_to_grid(startPos.y, nextTile.location.Size.y) : startPos.y;

			// get area size
			glm::vec2 areaSize = glm::vec2(abs(endPos.x - startPos.x), abs(endPos.y - startPos.y));
			areaSize.x = startPos.x < dragBegin.x ? mymath::ceil_to_grid(areaSize.x, nextTile.location.Size.x) : mymath::ceil_to_grid(areaSize.x, nextTile.location.Size.x);
			areaSize.y = startPos.y < dragBegin.y ? mymath::ceil_to_grid(areaSize.y, nextTile.location.Size.y) : mymath::ceil_to_grid(areaSize.y, nextTile.location.Size.y);
			// if x < 0 then size = area snapped pos = lih dragBegin - area?

			// still dragging
			if (program.input.rmb_down)
			{
				// update the gizmo
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);

				// if nextTile is TEXTUREMODE_TILE, to get the actual size of each individual tiled texture, one has to 
				if (nextTile.visuals.TextureMode == TEXTUREMODE_TILE)
				{
					placeCursor->visuals.TextureSize.x = nextTile.visuals.TextureSize.x; /// areaSize.x; 
					placeCursor->visuals.TextureSize.y = nextTile.visuals.TextureSize.y; /// areaSize.y;
				}
				else
				{
					placeCursor->visuals.TextureSize.x = nextTile.location.Size.x * nextTile.visuals.TextureSize.x; /// areaSize.x; 
					placeCursor->visuals.TextureSize.y = nextTile.location.Size.y * nextTile.visuals.TextureSize.y; /// areaSize.y;
				}
				placeCursor->visuals.TextureMode = TEXTUREMODE_TILE;
				updateGizmoVisuals(index); 
				moveGizmo(index, startPos);
				resizeGizmo(index, areaSize);
			}
			// just released rmb, draw the tiles
			else
			{
				program.input.rmb_down_last = false;

				if (abs(dragBegin.x - worldMousePos.x) > nextTile.location.Size.x / 2.0f && abs(dragBegin.y - worldMousePos.y) > nextTile.location.Size.y / 2.0f)
				{
					std::vector<E_Tile> tilesToPlace = {};

					// TODO: when tile size is < 1, something (probably this) causes a freeze + crash
					for (float y = 0; y < areaSize.y - nextTile.location.Size.y / 2.0f; y += nextTile.location.Size.y)
					{
						for (float x = 0; x < areaSize.x - nextTile.location.Size.x / 2.0f; x += nextTile.location.Size.x)
						{
							// check for overlaps if required
							if (overlap == false)
							{
								// not overlapping
								if (!checkForOverlaps(Bounding_Box(nextTile.location.Size), startPos + glm::vec4(x, y, 0.0f, 0.0f)))
								{
									E_Tile tile = E_Tile(Location(startPos + glm::vec4(x, y, nextTile.location.Position.z, 0.0f), nextTile.location.Size), nextTile.physics, nextTile.visuals, nextTile.tags);
									tilesToPlace.push_back(tile);
								}
							}
							else
							{
								E_Tile tile = E_Tile(Location(startPos + glm::vec4(x, y, nextTile.location.Position.z, 0.0f), nextTile.location.Size), nextTile.physics, nextTile.visuals, nextTile.tags);
								tilesToPlace.push_back(tile);
							}
						}
					}

					for (int i = 0; i < tilesToPlace.size(); i++)
					{
						tileIndices[tilesToPlace[i].ID] = tiles.size() + i;
					} 

					tiles.insert(tiles.end(), tilesToPlace.begin(), tilesToPlace.end());
					add_tile(tilesToPlace);
				}
				end_tool_drag(GLFW_MOUSE_BUTTON_RIGHT);
			}
		}
	}

	if (!program.gui.guiHovered && !program.file_system.contextOpen)
	{
		toolPos = mousePos;
	}
}

// For some tools (select), handles the complete functionality of when a drag is ended, for others (box place) it's mostly visual changes
void Editor::end_tool_drag(int mouseButton)
{
	switch(selectedTool)
	{
		case SELECT:
		{
			if (mouseButton == GLFW_MOUSE_BUTTON_LEFT)
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
			else if (mouseButton == GLFW_MOUSE_BUTTON_RIGHT) {}
			break;
		}
		case PLACE:
		{
			break;
		}
		case BOX:
		{
			if (mouseButton == GLFW_MOUSE_BUTTON_LEFT)
			{
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
				resizeGizmo(index, glm::vec2(0.0f));
			}
			else if (mouseButton == GLFW_MOUSE_BUTTON_RIGHT)
			{
				int index;
				Gizmo* placeCursor = ID_to_gizmo(placeCursorID, index);
				resizeGizmo(index, glm::vec2(0.0f));
			}
			break;
		}
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
					update_tile_selection(index, true);
				}
			}
			break;
		}
		case PLACE:
		{
			// get target position
			glm::vec4 pos = program.camera.screen_to_world(toolPos);
			pos = program.input.ctrl_down || autosnap ? mymath::floor_to_grid(pos) : pos;
			// check for overlaps if required
			if (overlap == false && checkForOverlaps(Bounding_Box(glm::vec2(nextTile.location.Size)), pos))
			{
				// overlapping when not supposed to, don't place a tile
				return;
			}

			placeCursorHighlightCounter = placeCursorHighlightDuration;
			pos.z = nextTile.location.Position.z;

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
			placeCursor->visuals.TextureSize.x = nextTile.visuals.TextureSize.x; 
			placeCursor->visuals.TextureSize.y = nextTile.visuals.TextureSize.y;
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

				update_tile_selection(index, false);
				return;
			}
			// tile wasn't selected before, add to selection
			selection.push_back(tile);
			update_tile_selection(index, true);
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
			atLocation = updatedAtlasLocations[gizmos[i].visuals.textureName];
		}
		//std::cout << "Gizmo textureName: " << gizmos[i].visuals.textureName << std::endl;

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
}

void Editor::moveTile(int index, glm::vec3 newPos)
{
	// snap position to the grid if needed
	if (autosnap)
	{
		newPos = mymath::floor_to_grid(newPos);
	}

	tiles[index].location.Position.x = newPos.x;
	tiles[index].location.Position.y = newPos.y;
	tiles[index].location.Position.z = newPos.z;
	// update visuals
	program.render.instanceTransformData[index].x = newPos.x;
	program.render.instanceTransformData[index].y = newPos.y;
	program.render.instanceAdditionalData[index].z = newPos.z;
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_5);

	setDirtiness(true);
}

void Editor::moveTile(unsigned int ID, glm::vec3 newPos)
{
	moveTile(ID_to_tile_index(ID), newPos);
}

void Editor::moveSelectedTiles(glm::vec3 offset)
{
	for (E_Tile* tile : selection)
	{
		glm::vec4 newPos = tile->location.Position + glm::vec4(offset, 0.0f);
		// snap position to the grid if needed
		if (autosnap)
		{
			newPos = mymath::floor_to_grid(newPos);
		}

		tile->location.Position.x = newPos.x;
		tile->location.Position.y = newPos.y;
		tile->location.Position.z = newPos.z;

		// and here's the kicker
		int index = ID_to_tile_index(tile->ID);

		// update visuals
		program.render.instanceTransformData[index].x = newPos.x;
		program.render.instanceTransformData[index].y = newPos.y;
		program.render.instanceAdditionalData[index].z = newPos.z;
	}
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_5);
	setDirtiness(true);
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
	resizeTile(ID_to_tile_index(ID), newSize);
}

void Editor::resizeSelectedTiles(glm::vec2 newSize)
{
	for (E_Tile* tile : selection)
	{
		// snap position to the grid if needed
		if (autosnap)
		{
			newSize = mymath::floor_to_grid(newSize);
		}

		tile->location.Size.x = newSize.x;
		tile->location.Size.y = newSize.y;
		tile->location.box.update_size(newSize);

		// and here's the kicker
		int index = ID_to_tile_index(tile->ID);

		// update visuals
		program.render.instanceTransformData[index].z = newSize.x;
		program.render.instanceTransformData[index].w = newSize.y;
	}
	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);
	setDirtiness(true);
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
	rotateTile(ID_to_tile_index(ID), newRotation);
}

void Editor::rotateSelectedTiles(double newRotation)
{
	for (E_Tile* tile : selection)
	{
		tile->location.Angle = newRotation;

		// and here's the kicker
		// int index;
		// ID_to_tile(tile->ID, index);

		// update visuals
		// TODO: if i make tiles have visible rotation, i would probably also have to make their bounding boxes have rotation 💀
		// also idrk how i would make moving or resizing with gizmos work in that case. probably doable tho!
	}
	// TODO: update instance data when rotation affects rendering
	setDirtiness(true);
}

void Editor::updateTileVisuals(int index)
{
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
	updateTileVisuals(ID_to_tile_index(ID));
}

void Editor::updateSelectedTilesVisuals()
{
	for (E_Tile* tile : selection)
	{
		int index = ID_to_tile_index(tile->ID);

		Visuals& visuals = tile->visuals;
		program.render.instanceTextureData[index].x = visuals.TextureSize.x;
		program.render.instanceTextureData[index].y = visuals.TextureSize.y;
		program.render.instanceAtlasData[index].x = visuals.atlasLocation.x;
		program.render.instanceAtlasData[index].y = visuals.atlasLocation.y;
		program.render.instanceAtlasData[index].z = visuals.atlasLocation.z;
		program.render.instanceAtlasData[index].w = visuals.atlasLocation.w;
		program.render.instanceColorData[index].x = visuals.Color.x;
		program.render.instanceColorData[index].y = visuals.Color.y;
		program.render.instanceColorData[index].z = visuals.Color.z;
		program.render.instanceColorData[index].w = visuals.Opacity;
		program.render.instanceAdditionalData[index].x = visuals.TextureMode == TEXTUREMODE_TILE;
	}

	program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_ALL);

	setDirtiness(true);
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
				if (scaleDragger->visuals.Opacity > 0.0f)
				{
					scaleDragger->visuals.Opacity = 0.0f;
					updateGizmoVisuals(index);
				}
			}
		}
		for (int i = 0; i < sizeof(moveDraggerGizmoIDs) / sizeof(int); i++)
		{
			if (moveDraggerGizmoIDs[i] != -1)
			{
				int index;
				Gizmo* moveDragger = ID_to_gizmo(moveDraggerGizmoIDs[i], index);
				if (moveDragger->visuals.Opacity > 0.0f)
				{
					moveDragger->visuals.Opacity = 0.0f;
					updateGizmoVisuals(index);
				}
			}
		}
	}
}

void Editor::add_tile(E_Tile &tile)
{
	tileIndices[tile.ID] = tiles.size() - 1;
	program.render.add_to_render_list(tile);
	setDirtiness(true);
}

// NOTE: indices must be added to tileIndices before calling this function
void Editor::add_tile(std::vector<E_Tile> &tiles)
{
	if (tiles.size() > 0)
	{
		program.render.add_to_render_list(tiles);
		setDirtiness(true);
	}
}

void Editor::remove_tile(int index)
{
	// remove from the tiles vector as well
	if (index != -1)
	{
		tileIndices.erase(tiles[index].ID);

		for(auto & entry : tileIndices){
			entry.second = entry.second > index ? entry.second - 1 : entry.second;
		}

		program.render.remove_from_render_list(index);
		tiles.erase(std::begin(tiles) + index);
		setDirtiness(true);
	}
}

void Editor::remove_tile(std::vector<int> &indices, std::unordered_set<int> &indexSet)
{
	if (indices.size() == 0) { return; } // don't do nothing lol

	// sort the given indices from largest to smallest so they can be erased without affecting the indices after them
	std::sort(indices.begin(), indices.end(), std::greater<int>());

	program.render.remove_from_render_list(indices);

	// if it takes less time to recreate the entire tileIndices map, do that instead
	bool replaceAllIndices = indices.size() * (tiles.size() / 2.0f) > tileIndices.size();

	for (int index: indices)
	{
		if (replaceAllIndices == false)
		{
			for (auto & entry : tileIndices){
				entry.second = entry.second > index ? entry.second - 1 : entry.second;
			}
		}
	}

	// holy crap this is fast! thanks Benjamin Lindley from StackOverflow
	int removedCount = 0;
	int last = 0;
	for (int i = 0; i < tiles.size(); ++i, ++last)
	{
		while(indexSet.find(i) != indexSet.end())
		{
			++i;
			++removedCount;
			if (removedCount % 1000 == 0)
			{
				std::cout << "Removed " << removedCount << " tiles" << std::endl;
			}
		}
		if (i >= tiles.size()) break;

		tiles[last] = tiles[i];   
	}

	tiles.resize(last);

	if (replaceAllIndices)
	{
		tileIndices.clear();
		for (int i = 0; i < tiles.size(); i++)
		{
			tileIndices[tiles[i].ID] = i;
		}
	}

	setDirtiness(true);
}

void Editor::remove_tile_all()
{
	selection.clear();
	tileIndices.clear();
	tiles.clear();
	program.render.clear_instance_data();
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
