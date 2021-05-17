#pragma once

#include <shader.h>
#include <glm.hpp>

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

	public:
		float deltaTime = 0.0f, lastFrame = 0.0f, timeCounter = 0.0f, FPS = 0.0f;
		unsigned int textures[2];
		unsigned int VBO, VAO, EBO, instanceVBO;
		TextureAtlas* textureAtlas;
		glm::vec4 instanceData[INSTANCE_CAP];

	public:
		void setup();
		void render();
		void terminate();
		void test();
};
