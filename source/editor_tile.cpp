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

E_Tile::E_Tile(Physics _physics)
{
	physics = _physics;
	ID = lastID;
	lastID++;
}

E_Tile::E_Tile(Visuals _visuals)
{
	visuals = _visuals;
	ID = lastID;
	lastID++;
}

E_Tile::E_Tile(Location _location, Physics _physics, Visuals _visuals)
{
	location = _location;
	physics = _physics;
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
	box = Bounding_Box(glm::vec2(Size.x, Size.y));
	//box = Bounding_Box(glm::vec4(Position.x - (Size.x / 2), Position.y - (Size.y / 2), Position.z, 1.0f), glm::vec4(Position.x + (Size.x / 2), Position.y + (Size.y / 2), Position.z, 1.0f));
}

Physics::Physics(){}
// initialize with settings
Physics::Physics(bool _CollisionsEnabled, bool _Static)
{
	CollisionsEnabled = _CollisionsEnabled;
	Static = _Static;
}

Visuals::Visuals() {}
// initialize with visibility setting and atlas coordinates
Visuals::Visuals(glm::vec2 _atlasCoords) { atlasCoords = _atlasCoords; } // only texture set by default
Visuals::Visuals(glm::vec2 _atlasCoords, std::string _textureName, TEXTUREMODE _TextureMode) // every setting cus too lazy to make 1 by 1
{
	atlasCoords = _atlasCoords;
	textureName = _textureName;
	TextureMode = _TextureMode;
}

//Physics::Physics(bool _CollisionsEnabled = true, bool _Static) { CollisionsEnabled = _CollisionsEnabled; Static = _Static; }
