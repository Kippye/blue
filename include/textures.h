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
	unsigned char* data;

	std::string fileName;
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

	std::vector<E_Texture> textures = {};
	std::vector<glm::vec2> texturePositions;

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

		E_Texture* loadTexture(const char* fileName, std::string directory = textureFolder, bool flip = true, bool bind = true);
		E_Texture* loadTexture(std::string fullPath, bool flip = true, bool bind = true);

		std::vector<E_Texture*>& loadTextures(std::vector<std::string> fileNames, std::string directory = textureFolder, bool flip = false, bool bind = true);
		std::vector<E_Texture*>& loadTextures(std::vector<std::string> fullPaths, bool flip = false, bool bind = true);

		TextureAtlas* loadTextureAtlas(std::vector<std::string> fullPaths, bool flip = false);

		/// <summary>
		/// Convert atlas coordinates to an index in the 1 dimensional texture array
		/// </summary>
		/// <param name="atlas"></param>
		/// <param name="coords"></param>
		/// <returns></returns>
		unsigned int getAtlasTextureIndex(TextureAtlas* atlas, glm::vec2 coords);
		/// <summary>
		/// Convert a texture's name to an index in the 1 dimensional texture array
		/// </summary>
		/// <param name="atlas"></param>
		/// <param name="textureName"></param>
		/// <returns></returns>
		unsigned int getAtlasTextureIndex(TextureAtlas* atlas, const char* textureName);
		/// <summary>
		/// Convert a 1 dimensional array index to 2 dimensional atlas coordinates
		/// </summary>
		/// <param name="atlas"></param>
		/// <param name="index"></param>
		/// <returns></returns>
		glm::vec2 getAtlasCoords(TextureAtlas* atlas, int index);
		/// <summary>
		/// Get the filename of the texture at the given atlas coordinates
		/// </summary>
		/// <param name="atlas"></param>
		/// <param name="coords"></param>
		/// <returns></returns>
		std::string getAtlasTexturePath(TextureAtlas* atlas, glm::vec2 coords);
		/// <summary>
		/// Get the atlas coordinates of the texture at texturePath
		/// </summary>
		/// <param name="atlas"></param>
		/// <param name="texturePath"></param>
		/// <returns></returns>
		glm::vec2 getAtlasTextureCoords(TextureAtlas* atlas, std::string texturePath);
};
