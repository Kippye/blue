#pragma once

#include <textures.h>
#include <editor_tile.h>
#include <glm.hpp>
#include <vector>
#include <map>

class E_Tile;

enum Tool
{
    SELECT, // select placed tiles by clicking, dragging, or ctrl clicking (multiple selection)
    PLACE, // place currently selected tile (only necessary after switching to another tool)
    MOVE // moves current selection, can't be selected if nothing is selected
};

class Editor
{
	private:
        Tool selectedTool = SELECT;
        //std::map<float[4], Tile*> LocationToTile = {};
    public:
		// editor data
        std::vector<E_Tile> tiles = {};
		// tool data
        glm::vec2& toolPos = glm::vec2(0.0f);

		std::vector<E_Tile*> selection = {};
		glm::vec2 placementTileTexture;

		// tool getter / setter
		Tool getTool();
		void setTool(Tool);

		Editor();

		// utility functions
		E_Tile* positionToTile(glm::vec4 &pos, int &index, bool grid = false);
		std::vector<E_Tile*>& getTilesInArea(Bounding_Box area, std::vector<int> &indices);

		// tool functions
        void tool_use();
        void tool_use_secondary();
        void updateToolPos(glm::vec2 &mousePos);

        void add_tile(E_Tile &tile);
        void remove_tile(E_Tile &tile, int index = -1);
};
