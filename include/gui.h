#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <textures.h>
#include <window.h>
#include <vector>

enum GUI_STYLE
{
	DARK,
	LIGHT
};

class Gui
{
	public:
		GUI_STYLE guiStyle = DARK;
		bool guiFocused = false;
		Texture* textureTest;
		std::vector<Texture*>& tileTextures = std::vector<Texture*>{};
		Window* window;
		float propertiesPaneWidth = 64.0f * 4 + 32.0f;

		TextureAtlas* textureAtlas;

	public:
		void guiInit(GLFWwindow* window);
		void addEditorGui();
		void drawGui();
};
