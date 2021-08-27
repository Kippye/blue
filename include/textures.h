#pragma once

extern "C" {
#include <glad.h>
}

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <map>

struct E_Texture
{
	public:
	GLuint ID;
	int width;
	int height;
};

struct TextureAtlas
{
	public:
	GLuint ID;
	unsigned char* data;
	int width;
	int height;

	std::vector<std::string> textureFiles = {};

	~TextureAtlas()
	{
		delete[] data;
	}
};

class TextureLoader
{
	private:

	public:
		static inline const std::string textureFolder = "textures/";

		GLuint createEmptyTexture(int* width, int* height);
		unsigned int loadTexture_ID(const char* fileName, std::string directory = textureFolder, bool flip = true);
		unsigned char* loadTextureData(const char* fileName, int* width, int* height, std::string directory = textureFolder, bool flip = true, unsigned int* ID = nullptr, bool bind = false);
		unsigned char* loadTextureData(std::string fullPath, int* width, int* height, bool flip = true, unsigned int* ID = nullptr, bool bind = false);

		E_Texture* loadTexture(const char* fileName, std::string directory = textureFolder, bool flip = false);
		E_Texture* loadTexture(std::string fullPath, bool flip = false);
		std::vector<E_Texture*>& loadTextures(std::vector<std::string> fileNames, std::string directory = textureFolder, bool flip = false);
		std::vector<E_Texture*>& loadTextures(std::vector<std::string> fullPaths, bool flip = false);

		TextureAtlas* loadTextureAtlas(std::vector<std::string> fileNames, std::string directory = textureFolder, bool flip = false);
		TextureAtlas* loadTextureAtlas(std::vector<std::string> fullPaths, bool flip = false);

		unsigned int getAtlasTextureIndex(TextureAtlas* atlas, glm::vec2 coords);
		unsigned int getAtlasTextureIndex(TextureAtlas* atlas, const char* textureName);
		glm::vec2 getAtlasCoords(TextureAtlas* atlas, int index);
		std::string getAtlasTexturePath(TextureAtlas* atlas, glm::vec2 coords);
		glm::vec2 getAtlasTextureCoords(TextureAtlas* atlas, std::string texturePath);
};
