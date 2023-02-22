#pragma once

#include <util.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>

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
		double Angle = 0.0f;
		Bounding_Box box;

		// not supported by blur yet glm::vec3 rotation;

		Location();
		Location(glm::vec4 _Position, glm::vec3 _Size = glm::vec3(1.0f), float _Angle = 0.0f);
};

class Physics
{
	public:
		bool CollisionsEnabled = false;
		bool Static = true;
		float Bounce = 0.0f;
		float Density = 1.0f; 
		float Friction = 0.00005f;
		Physics();
		Physics(bool _CollisionsEnabled, bool _Static = false, float _Bounce = 0.0f, float _Density = 1.0f, float Friction = 0.00005f);
};

class Visuals
{
	public:
		glm::uvec4 atlasLocation = glm::uvec4(0, 0, 16, 16);
		std::string textureName = "";
		TEXTUREMODE TextureMode = TEXTUREMODE_STRETCH;
		glm::vec2 TextureSize = glm::vec2(1.0f, 1.0f);
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float Opacity = 1.0f;

		Visuals();
		Visuals(glm::uvec4 _atlasLocation);
		Visuals(glm::uvec4 _atlasLocation, std::string _textureName, TEXTUREMODE _TextureMode, glm::vec2 _TextureSize, glm::vec4 _Color, float _Opacity);
};


class E_Tile
{
	private:
		static inline unsigned int lastID = 0;
	public:
		unsigned int ID = 0;
		bool selected = false;
		// contains whether or not a specific index tag is applied to this tile
		// TODO: should use a const
		bool tags[MAX_TAGS] = { false, false, false, false, false };
		Location location;
		Physics physics;
		Visuals visuals;
		E_Tile();
		E_Tile(Location _location);
		E_Tile(Physics _physics);
		E_Tile(Visuals _visuals);
		E_Tile(Location _location, Physics _physics, Visuals _visuals);
		E_Tile(Location _location, Physics _physics, Visuals _visuals, bool* _tags);
};

// operators
inline bool operator==(const E_Tile& tile1, const E_Tile& tile2){ return tile1.ID == tile2.ID; }
inline bool operator!=(const E_Tile& tile1, const E_Tile& tile2){ return !(tile1 == tile2); }
