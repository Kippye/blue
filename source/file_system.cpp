#include <file_system.h>
#include <main.h>

#include <sstream>

// TEMP
#include <chrono>

#define DEBUG_FILE_LOADING false
namespace fs = std::filesystem;

class Program;
extern Program program;

bool FileSystem::check_ignored(std::string name)
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

bool FileSystem::check_path_ignored(fs::path path, fs::path* highestParentPath)
{
	fs::path pc = path;

	bool debug = false;

	if (highestParentPath == nullptr)
	{
		highestParentPath = &rootPath;
	}
	// check initial filename
	if (debug) std::cout  << pc.string() << std::endl;
	if (check_ignored(pc.filename().string())) { return true; }

	unsigned int safetyCounter = 0;

	if (debug) std::cout << pc.parent_path().string() << "; " << highestParentPath->string() << "; " << (pc.parent_path().compare(*highestParentPath)) << "; " << (pc.parent_path() == *highestParentPath) << std::endl;

	while (pc.has_parent_path() && pc.parent_path().compare(*highestParentPath) != 0)
	{
		pc = pc.parent_path();
		if (debug) std::cout << "Parent path: " << pc.string() << std::endl;

		if (check_ignored(pc.filename().string()))
		{
			if (debug) std::cout << "Path " << pc.string() << " is ignored" << std::endl;
			return true;
		}

		safetyCounter++;
		if (safetyCounter > 100)
		{
			debug::print_error("FileSystem::check_path_ignored: Surpassed the loop limit [100] in parent path ignore check loop");
			return false;
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
		dirs["ignores"].add(Setting::TypeString) = out;

		i++;
	}

	// for (std::string s : ignoreList)
	// {
	// 	std::cout << s << ", ";
	// }
	// std::cout << std::endl;
	trySaveConfigs();
	std::cout << "Ignore list updated" << std::endl;
}

void FileSystem::vectorToIgnoreBuffer()
{
	int index = 0;

	// clear first, jic
	std::fill(std::begin(ignoreListBuffer), std::end(ignoreListBuffer), '\0');

	for (size_t str = 0; str < ignoreList.size(); str++)
	{
		for (size_t chr = 0; chr < ignoreList[str].length(); chr++)
		{
			if (index == sizeof(ignoreListBuffer)) { debug::print_warning("FileSystem::vectorToIgnoreBuffer: Reached char buffer size when converting from ignoreList vector!"); return; }
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

void FileSystem::update_editor_config()
{
	// initialize configuration
	Setting& root = config.getRoot();

	if(!root.exists("editor"))
		root.add("editor", Setting::TypeGroup);

	Setting& editor = root["editor"];

	if (!editor.exists("backgroundColor"))
	{
		editor.add("backgroundColor", Setting::TypeArray);
	}

	// clear saved ignores
	for (int i = editor["backgroundColor"].getLength() - 1; i >= 0; i--)
	{
		editor["backgroundColor"].remove(i);
	}

	for (int i = 0; i < 3; i++)
	{
		editor["backgroundColor"].add(Setting::TypeFloat) = program.editor.backgroundColor[i];
	}

	trySaveConfigs();
	std::cout << "Editor configuration updated" << std::endl;
}

std::vector<std::string>& FileSystem::getInDir(const char* directory, bool useIgnoreList, bool filesOnly, bool fullPath, bool extension, std::vector<std::string> acceptedExtensions)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried getting a directory without dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : fs::directory_iterator(directory))
	{
		if (useIgnoreList && check_ignored(entry.path().filename().string())) { continue; } // on the ignore list, ignore
		if (filesOnly && entry.path().extension().string() == "") { continue; } // we only want files, not directories

		if (acceptedExtensions.size() > 0)
		{
			bool isAcceptedExtension = false;

			for (size_t i = 0; i < acceptedExtensions.size(); i++)
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
				fs::path path = entry.path();
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

std::vector<std::string>& FileSystem::getInDirRecursive(const char* directory, bool useIgnoreList, bool filesOnly, bool fullPath, bool extension, std::vector<std::string> acceptedExtensions, fs::path* highestParentPath)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried loading without dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : fs::recursive_directory_iterator(directory))
	{
		if (useIgnoreList && check_path_ignored(entry.path(), highestParentPath)) { continue; } // on the ignore list, ignore
		if (filesOnly && entry.path().extension().string() == "") { continue; } // we only want files, ignore it

		if (acceptedExtensions.size() > 0)
		{
			bool isAcceptedExtension = false;

			for (size_t i = 0; i < acceptedExtensions.size(); i++)
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
				fs::path path = entry.path();
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
	auto start_time = std::chrono::high_resolution_clock::now();

	bool firstUpdate = program.render.textureAtlas == nullptr;

	// update the textures from the content folder
	if (contentDir != "")
	{
		std::cout << "Creating texture atlas..." << std::endl;
		fs::path highestParentPath = fs::path(contentDir.substr(0, contentDir.length() - 1));
		std::vector<std::string>& filesInContent = getInDirRecursive(contentDir.c_str(), true, true, true, true, imageExtensions, &highestParentPath);
		program.gui.tileTextures = program.textureLoader.loadTextures(filesInContent, false);
		program.gui.gd.displayedTextureButtons = 0;
		// NOTE: sometimes this randomly causes a crash at "Loading textures from content..."
		program.render.textureAtlas = loadContentAsAtlas();
		for (E_Texture* tt: program.gui.tileTextures)
		{
			tt->atlasLocation = program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, tt->path);
		}
		auto load_as_atlas_time = std::chrono::high_resolution_clock::now();
		auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(load_as_atlas_time - start_time).count();
		std::cout << "Atlas created in " << duration2 << " ms" << std::endl;
		auto coord_update_start_time = std::chrono::high_resolution_clock::now();
		std::cout << "Updating editor atlas coordinates..." << std::endl;
		// only waste time on updating these if they're already something else
		//if (!firstUpdate)
		//{
			program.editor.update_atlas_coords(program.render.textureAtlas);
		//}
		auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - coord_update_start_time).count();
		std::cout << "Editor atlas coordinate update completed in " << duration3 << " ms" << std::endl;
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cout << "Texture update completed in " << duration << " ms" << std::endl;
}

void FileSystem::loadGUITextures()
{
	std::cout << "Loading GUI textures..." << std::endl;

	std::string guiTextureFolder = program.textureLoader.textureFolder + "gui/";
	std::vector<std::string>& filesInFolder = getInDirRecursive(guiTextureFolder.c_str(), false, true, false, true, imageExtensions);
	std::vector<std::string>& fileNames = getInDirRecursive(guiTextureFolder.c_str(), false, true, false, false, imageExtensions);
	std::vector<E_Texture*>& textures = program.textureLoader.loadTextures(filesInFolder, guiTextureFolder);

	for (size_t i = 0; i < fileNames.size(); i++)
	{
		program.gui.guiTextures.insert({fileNames[i], textures[i]->ID});
	}
	std::cout << "GUI textures loaded" << std::endl;
}

TextureAtlas* FileSystem::loadContentAsAtlas()
{
	std::cout << "Loading textures from content..." << std::endl;
	if (contentDir == "") { std::cout << "Tried to load content without the folder being set" << std::endl; return nullptr; }
	fs::path highestParentPath = fs::path(contentDir.substr(0, contentDir.length() - 1));
	std::vector<std::string>& filesInContent = getInDirRecursive(contentDir.c_str(), true, true, true, true, imageExtensions, &highestParentPath);
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
		std::cerr << "I/O error while reading user configuration from: [" << config_file << "]: " << fioex.what() << std::endl;
		return;
	}
	catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
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

	if(!root.exists("editor"))
		root.add("editor", Setting::TypeGroup);

	Setting& editor = root["editor"];

	if (!editor.exists("backgroundColor"))
	{
		editor.add("backgroundColor", Setting::TypeArray);
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

	for (int i = 0; i < std::min(editor["backgroundColor"].getLength(), 3); i++)
	{
		program.editor.backgroundColor[i] = editor["backgroundColor"][i];
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
		std::cout << "Configuration successfully written to: " << config_file << std::endl;
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while writing file: [" << config_file << "]: " << fioex.what() << std::endl;
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
