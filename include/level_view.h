#include <textures.h>
#include <editor_tile.h>
#include <glm.hpp>
#include <vector>

class Editor_Tile;

enum TOOL
{
	SELECT,
	PLACE
};

class Tool
{
	public:
		void activate();
		void deactivate();
		void use();
};

class Select : public Tool
{
	// TODO: vector selection, when delete is pressed, check if selectedToolType is SELECT, if true, delete all tiles in selection.
};

class Place : public Tool
{
	public:
		Texture* selectedTile;
};

class Level_View
{
	public:
		Select* select = new Select();
		Place* place = new Place();
		TOOL selectedToolType = SELECT;
		Tool* selectedTool = select;

		std::vector<Editor_Tile> currentTiles = {};

		void update_tool(glm::vec2 mousePos);
		void select_tool(TOOL tool);

		void use_tool();
		// TODO: some kind of editor_tile vector of every drawn vector just kept here
};
