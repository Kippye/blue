#pragma once

#include <util.h>
#include <textures.h>
#include <editor_tile.h>
#include <gizmo.h>
#include <glm.hpp>
#include <vector>
#include <deque>
#include <map>
#include <string.h>

class E_Tile;
class Gizmo;

enum Tool
{
    SELECT, // select placed tiles by clicking, dragging, or shift clicking (multiple selection)
    PLACE, // place copies of current nextTile or remove single tiles
	BOX // box draw tiles or a single tile
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
		glm::vec4 scaleDraggerColor = glm::vec4(1.0f, 0.55f, 0.0f, 1.0f);
		glm::vec4 moveDraggerColors[3] = {
			glm::vec4(0.25f, 0.25f, 1.0f, 1.0f),
			glm::vec4(0.25f, 1.0f, 0.25f, 1.0f),
			glm::vec4(1.0f, 0.25f, 0.25f, 1.0f)
		};
		// TL, TR, BL, BR
		int scaleDraggerGizmoIDs[4] = { -1, -1, -1, -1 };
		int moveDraggerGizmoIDs[3] = { -1, -1, -1 };
		const float minBoxDrawSize = 0.2f;
		const float minScaleSize = 0.1f;
    public:
		// editor data
        std::deque<E_Tile> tiles = {};
		std::vector<Gizmo> gizmos = {};
		std::vector<TileOptions> copyBuffer = {};
		int placeCursorID = -1;
		int gridGizmoID = -1;
		int dragGizmoID = -1;
		int activeDraggerID = -1;

		std::vector<std::string> tags = std::vector(MAX_TAGS, std::string("NA"));
		// tool data
        glm::vec2& toolPos = glm::vec2(0.0f);
		glm::vec2 dragBegin = glm::vec2(0.0f);
		std::vector<E_Tile*> lastSelectionArea = {};
		bool overlap = true;
		bool autosnap = false;
		bool gridVisible = true;
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
		void set_grid_visible(bool to);
		void update_grid_size(float newSize);

		// utility functions
		bool checkForOverlaps(Bounding_Box &box, glm::vec4 &pos);
		E_Tile* positionToTile(glm::vec4 &pos, int &index);
		E_Tile* ID_to_tile(int ID, int &index);
		Gizmo* positionToGizmo(glm::vec4 &pos, int &index, GizmoType acceptedTypes);
		Gizmo* ID_to_gizmo(int ID, int &index);
		int ID_to_gizmo_index(int ID);
		std::vector<E_Tile*>* getTilesInArea(Bounding_Box area, glm::vec4 &pos, std::vector<int> &indices);

	private:
		void Editor::update_tile_selection(int index, bool to);
		void Editor::update_tile_selection(unsigned int ID, bool to);
	public:
		void select_by_texture(std::string textureName);
		void push_selection_to_back();
		void select_all();
		void deselect_all();
		void delete_selection();
		void delete_all();
		void copy_selection();
		void cut_selection();
		void paste();
		void undo();
		void redo();

		void clear_tags();

		void reset_next_tile();

		// tool functions
        void tool_use();
		void end_tool_drag(int mouseButton);
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
		void moveGizmo(int index, glm::vec2 newPos, bool snappable = false);
        void moveGizmo(unsigned int ID, glm::vec2 newPos, bool snappable = false);
		void resizeGizmo(int index, glm::vec2 newSize, bool snappable = false);
		void resizeGizmo(unsigned int ID, glm::vec2 newSize, bool snappable = false);
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
