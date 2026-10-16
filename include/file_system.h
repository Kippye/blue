#pragma once

#include <textures.h>
#include <string>
#include <vector>
#include <iostream>
#include <libconfig.h++>
#include <filesystem>

using namespace libconfig;

#if defined(_WIN32) || defined(WIN32)
	inline std::filesystem::path rootPath("C:\\");
	inline const char* pathSeparator = "\\";
#elif defined(__unix__)
	inline std::filesystem::path rootPath("/");
	inline const char *pathSeparator = "/";
#endif

class FileSystem
{
	private:
		Config config;
		static const inline char *config_file = "user.cfg";
		static const inline char *ignore_file = "ignore.cfg";
		std::vector<std::string> ignoreList = {};
		std::vector<std::string> imageExtensions = {".png", ".jpg", ".jpeg"};

	public:
		std::string contentDir = "";
		std::string blfDir = "";
		std::string blfFile = "";

		char defaultIgnoreListBuffer[1024] = { "close.png, cursor.png, dragger-cursor.png, logo.png, logograss.png, resizedragger.png" };
		char ignoreListBuffer[1024] = {};

		bool contextOpen = false;

		bool check_ignored(std::string name);
		bool check_path_ignored(std::filesystem::path path, std::filesystem::path* highestParentPath);
		void ignore_buffer_to_vector();
		void vectorToIgnoreBuffer();
		void update_editor_config();

		std::vector<std::string>& getInDir(const char* directory, bool useIgnoreList = true, bool filesOnly = true, bool fullPath = false, bool extension = true, std::vector<std::string> acceptedExtensions = {});
		std::vector<std::string>& getInDirRecursive(const char* directory, bool useIgnoreList = true, bool filesOnly = true, bool fullPath = false, bool extension = true, std::vector<std::string> acceptedExtensions = {}, std::filesystem::path* highestParentPath = nullptr);

		void updateTextures();

		void loadGUITextures();
		TextureAtlas* loadContentAsAtlas();

		void tryLoadConfigs();
		void trySaveConfigs();
		void startNewFile();
		void tryOpenLastFile();

		template <typename T> Setting* changeSetting(const std::string& setting_name, T new_value, Setting::Type type, bool add_if_NA = true)
		{
			Setting& root = config.getRoot();

			try {
				Setting& setting = root.lookup(setting_name);

				setting = new_value;
				return &setting;
			} catch (SettingNotFoundException)
			{
				if (add_if_NA)
				{
					// TEMP HACK
					std::string settingDir = setting_name.substr(0, setting_name.find(".") - 1);
					std::string settingName = setting_name.substr(setting_name.find("."));
					return &root.add(setting_name, Setting::TypeGroup).add(settingName, type);
				} else {
					std::cerr << "tried to change non-existant setting: " << setting_name << std::endl;
					return nullptr;
				}
			}
		}
};
