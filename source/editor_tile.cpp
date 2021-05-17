#include <editor_tile.h>

Editor_Tile::Editor_Tile(){}

Editor_Tile::Editor_Tile(Transforms trans){ transforms = trans; }

Editor_Tile::Editor_Tile(Visuals visual){ visuals = visual; }

Editor_Tile::Editor_Tile(Transforms trans, Visuals visual)
{
	transforms = trans;
	visuals = visual;
}

Transforms::Transforms(){}
Transforms::Transforms(glm::vec3 _Position) { Position = _Position; }

Visuals::Visuals(){}
Visuals::Visuals(bool _Visible, glm::vec2 _atlasCoords) { Visible = _Visible; atlasCoords = _atlasCoords; }
//Physics::Physics(bool _CollisionsEnabled = true, bool _Static) { CollisionsEnabled = _CollisionsEnabled; Static = _Static; }
