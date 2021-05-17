#pragma once

#include <string>
#include <vector>

class FileSystem
{
	public:
		std::string contentDir = "C:\\Users\\Kasutaja\\Documents\\BlurEngine\\kill guy shot gun\\content\\";
		bool contextOpen = false;
		std::vector<std::string>& getInDir(const char* directory, bool filesOnly = true, bool fullPath = false);
};
