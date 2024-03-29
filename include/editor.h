#pragma once

#include <util.h>
#include <textures.h>
#include <editor_tile.h>
#include <gizmo.h>
#include <glm.hpp>
#include <vector>
#include <deque>
#include <unordered_set>
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
		std::map<unsigned int, int> tileIndices;
    public:
		// editor data
        std::deque<E_Tile> tiles = {};
		std::vector<Gizmo> gizmos = {};
		std::vector<TileOptions> copyBuffer = {};
		int placeCursorID = -1;
		int gridGizmoID = -1;
		int dragGizmoID = -1;
		int activeDraggerID = -1;

		const float placeCursorHighlightDuration = 0.05f;
		float placeCursorHighlightCounter = 0.0f;

		std::vector<std::string> tags = std::vector(MAX_TAGS, std::string("NA"));
		// tool data
        glm::vec2 toolPos = glm::vec2(0.0f);
		glm::vec2 dragBegin = glm::vec2(0.0f);
		std::vector<unsigned int> lastSelectionArea = {};
		bool overlap = true;
		bool autosnap = false;
		bool perspectiveEnabled = true;
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
		void set_perspective_enabled(bool to);
		void update_grid_size(float newSize);

		// utility functions
		bool checkForOverlaps(const Bounding_Box &box, const glm::vec4 &pos);
		E_Tile* positionToTile(const glm::vec4 &pos, int &index);
		E_Tile* ID_to_tile(int ID, int &index);
		int ID_to_tile_index(int ID);
		Gizmo* positionToGizmo(const glm::vec4 &pos, int &index, GizmoType acceptedTypes);
		Gizmo* ID_to_gizmo(int ID, int &index);
		int ID_to_gizmo_index(int ID);
		std::vector<unsigned int>* getTilesInArea(Bounding_Box area, glm::vec4 &pos, std::vector<int> &indices);

	private:
		void update_tile_selection(int index, bool to);
		void update_tile_selection(unsigned int ID, bool to);
	public:
		void select_by_texture(std::string textureName);
		void select_all();
		void deselect_all();
		void deselect_all(std::vector<int>& indices);
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

        void moveTile(int index, glm::vec3 newPos);
        void moveTile(unsigned int ID, glm::vec3 newPos);
		void moveSelectedTiles(glm::vec3 offset);
        void resizeTile(int index, glm::vec2 newSize);
		void resizeTile(unsigned int ID, glm::vec2 newSize);
		void resizeSelectedTiles(glm::vec2 newSize);
		void rotateTile(int index, double newRotation);
		void rotateTile(unsigned int ID, double newRotation);
		void rotateSelectedTiles(double newRotation);
		// update a tile's visual instance data
		void updateTileVisuals(int index);
		// update a tile's visual instance data
		void updateTileVisuals(unsigned int ID);
		void updateSelectedTilesVisuals();
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
        void remove_tile(std::vector<int> &indices, std::unordered_set<int> &indexSet);
		void remove_tile_all();

		void add_gizmo(Gizmo &gizmo);
        void remove_gizmo(int index = -1);
};
