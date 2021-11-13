#pragma once

#include <util.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>

enum TEXTUREMODE
{
	TEXTUREMODE_STRETCH,
	TEXTUREMODE_TILE
};

class Location
{
	public:
		glm::vec4 Position = glm::vec4(0.0f);
		glm::vec3 Size = glm::vec3(1.0f);
		Bounding_Box box;

		// not supported by blur yet glm::vec3 rotation;

		Location();
		Location(glm::vec4 _Position, glm::vec3 _Size = glm::vec3(1.0f));
};

class Physics
{
	public:
		bool CollisionsEnabled = true;
		bool Static = true;
		Physics();
		Physics(bool _CollisionsEnabled, bool _Static = false);
};

class Visuals
{
	public:
		glm::vec2 atlasCoords = glm::vec2(0.0f);
		std::string textureName = "";
		TEXTUREMODE TextureMode = TEXTUREMODE_TILE;

		Visuals();
		Visuals(glm::vec2 _atlasCoords);
		Visuals(glm::vec2 _atlasCoords, std::string _textureName, TEXTUREMODE _TextureMode);
};


class E_Tile
{
	private:
		static inline unsigned int lastID = 0;
	public:
		unsigned int ID = 0;
		bool selected = false;
		Location location;
		Physics physics;
		Visuals visuals;
		E_Tile();
		E_Tile(Location _location);
		E_Tile(Physics _physics);
		E_Tile(Visuals _visuals);
		E_Tile(Location _location, Physics _physics, Visuals _visuals);
};

// operators
inline bool operator==(const E_Tile& tile1, const E_Tile& tile2){ return tile1.ID == tile2.ID; }
inline bool operator!=(const E_Tile& tile1, const E_Tile& tile2){ return !(tile1 == tile2); }
