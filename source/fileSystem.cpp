#include <fileSystem.h>
#include <main.h>
#include <filesystem>

#include <iostream>
#define DEBUG_FILE_LOADING false

class Program;
extern Program program;

std::vector<std::string>& FileSystem::getInDir(const char* directory, bool filesOnly, bool fullPath)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried loading without content dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (filesOnly && entry.path().extension().string() == "") { continue; }

		if (DEBUG_FILE_LOADING) std::cout << "found file: " << entry.path() << std::endl;
		// add full absolute path
		if (fullPath)
		{
			fileNames->push_back(entry.path().string());
		}
		// add only filename + extension
		else
		{
			fileNames->push_back(entry.path().filename().string());
		}
	}

	return *fileNames;
}

TextureAtlas* FileSystem::loadContentAsAtlas()
{
	program.gui.setStatus("loading textures from content folder...");
	std::vector<std::string>& filesInContent = getInDir(contentDir.c_str());
	TextureAtlas* atlas = program.textureLoader.loadTextureAtlas(filesInContent, contentDir);
	program.gui.setStatus("");
	return atlas;
}
