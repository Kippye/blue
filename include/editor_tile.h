#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Transforms
{
	public:
		glm::vec3 Position = glm::vec3(0.0f);
		// not supported by blur yet glm::vec3 rotation;
		Transforms();
		Transforms(glm::vec3 _Position);
};

class Visuals
{
	public:
		bool Visible = true;
		glm::vec2 atlasCoords = glm::vec2(0.0f);
		Visuals();
		Visuals(bool _Visible, glm::vec2 _atlasCoords);
};

class Editor_Tile
{
	public:
		Editor_Tile();
		Editor_Tile(Transforms trans);
		Editor_Tile(Visuals visual);
		Editor_Tile(Transforms trans, Visuals visual);
		Transforms transforms;
		Visuals visuals;
		//Physics physics;
};
