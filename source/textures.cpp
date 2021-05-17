#include <textures.h>
extern "C" {
#include <glad.h>
}
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cmath>

unsigned int TextureLoader::loadTexture_ID(const char* fileName, std::string directory, bool flip)
{
	std::string filePath = fileName;
	std::string fullPath = (directory + filePath);

	unsigned int texture;
	// 1 - amount, &texture - array of IDs
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// filtering options for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// GL_LINEAR would be the "normal" mode, GL_NEAREST pixel mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(flip);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		// generate texture
		if (filePath.find(".png") != std::string::npos)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (filePath.find(".jpg") != std::string::npos)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Unsupported image file type!" << std::endl;
		}
		// generates a mipmap for us
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture at: " << fullPath << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	// download free memory
	stbi_image_free(data);

	return texture;
}

Texture* TextureLoader::loadTexture(const char* fileName, std::string directory, bool flip)
{
	std::string filePath = fileName;
	std::string fullPath = (directory + filePath);

	Texture* texture = new Texture();

	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(fullPath.c_str(), &texture->width, &texture->height, NULL, 4);

	// 1 - amount, &texture - array of IDs
	glGenTextures(1, &texture->ID);
	glBindTexture(GL_TEXTURE_2D, texture->ID);
	// filtering options for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// GL_LINEAR would be the "normal" mode, GL_NEAREST pixel mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Upload pixels into texture
	#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	#endif

	if (data)
	{
		// generate texture
		if (filePath.find(".png") != std::string::npos)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (filePath.find(".jpg") != std::string::npos)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Unsupported image file type!" << std::endl;
		}
	}
	else
	{
		std::cout << "Failed to load texture at: " << fullPath << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	// download free memory
	stbi_image_free(data);

	return texture;
}

unsigned char* TextureLoader::loadTextureData(const char* fileName, int* width, int* height, std::string directory, bool flip, unsigned int* ID, bool bind)
{
	std::string filePath = fileName;
	std::string fullPath = (directory + filePath);

	int nrChannels;

	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(fullPath.c_str(), width, height, &nrChannels, 4);

	if (bind)
	{
		// 1 - amount, &texture - array of IDs
		glGenTextures(1, ID);
		glBindTexture(GL_TEXTURE_2D, *ID);
		// filtering options for this texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// GL_LINEAR would be the "normal" mode, GL_NEAREST pixel mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Upload pixels into texture
		#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		#endif
	}

	if (data)
	{
		// generate texture
		if (filePath.find(".png") != std::string::npos)
		{
			if (bind) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (filePath.find(".jpg") != std::string::npos)
		{
			if (bind) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Unsupported image file type!" << std::endl;
		}
	}
	else
	{
		std::cout << "Failed to load texture data at: " << fullPath << std::endl;
	}

	if (bind)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// download free memory
	// stbi_image_free(data);

	return data;
}

std::vector<Texture*>& TextureLoader::loadTextures(std::vector<std::string> fileNames, std::string directory, bool flip)
{
	std::vector<Texture*>* textures = new std::vector<Texture*>();

	for (int i = 0; i < fileNames.size(); i++)
	{
		textures->push_back(loadTexture(fileNames[i].c_str(), directory, flip));
	}

	return *textures;
}

GLuint TextureLoader::createEmptyTexture(int* width, int* height)
{
	unsigned char *data = new unsigned char[*width * *height];

	GLuint ID;

    for (int i = 0; i < *width * *height; i++)
        data = 0;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
    return ID;
}

TextureAtlas* TextureLoader::loadTextureAtlas(std::vector<std::string> fileNames, std::string directory, bool flip)
{
	TextureAtlas* textureAtlas = new TextureAtlas();

	int tilesPerRow = ceil(sqrt(fileNames.size()));
	int tilesPerColumn = ceil(sqrt(fileNames.size() - tilesPerRow));

	int atlasDataSize = ((16 * 16) * (tilesPerRow * tilesPerColumn));
	unsigned char* textureAtlasData = new unsigned char[atlasDataSize];
	// create an empty texture for the atlas
	textureAtlas->width = tilesPerRow * 16;
	textureAtlas->height = tilesPerColumn * 16;
	textureAtlas->ID = createEmptyTexture(&textureAtlas->width, &textureAtlas->height);

	int total = 0;

	for (int y = 0; y < tilesPerColumn; y++)
	{
		for (int x = 0; x < tilesPerRow; x++)
		{
			if (total >= fileNames.size()){ break; }

			int width, height;
			unsigned int ID;
			unsigned char* texture = loadTextureData(fileNames[total].c_str(), &width, &height, directory, false);

			// add subtexture to texture atlas
			glBindTexture(GL_TEXTURE_2D, textureAtlas->ID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 16 * x, 16 * y, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, texture);
			glBindTexture(GL_TEXTURE_2D, 0);

			total++;
		}
	}

	std::cout << "LOADED TEXTURE ATLAS: " << "width: " << textureAtlas->width << "; height: " << textureAtlas->height << "; ID: " << textureAtlas->ID << std::endl;

	return textureAtlas;
}
