#include <editor_tile.h>

// empty constructor - default tile
Tile::Tile()
{
	ID = lastID;
	lastID++;
}
// constructor with transforms
Tile::Tile(Location _location)
{
	location = _location;
	ID = lastID;
	lastID++;
}

Tile::Tile(Visuals _visuals)
{
	visuals = _visuals;
	ID = lastID;
	lastID++;
}

Tile::Tile(Location _location, Visuals _visuals)
{
	location = _location;
	visuals = _visuals;
	ID = lastID;
	lastID++;
}

Location::Location(){}
// initialize with position
Location::Location(glm::vec4 _Position)
{
	Position = _Position;
	box = Bounding_Box(glm::vec4(Position.x - 0.5f, Position.y - 0.5f, Position.z, 1.0f), glm::vec4(Position.x + 0.5f, Position.y + 0.5f, Position.z, 1.0f));
}

Visuals::Visuals(){}
// initialize with visibility setting and atlas coordinates
Visuals::Visuals(bool _Visible, glm::vec2 _atlasCoords) { Visible = _Visible; atlasCoords = _atlasCoords; }
//Physics::Physics(bool _CollisionsEnabled = true, bool _Static) { CollisionsEnabled = _CollisionsEnabled; Static = _Static; }
