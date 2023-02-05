#pragma once

#include <shader.h>
#include <glm.hpp>
#include <vector>
#include <editor_tile.h>

enum INSTANCE_ARRAY_UPDATE
{
	INSTANCE_ARRAY_UPDATE_ALL = 0,
	INSTANCE_ARRAY_UPDATE_1 = 1 << 1,
	INSTANCE_ARRAY_UPDATE_2 = 1 << 2,
	INSTANCE_ARRAY_UPDATE_3 = 1 << 3,
};

class Render
{
	private:
		float vertices[5] = {
			 // positions    		 // texture coords
			 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
		};

		unsigned int indices[1] = {
			0
		};

		Shader shader;

		float timeCounter = 0.0f;
	public:
		bool mouse_repeat = false;
		float deltaTime = 0.0f, lastFrame = 0.0f, mouse_button_delay = 0.0f, FPS = 0.0f;

		glm::mat4 projection = glm::mat4(1.0f);

		unsigned int VBO, VAO, EBO, instanceVBO, instanceAdditionalVBO, instanceColorVBO;
		TextureAtlas* textureAtlas;

		std::vector<glm::vec4> instanceTransformData = {};
		std::vector<glm::vec4> instanceAdditionalData = {};
		std::vector<glm::vec4> instanceColorData = {};

	public:
		void setup();
		void updateInstanceArray(INSTANCE_ARRAY_UPDATE type = INSTANCE_ARRAY_UPDATE_ALL); // update the instance VBOs of transform and additional data
		void render();

		void set_tile_selection(int index, bool to);
		void Render::set_tile_selection(std::vector<int> &indices, bool to);

		void add_to_instance_data(E_Tile &tile); // add to instance data
		void erase_from_instance_data(int index); // remove from instance data

		void add_to_render_list(E_Tile &tile); // add to the list of tiles to be rendered
		void add_to_render_list(std::vector<E_Tile> &tiles); // add several tiles to be rendered in one update call
		void remove_from_render_list(int index); // remove from the list of tiles to be rendered
		void remove_from_render_list(std::vector<int> &indices); // remove several tiles from being rendered in one update call

		void terminate();
};
