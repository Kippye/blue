#pragma once

#include <textures.h>
#include <string>
#include <vector>
#include <iostream>
#include <libconfig.h++>

using namespace libconfig;

class FileSystem
{
	private:
		Config config;
		static const inline char *config_file = "user.cfg";
		static const inline char *ignore_file = "ignore.cfg";
		std::vector<std::string> ignoreList = {};
		std::vector<std::string> imageExtensions = {".png", ".jpg", ".jpeg"};

	public:
		// TEMP save this in config file instead
		std::string contentDir = "";
		std::string blfDir = "";
		std::string blfFile = "";

		char defaultIgnoreListBuffer[1024] = { "scripts, shaders, logo.png, logograss.png, resizedragger.png" };
		char ignoreListBuffer[1024] = {};

		bool contextOpen = false;

		bool check_if_is_ignored(std::string name);
		void ignore_buffer_to_vector();
		void vectorToIgnoreBuffer();

		std::vector<std::string>& getInDir(const char* directory, bool useIgnoreList = true, bool filesOnly = true, bool fullPath = false, bool extension = true, std::vector<std::string> acceptedExtensions = {});
		std::vector<std::string>& getInDirRecursive(const char* directory, bool useIgnoreList = true, bool filesOnly = true, bool fullPath = false, bool extension = true, std::vector<std::string> acceptedExtensions = {});

		void updateTextures();

		void loadGUITextures();
		TextureAtlas* loadContentAsAtlas();

		void tryLoadConfigs();
		void trySaveConfigs();
		void startNewFile();
		void tryOpenLastFile();

		template <typename T> void changeSetting(const char* setting_name, T new_value, Setting::Type type, bool add_if_NA = true)
		{
			Setting& root = config.getRoot();
			if (!root.exists(setting_name))
			{
				if (add_if_NA)
				{
					root.add(setting_name, type);
				}
				else
				{
					std::cerr << "tried to change non-existant setting: " << setting_name << std::endl;
					return;
				}
			}

			root.lookup(setting_name) = new_value;
		}
};
