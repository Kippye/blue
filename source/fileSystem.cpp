#include <fileSystem.h>
#include <filesystem>

#include <iostream>

std::vector<std::string>& FileSystem::getInDir(const char* directory, bool filesOnly, bool fullPath)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { std::cout << "Tried loading without content dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (filesOnly && entry.path().extension().string() == "") { continue; }

		std::cout << "found file: " << entry.path() << std::endl;
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
