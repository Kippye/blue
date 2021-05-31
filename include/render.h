#pragma once

#include <shader.h>
#include <glm.hpp>
#include <vector>
#include <editor_tile.h>

class Render
{
	private:
		float vertices[20] = {
			 // positions    		 // texture coords
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // TR
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // BR
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // BL
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f // TL
		};

		unsigned int indices[6] = {
			0, 1, 3, // tri 1
			1, 2, 3  // tri 2
		};

		Shader shader;

		static const int INSTANCE_CAP = 10000;

		float timeCounter = 0.0f;
	public:
		bool mouse_repeat = false;
		float deltaTime = 0.0f, lastFrame = 0.0f, mouse_button_delay = 0.0f, FPS = 0.0f;
		glm::mat4 projection = glm::mat4(1.0f);
		unsigned int VBO, VAO, EBO, instanceVBO, instanceAdditionalVBO;
		TextureAtlas* textureAtlas;
		std::vector<glm::vec4> instanceTransformData = {};
		std::vector<glm::vec4> instanceAdditionalData = {};

	public:
		void setup();
		void updateInstanceArray(); // update the VBO data of instance positions and texture coords
		void render();
		void add_to_render_list(E_Tile &tile); // add to the list of tiles to be rendered
		void add_to_render_list(std::vector<E_Tile> &tiles); // add several tiles to be rendered in one update call
		void remove_from_render_list(E_Tile &tile, int index); // remove from the list of tiles to be rendered
		void remove_from_render_list(std::vector<E_Tile> &tiles, std::vector<int> &indices); // remove several tiles from being rendered in one update call
		void terminate();
};
