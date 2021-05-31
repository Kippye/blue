#include <file_system.h>
#include <main.h>
#include <filesystem>

#define DEBUG_FILE_LOADING false

class Program;
extern Program program;

std::vector<std::string>& FileSystem::getInDir(const char* directory, bool filesOnly, bool fullPath, bool extension)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried loading without content dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (filesOnly && entry.path().extension().string() == "") { continue; }

		if (DEBUG_FILE_LOADING) std::cout << "found file: " << entry.path() << std::endl;

		if (fullPath) // dir/fn.ext
		{
			if (extension) // dir/fn.ext
			{
				fileNames->push_back(entry.path().string());
			}

			else // dir/fn
			{
				std::filesystem::path path = entry.path();
				path.replace_extension();
				fileNames->push_back(path.string());
			}
		}
		// add only filename + extension
		else
		{
			if (extension) // fn.ext
			{
				fileNames->push_back(entry.path().filename().string());
			}

			else // fn
			{
				fileNames->push_back(entry.path().stem().string());
			}
		}
	}

	return *fileNames;
}

void FileSystem::loadGUITextures()
{
	if (DEBUG_FILE_LOADING) std::cout << "loading GUI textures" << std::endl;
	std::string guiTextureFolder = program.textureLoader.textureFolder + "gui/";
	std::vector<std::string>& filesInFolder = getInDir(guiTextureFolder.c_str());
	std::vector<std::string>& fileNames = getInDir(guiTextureFolder.c_str(), true, false, false);
	std::vector<E_Texture*>& textures = program.textureLoader.loadTextures(filesInFolder, guiTextureFolder);

	for (int i = 0; i < fileNames.size(); i++)
	{
		program.gui.guiTextures.insert({fileNames[i], textures[i]->ID});
	}
	if (DEBUG_FILE_LOADING) std::cout << "GUI textures successfully loaded" << std::endl;
}

TextureAtlas* FileSystem::loadContentAsAtlas()
{
	if (contentDir == "") { std::cout << "tried to load content without the folder being set" << std::endl; return nullptr; }
	program.gui.setStatus("loading textures from content folder...");
	std::vector<std::string>& filesInContent = getInDir(contentDir.c_str());
	TextureAtlas* atlas = program.textureLoader.loadTextureAtlas(filesInContent, contentDir);
	program.gui.setStatus("");
	return atlas;
}

void FileSystem::tryLoadConfigs()
{
	/// set file format options
	config.setOptions
	(
		Config::OptionFsync
	  | Config::OptionSemicolonSeparators
	  | Config::OptionColonAssignmentForGroups
	  | Config::OptionOpenBraceOnSeparateLine
	);

	/// read file or error and quit
	try
	{
		config.readFile(config_file);
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading user configuration." << std::endl;
		return;
	}
	catch(const ParseException &pex)
	{
		std::cerr << "parse error at " << pex.getFile() << ":" << pex.getLine()
		<< " - " << pex.getError() << std::endl;
		return;
	}

	/// find settings or set to defaults if they dont exist
	Setting& root = config.getRoot();

	if(!root.exists("dirs"))
		root.add("dirs", Setting::TypeGroup);

	Setting& dirs = root["dirs"];

	/// get saved directories
	if(!dirs.exists("content"))
	{
		dirs.add("content", Setting::TypeString);
		dirs["content"] = "";
	}
	if (!dirs.exists("blf"))
	{
		dirs.add("blf", Setting::TypeString);
		dirs["blf"] = "";
	}

	std::string sc = dirs["content"].c_str();
	contentDir = sc;
	std::string sb = dirs["blf"].c_str();
	blfDir = sb;
}

void FileSystem::trySaveConfigs()
{
	// Write out the updated configuration.
	try
	{
		config.writeFile(config_file);
		std::cerr << "configuration successfully written to: " << config_file << std::endl;
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while writing file: " << config_file << std::endl;
		return;
	}
}
