#pragma once

#include <shader.h>
#include <glm.hpp>
#include <vector>
#include <editor_tile.h>

enum INSTANCE_ARRAY_UPDATE
{
	INSTANCE_ARRAY_UPDATE_ALL = 0,
	// TRANSFORM UPDATE
	INSTANCE_ARRAY_UPDATE_1 = 1 << 1,
	// TEXTURE UPDATE
	INSTANCE_ARRAY_UPDATE_2 = 1 << 2,
	// ATLAS UPDATE
	INSTANCE_ARRAY_UPDATE_3 = 1 << 3,
	// COLOR UPDATE
	INSTANCE_ARRAY_UPDATE_4 = 1 << 4,
	// ADDITIONAL DATA UPDATE
	INSTANCE_ARRAY_UPDATE_5 = 1 << 5
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
		double timeSinceStart = 0.0;
	public:
		bool mouseRepeat = false;
		float deltaTime = 0.0f, lastFrame = 0.0f, mouseButtonDelay = 0.0f, FPS = 0.0f;
		unsigned int instanceDataUpdates = 0;

		glm::mat4 projection = glm::mat4(1.0f);

		unsigned int VBO, VAO, EBO, instanceVBO, instanceTextureVBO, instanceAtlasVBO, instanceColorVBO, instanceAdditionalVBO;
		unsigned int VBO_G, VAO_G, EBO_G, instanceVBO_G, instanceTextureVBO_G, instanceAtlasVBO_G, instanceColorVBO_G, instanceAdditionalVBO_G;
		TextureAtlas* textureAtlas;

		std::vector<glm::vec4> instanceTransformData = {};
		std::vector<glm::vec4> instanceTextureData = {};
		std::vector<glm::uvec4> instanceAtlasData = {};
		std::vector<glm::vec4> instanceColorData = {};
		std::vector<glm::vec4> instanceAdditionalData = {};

		std::vector<glm::vec4> GinstanceTransformData = {};
		std::vector<glm::vec4> GinstanceTextureData = {};
		std::vector<glm::uvec4> GinstanceAtlasData = {};
		std::vector<glm::vec4> GinstanceColorData = {};
		std::vector<glm::vec4> GinstanceAdditionalData = {};

	public:
		void setup();
		void updateInstanceArray(INSTANCE_ARRAY_UPDATE type = INSTANCE_ARRAY_UPDATE_ALL); // update the instance VBOs of transform and additional data for TILES only
		void updateGizmoInstanceArray(INSTANCE_ARRAY_UPDATE type = INSTANCE_ARRAY_UPDATE_ALL); // update the instance VBOs of transform and additional data for GIZMOS only
		void render();

		void set_tile_selection(int index, bool to);
		void set_tile_selection(std::vector<int> &indices, bool to);
		void set_gizmo_highlighted(int index, float to);

		void add_to_instance_data(E_Tile &tile); // add to instance data
		void add_gizmo_to_instance_data(Gizmo &gizmo); // add to instance data
		void erase_from_instance_data(int index); // remove from instance data
		void erase_gizmo_from_instance_data(int index); // remove from instance data
		void clear_instance_data();
		void clear_gizmo_instance_data();

		void add_to_render_list(E_Tile &tile); // add to the list of tiles to be rendered
		void add_to_render_list(std::vector<E_Tile> &tiles); // add several tiles to be rendered in one update call
		void add_gizmo_to_render_list(Gizmo &gizmo);
		void remove_from_render_list(int index); // remove from the list of tiles to be rendered
		void remove_from_render_list(std::vector<int> &indices, bool sort = false); // remove several tiles from being rendered in one update call
		void remove_gizmo_from_render_list(int index);

		void terminate();
};
