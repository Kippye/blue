#pragma once

#include <textures.h>
#include <window.h>
#include <editor_tile.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <map>

enum GUI_POPUP
{
	STATUS
};

enum GUI_PROMPT
{
	DIR,
	OPEN,
	SAVE,
	SAVE_AS
};

enum GUI_STYLE
{
	DARK,
	LIGHT
};

struct GuiSizes
{
	public:
		// editor pane size variables
		float propertiesPaneWidth = 64.0f * 4,
		tileSelectorPaneWidth = 64.0f * 4,
		bottomBarButtonWidth = 80.0f,
		bottomBarHeight = 32.0f,
		editorPaneHeight = 64.0f;
		ImVec2 editorButtonSize = ImVec2(58, 58);
		// style stuff
		ImVec2 genericPadding = ImVec2(8.0f, 8.0f);
};

class Gui
{
	private:
		std::string statusMessage = "";
		GuiSizes s;
	public:
		GUI_STYLE guiStyle = DARK;
		GUI_PROMPT promptType = DIR;
		bool guiFocused = false, guiHovered = false;
		std::map<std::string, unsigned int> guiTextures = {};
		std::map<GUI_POPUP, bool> popupToggles =
		{
			{STATUS, false}
		};
		// TEMP
		//Texture* textureTest;
		std::vector<E_Texture*>& tileTextures = std::vector<E_Texture*>{};
		ImGuiIO* guiIO;
		ImGuiContext* gui;
		Window* window;
		// TEMP
		//TextureAtlas* textureAtlas;

	public:
		void guiInit(Window* windowManager);
		void drawGui();
		void setStatus(const char* status);
		void openFileDialog(GUI_PROMPT type);
	private:
		void addEditorGui();
		void addTileSelectorGui();
		void addPropertiesGui();
		void addBottomBarGui();
		void addPopupGui();
		void checkFileDialog();
};
