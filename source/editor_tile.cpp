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

E_Tile::E_Tile(Location _location, Physics _physics, Visuals _visuals, bool* _tags)
{
	location = _location;
	physics = _physics;
	visuals = _visuals;
	for (int i = 0; i < sizeof(tags) / sizeof(bool); i++)
	{
		tags[i] = _tags[i];
  	}
	ID = lastID;
	lastID++;
}

Location::Location(){}
// initialize with position
Location::Location(glm::vec4 _Position, glm::vec3 _Size, float _Angle)
{
	Position = _Position;
	Size = _Size;
	Angle = _Angle;
	box = Bounding_Box(glm::vec2(Size.x, Size.y));
	//box = Bounding_Box(glm::vec4(Position.x - (Size.x / 2), Position.y - (Size.y / 2), Position.z, 1.0f), glm::vec4(Position.x + (Size.x / 2), Position.y + (Size.y / 2), Position.z, 1.0f));
}

Physics::Physics(){}
// initialize with settings
Physics::Physics(bool _CollisionsEnabled, bool _Static, float _Bounce, float _Density, float _Friction)
{
	CollisionsEnabled = _CollisionsEnabled;
	Static = _Static;
	Bounce = _Bounce;
	Density = _Density;
	Friction = _Friction;
}

Visuals::Visuals() {}
// initialize with visibility setting and atlas coordinates
Visuals::Visuals(glm::uvec4 _atlasLocation) 
{ 
	atlasLocation = _atlasLocation;
}
Visuals::Visuals(glm::uvec4 _atlasLocation, std::string _textureName, TEXTUREMODE _TextureMode, glm::vec2 _TextureSize, glm::vec4 _Color, float _Opacity) // every setting cus too lazy to make 1 by 1
{
	atlasLocation = _atlasLocation;
	textureName = _textureName;
	TextureMode = _TextureMode;
	TextureSize = _TextureSize;
	Color = _Color;
	Opacity = _Opacity;
}