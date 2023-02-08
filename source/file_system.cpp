#include <file_system.h>
#include <main.h>

#include <filesystem>
#include <sstream>

#define DEBUG_FILE_LOADING false

class Program;
extern Program program;

bool FileSystem::check_if_is_ignored(std::string name)
{
	for (std::string ignoreString : ignoreList)
	{
		if (ignoreString == name)
		{
			return true;
		}
	}

	return false;
}

void FileSystem::ignore_buffer_to_vector()
{
	// clear the vector first
	ignoreList.clear();

	std::string ignores = ignoreListBuffer; // convert the buffer to a string
	ignores.erase(std::remove_if(ignores.begin(), ignores.end(), ::isspace), ignores.end());
	std::stringstream ignores_stream(ignores);

	int i = 0;

	// initialize configuration
	Setting& root = config.getRoot();

	if(!root.exists("dirs"))
		root.add("dirs", Setting::TypeGroup);

	Setting& dirs = root["dirs"];

	if (!dirs.exists("ignores"))
	{
		dirs.add("ignores", Setting::TypeArray);
	}

	// clear saved ignores
	for (int j = dirs["ignores"].getLength() - 1; j >= 0; j--)
	{
		dirs["ignores"].remove(j);
	}

	while (ignores_stream.good())
	{
		std::string out;
		std::getline(ignores_stream, out, ',');   //calling getline fuction
		ignoreList.push_back(out);

		// add new ignore file to the settings
		std::string name = "ignore_" + std::to_string(i);
		dirs["ignores"].add(name.c_str(), Setting::TypeString) = out;
		i++;
	}

	std::cout << "Updated ignore list:" << std::endl;
	for (std::string s : ignoreList)
	{
		std::cout << s << ", ";
	}
	std::cout << std::endl;
}

void FileSystem::vectorToIgnoreBuffer()
{
	int index = 0;

	// clear first, jic
	std::fill(std::begin(ignoreListBuffer), std::end(ignoreListBuffer), '\0');

	for (int str = 0; str < ignoreList.size(); str++)
	{
		for (int chr = 0; chr < ignoreList[str].length(); chr++)
		{
			if (index == sizeof(ignoreListBuffer)) { std::cerr << "reached char buffer size when converting from ignoreList vector!" << std::endl; return; }
			ignoreListBuffer[index] = ignoreList[str][chr];
			index++;
			// add separators and space for visual clarity
			if (chr == ignoreList[str].length() - 1 && str != ignoreList.size() - 1)
			{
				ignoreListBuffer[index] = ',';
				index++;
				ignoreListBuffer[index] = ' ';
				index++;
			}
		}
	}
}

std::vector<std::string>& FileSystem::getInDir(const char* directory, bool useIgnoreList, bool filesOnly, bool fullPath, bool extension, std::vector<std::string> acceptedExtensions)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried getting a directory without dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (useIgnoreList && check_if_is_ignored(entry.path().filename().string())) { continue; } // on the ignore list, ignore
		if (filesOnly && entry.path().extension().string() == "") { continue; } // we only want files, not directories

		if (acceptedExtensions.size() > 0)
		{
			bool isAcceptedExtension = false;

			for (int i = 0; i < acceptedExtensions.size(); i++)
			{
				if (entry.path().extension().string() == acceptedExtensions[i])
				{
					isAcceptedExtension = true;
					break;
				}
			}
			// this file does not have an accepted extension
			if (!isAcceptedExtension)
			{
				continue;
			}
		}

		if (DEBUG_FILE_LOADING) std::cout << "Found file: " << entry.path() << "in " << directory << std::endl;

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

std::vector<std::string>& FileSystem::getInDirRecursive(const char* directory, bool useIgnoreList, bool filesOnly, bool fullPath, bool extension, std::vector<std::string> acceptedExtensions)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried loading without dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
	{
		if (useIgnoreList && check_if_is_ignored(entry.path().filename().string())) { continue; } // on the ignore list, ignore
		if (filesOnly && entry.path().extension().string() == "") { continue; } // we only want files, ignore it

		if (acceptedExtensions.size() > 0)
		{
			bool isAcceptedExtension = false;

			for (int i = 0; i < acceptedExtensions.size(); i++)
			{
				if (entry.path().extension().string() == acceptedExtensions[i])
				{
					isAcceptedExtension = true;
					break;
				}
			}
			// this file does not have an accepted extension
			if (!isAcceptedExtension)
			{
				continue;
			}
		}

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

void FileSystem::updateTextures()
{
	std::cout << "Updating textures..." << std::endl;

	bool firstUpdate = program.render.textureAtlas == nullptr;

	// update the textures from the content folder
	if (contentDir != "")
	{
		std::vector<std::string>& filesInContent = getInDirRecursive(contentDir.c_str(), true, true, true, true, imageExtensions);
		program.gui.tileTextures = program.textureLoader.loadTextures(filesInContent, false);
		program.render.textureAtlas = loadContentAsAtlas();
		// only waste time on updating these if they're already something else
		//if (!firstUpdate)
		//{
			program.editor.update_atlas_coords(program.render.textureAtlas);
		//}
	}

	std::cout << "Textures updated" << std::endl;
}

void FileSystem::loadGUITextures()
{
	std::cout << "Loading GUI textures..." << std::endl;

	if (DEBUG_FILE_LOADING) std::cout << "loading GUI textures" << std::endl;
	std::string guiTextureFolder = program.textureLoader.textureFolder + "gui/";
	std::vector<std::string>& filesInFolder = getInDirRecursive(guiTextureFolder.c_str(), true, true, false, true, imageExtensions);
	std::vector<std::string>& fileNames = getInDirRecursive(guiTextureFolder.c_str(), false, true, false, false, imageExtensions);
	std::vector<E_Texture*>& textures = program.textureLoader.loadTextures(filesInFolder, guiTextureFolder);

	for (int i = 0; i < fileNames.size(); i++)
	{
		program.gui.guiTextures.insert({fileNames[i], textures[i]->ID});
	}
	if (DEBUG_FILE_LOADING) std::cout << "GUI textures successfully loaded" << std::endl;
	std::cout << "GUI textures loaded" << std::endl;
}

TextureAtlas* FileSystem::loadContentAsAtlas()
{
	std::cout << "Loading textures from content..." << std::endl;
	if (contentDir == "") { std::cout << "Tried to load content without the folder being set" << std::endl; return nullptr; }
	std::vector<std::string>& filesInContent = getInDirRecursive(contentDir.c_str(), true, true, true, true, imageExtensions);
	TextureAtlas* atlas = program.textureLoader.loadTextureAtlas(filesInContent, false);
	std::cout << "Textures loaded from content" << std::endl;
	return atlas;
}

void FileSystem::tryLoadConfigs()
{
	std::cout << "Loading configuration files..." << std::endl;

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
	if (!dirs.exists("blfDir"))
	{
		dirs.add("blfDir", Setting::TypeString);
		dirs["blfDir"] = "";
	}
	if (!dirs.exists("blf"))
	{
		dirs.add("blf", Setting::TypeString);
		dirs["blf"] = "";
	}
	if (!dirs.exists("ignores"))
	{
		dirs.add("ignores", Setting::TypeArray);
	}

	std::string stringContent = dirs["content"].c_str();
	if (!std::filesystem::exists(std::filesystem::path(stringContent)))
	{
		dirs["content"] = "";
		stringContent = "";
	}
	contentDir = stringContent;

	std::string stringBlfDir = dirs["blfDir"].c_str();
	if (!std::filesystem::exists(std::filesystem::path(stringBlfDir)))
	{
		dirs["blfDir"] = "";
		stringBlfDir = "";
	}
	blfDir = stringBlfDir;

	std::string stringBlf = dirs["blf"].c_str();
	if (!std::filesystem::exists(std::filesystem::path(stringBlf)))
	{
		dirs["blf"] = "";
		stringBlf = "";
	}
	blfFile = stringBlf;

	for (int i = 0; i < dirs["ignores"].getLength(); i++)
	{
		std::string ignorePathString = dirs["ignores"][i].c_str();
		ignoreList.push_back(ignorePathString);
	}

	std::cout << "Configuration files loaded" << std::endl;
}

void FileSystem::trySaveConfigs()
{
	std::cout << "Saving configuration files..." << std::endl;

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

	std::cout << "Configuration files saved" << std::endl;
}

void FileSystem::startNewFile()
{
	program.editor.delete_all();
	program.editor.clear_tags();
	blfFile = "";
	program.windowManager.setTitle("untitled.blf");
}

void FileSystem::tryOpenLastFile()
{
	if (blfFile != "")
	{
		program.gui.popupToggles[REOPEN_CONTEXT] = true;
	}
}
