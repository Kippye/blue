#pragma once

extern "C" {
#include <glad.h>
}

#include <string>
#include <vector>

struct Texture
{
	GLuint ID;
	int width;
	int height;
};

struct TextureAtlas
{
	GLuint ID;
	unsigned char* data;
	int width;
	int height;

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

		unsigned int loadTexture_ID(const char* fileName, std::string directory = textureFolder, bool flip = true);
		Texture* loadTexture(const char* fileName, std::string directory = textureFolder, bool flip = false);
		std::vector<Texture*>& loadTextures(std::vector<std::string> fileNames, std::string directory = textureFolder, bool flip = false);
		unsigned char* loadTextureData(const char* fileName, int* width, int* height, std::string directory = textureFolder, bool flip = true, unsigned int* ID = nullptr, bool bind = false);
		GLuint createEmptyTexture(int* width, int* height);
		TextureAtlas* loadTextureAtlas(std::vector<std::string> fileNames, std::string directory = textureFolder, bool flip = false);
};
