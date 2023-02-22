#pragma once

#include <util.h>
#include <textures.h>
#include <editor_tile.h>
#include <gizmo.h>
#include <glm.hpp>
#include <vector>
#include <map>
#include <string.h>

class E_Tile;
class Gizmo;

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
		bool tags[MAX_TAGS] = { false, false, false, false, false };
};

const TileOptions DEFAULT_TILE = TileOptions();

class Editor
{
	private:
        Tool selectedTool = SELECT;
		bool dirty = false; // are there any unsaved changes?
    public:
		// editor data
        std::vector<E_Tile> tiles = {};
		std::vector<Gizmo> gizmos = {};
		int placeCursorID = -1;
		int gridGizmoID = -1;
		std::vector<std::string> tags = std::vector(MAX_TAGS, std::string("NA"));
		// tool data
        glm::vec2& toolPos = glm::vec2(0.0f);
        glm::vec2 cachedToolPos = glm::vec2(0.0f);
		bool overlap = true;
		bool autosnap = false;
		// editor settings
		glm::vec3 backgroundColor = glm::vec3(0.2f, 0.2f, 0.8f);

		std::vector<E_Tile*> selection = {};
		TileOptions nextTile;

		Editor();

		// properties
		Tool getTool();
		void setTool(Tool);
		bool getAutosnap();
		void setAutosnap(bool);
		bool getDirtiness();
		void setDirtiness(bool);

		// utility functions
		bool checkForOverlaps(Bounding_Box &box, glm::vec4 &pos);
		E_Tile* positionToTile(glm::vec4 &pos, int &index, bool grid = false);
		E_Tile* ID_to_tile(int ID, int &index);
		Gizmo* ID_to_gizmo(int ID, int &index);
		int ID_to_gizmo_index(int ID);
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

		void clear_tags();

		void reset_next_tile();

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
		void rotateTile(int index, double newRotation);
		void rotateTile(unsigned int ID, double newRotation);
		// update a tile's visual instance data
		void updateTileVisuals(int index);
		// update a tile's visual instance data
		void updateTileVisuals(unsigned int ID);
		// GIZMOS
		void moveGizmo(int index, glm::vec2 newPos);
        void moveGizmo(unsigned int ID, glm::vec2 newPos);
		void resizeGizmo(int index, glm::vec2 newSize);
		void resizeGizmo(unsigned int ID, glm::vec2 newSize);
		// update a gizmo's visual instance data
		void updateGizmoVisuals(int index);
		// update a gizmo's visual instance data
		void updateGizmoVisuals(unsigned int ID);

		void update_gizmos();

        void add_tile(E_Tile &tile);
        void add_tile(std::vector<E_Tile> &tiles);
        void remove_tile(int index = -1);
        void remove_tile(std::vector<int> &indices);

		void add_gizmo(Gizmo &gizmo);
        void remove_gizmo(int index = -1);
};
