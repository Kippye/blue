#pragma once

#include <textures.h>
#include <editor_tile.h>
#include <glm.hpp>
#include <vector>
#include <map>

class Tile;

enum Tool
{
    Select, // select placed tiles by clicking, dragging, or ctrl clicking (multiple selection)
    Place, // place currently selected tile (only necessary after switching to another tool)
    Move // moves current selection, can't be selected if nothing is selected
    //Remove, // simple remove tool allowing for single click or drag removing of tiles
};

class Editor
{
	private:
        Tool selectedTool = Select;
        //std::map<float[4], Tile*> LocationToTile = {};
    public:
		// editor data
        std::vector<Tile> tiles = {};
		// tool data
        glm::vec2& toolPos = glm::vec2(0.0f);

		std::vector<Tile> selection = {};
		glm::vec2 placementTileTexture;

		// tool getter / setter
		Tool getTool();
		void setTool(Tool);

		Editor();

		// utility functions
		Tile* positionToTile(glm::vec4 &pos);

		// tool functions
        void tool_use();
        void tool_use_secondary();
        void updateToolPos(glm::vec2 &mousePos);

        void add_tile(Tile &tile);
        void remove_tile(Tile &tile, int index = -1);
};
