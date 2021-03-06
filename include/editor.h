#pragma once

#include <textures.h>
#include <editor_tile.h>
#include <glm.hpp>
#include <vector>
#include <map>

class E_Tile;

enum GRID_MODE
{
	GRID_MODE_NORMAL,
	GRID_MODE_AUTO,
	GRID_MODE_FULL
};

enum OVERLAP_MODE
{
	OVERLAP_NEVER,
	OVERLAP_FREE,
	OVERLAP_ALL
};

enum Tool
{
    SELECT, // select placed tiles by clicking, dragging, or ctrl clicking (multiple selection)
    PLACE, // place currently selected tile (only necessary after switching to another tool)
	BOX, // box draw tiles or a single tile, or delete tiles in a box area
    MOVE // moves current selection, can't be selected if nothing is selected
};

struct TileOptions
{
	public:
		Location location = Location(glm::vec4(0.0f), glm::vec3(1.0f));
		Physics physics = Physics();
		Visuals visuals = Visuals();
};

class Editor
{
	private:
        Tool selectedTool = SELECT;
        GRID_MODE gridMode = GRID_MODE_NORMAL;
		bool dirty = false; // are there any unsaved changes?
    public:
		// editor data
        std::vector<E_Tile> tiles = {};
		// tool data
        glm::vec2& toolPos = glm::vec2(0.0f);
        glm::vec2 cachedToolPos = glm::vec2(0.0f);
		OVERLAP_MODE overlapMode = OVERLAP_FREE;

		std::vector<E_Tile*> selection = {};
		TileOptions nextTile;

		Editor();

		// properties
		Tool getTool();
		void setTool(Tool);
		GRID_MODE getGridMode();
		void setGridMode(GRID_MODE);
		bool getDirtiness();
		void setDirtiness(bool);

		// utility functions
		bool checkForOverlaps(Bounding_Box &box, glm::vec4 &pos);
		E_Tile* positionToTile(glm::vec4 &pos, int &index, bool grid = false);
		E_Tile* ID_to_tile(int ID, int &index);
		std::vector<E_Tile*>* getTilesInArea(Bounding_Box area, glm::vec4 &pos, std::vector<int> &indices);

	private:
		void Editor::update_tile_selection(E_Tile* tile, int index, bool to);
		void Editor::update_tile_selection(int ID, bool to);
	public:
		void select_by_texture(std::string textureName);
		void push_selection_to_back();
		void deselect_all();
		void delete_selection();
		void delete_all();

		// tool functions
        void tool_use();
        void tool_use_secondary();
        void updateToolPos(glm::vec2 &mousePos);
		// tile manipulation functions
		void update_atlas_coords(TextureAtlas* atlas);

        void moveTile(int index, glm::vec2 newPos);
        void moveTile(unsigned int ID, glm::vec2 newPos);
        void resizeTile(int index, glm::vec2 newSize);
		void resizeTile(unsigned int ID, glm::vec2 newSize);
		void changeTileVisuals(int index, Visuals visuals);
		void changeTileVisuals(unsigned int ID, Visuals visuals);

        void add_tile(E_Tile &tile);
        void add_tile(std::vector<E_Tile> &tiles);
        void remove_tile(int index = -1);
        void remove_tile(std::vector<int> &indices);
};
