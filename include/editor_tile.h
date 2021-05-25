#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <util.h>

class Location
{
	public:
		glm::vec4 Position = glm::vec4(0.0f);
		Bounding_Box box;

		// not supported by blur yet glm::vec3 rotation;

		Location();
		Location(glm::vec4 _Position);
};

class Visuals
{
	public:
		bool Visible = true;
		glm::vec2 atlasCoords = glm::vec2(0.0f);
		int renderIndex;

		Visuals();
		Visuals(bool _Visible, glm::vec2 _atlasCoords);
};

class Tile
{
	private:
		static inline unsigned int lastID = 0;
	public:
		unsigned int ID = 0;
		Location location;
		Visuals visuals;
		Tile();
		Tile(Location _location);
		Tile(Visuals _visuals);
		Tile(Location _location, Visuals _visuals);

};

// operators
inline bool operator==(const Tile& tile1, const Tile& tile2){ return tile1.ID == tile2.ID; }
inline bool operator!=(const Tile& tile1, const Tile& tile2){ return !(tile1 == tile2); }
