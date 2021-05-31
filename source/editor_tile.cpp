#include <editor_tile.h>

// empty constructor - default tile
E_Tile::E_Tile()
{
	ID = lastID;
	lastID++;
}
// constructor with transforms
E_Tile::E_Tile(Location _location)
{
	location = _location;
	ID = lastID;
	lastID++;
}

E_Tile::E_Tile(Visuals _visuals)
{
	visuals = _visuals;
	ID = lastID;
	lastID++;
}

E_Tile::E_Tile(Location _location, Visuals _visuals)
{
	location = _location;
	visuals = _visuals;
	ID = lastID;
	lastID++;
}

Location::Location(){}
// initialize with position
Location::Location(glm::vec4 _Position, glm::vec3 _Size)
{
	Position = _Position;
	Size = _Size;
	box = Bounding_Box(glm::vec4(Position.x - (Size.x / 2), Position.y - (Size.y / 2), Position.z, 1.0f), glm::vec4(Position.x + (Size.x / 2), Position.y + (Size.y / 2), Position.z, 1.0f));
}

Visuals::Visuals(){}
// initialize with visibility setting and atlas coordinates
Visuals::Visuals(bool _Visible, glm::vec2 _atlasCoords) { Visible = _Visible; atlasCoords = _atlasCoords; }
//Physics::Physics(bool _CollisionsEnabled = true, bool _Static) { CollisionsEnabled = _CollisionsEnabled; Static = _Static; }
