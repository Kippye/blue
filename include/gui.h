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
	STATUS,
	IGNORE_LIST,
	SAVE_CONTEXT,
	REOPEN_CONTEXT,
	CONTENT_LACK_WARNING
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
		float
		propertiesPaneWidth = 64.0f * 4,
		tileSelectorPaneWidth = 64.0f * 4,
		bottomBarButtonWidth = 112.0f,
		bottomBarHeight = 32.0f,
		editorPaneHeight = 64.0f,
		editorButtonDistance = 66.0f,
		editorButtonOffset = 33.0f,
		editorComboWidth = 128.0f,
		propertySectionSeparator = 0.0f; //20.0f;
		ImVec2 editorButtonSize = ImVec2(58, 58);
		ImVec2 propertiesButtonSize = ImVec2(128, 32);
		// style stuff
		ImVec2 genericPadding = ImVec2(8.0f, 8.0f);
};

struct GuiSelections
{
	public:
		int currentTextureSelection = 0;
		int currentTextureModeSelection = 0;
		bool newTileOptionsVisible = true;
		bool tagsVisible = false;
		const char* tileTextureModeOptions[2] = { "stretch", "tile" };
};

struct GuiData
{
	public:
		float f64_free = 0.1f,
		f64_snap = 1.0f;
		ImVec4 normalButtonColor;
		ImVec4 selectedButtonColor = ImVec4(0.00f, 0.51f, 1.00f, 1.0f);
		ImVec4 contentFolderColor = ImVec4(0.25f, 0.68f, 0.01f, 1.0f);
		ImVec4 blfFileColor = ImVec4(0.68f, 0.45f, 0.01f, 1.0f);
		ImVec4 buttonDisabled = ImVec4(0.60f, 0.25f, 0.25f, 1.0f);
		ImVec4 buttonDisabledHovered = ImVec4(0.73f, 0.30f, 0.30f, 1.0f);
		ImVec4 buttonDisabledActive = ImVec4(0.82f, 0.26f, 0.0f, 1.0f);
};

class Gui
{
	private:
		// TEMP
		float lastPos[2] = { 0.0f, 0.0f };

		std::string statusMessage = "";
		GuiSizes s;
		GuiData gd;
	public:
		GuiSelections se;
		GUI_STYLE guiStyle = DARK;
		GUI_PROMPT promptType = DIR;
		bool guiFocused = false, guiHovered = false, guiWantKeyboard = false;
		std::map<std::string, unsigned int> guiTextures = {};
		std::map<GUI_POPUP, bool> popupToggles =
		{
			{ STATUS, false },
			{ IGNORE_LIST, false },
			{ SAVE_CONTEXT, false},
			{ CONTENT_LACK_WARNING, false }
		};

		std::vector<E_Texture*>& tileTextures = std::vector<E_Texture*>{};
		ImGuiIO* guiIO;
		ImGuiContext* gui;
		Window* window;

	public:
		void guiInit(Window* windowManager);
		void drawGui();
		void setStatus(const char* status);
		void openFileDialog(GUI_PROMPT type);
	private:
		void addEditorGui();
		void addTextureSelectorGui();
		void addPropertiesGui();
		void addBottomBarGui();
		void addPopupGui();
		void checkFileDialog();
};
