#pragma once

#include <window.h>
#include <filesystem.h>
#include <textures.h>
#include <camera.h>
#include <render.h>
#include <input.h>
#include <level_view.h>
#include <gui.h>

class Window;
class FileSystem;
class TextureLoader;
class Camera;
class Render;
class Input;
class Level_View;

class Program
{
		public:
			Window& windowManager = *(new Window());
			FileSystem& fileSystem = FileSystem();
			TextureLoader& textureLoader = TextureLoader();
			Camera& camera = Camera();
			Render& render = Render();
			Input& input = Input();
			Level_View levelView = Level_View();
			Gui& gui = Gui();

			Program(const Program&) = default;
			Program& operator=(const Program&) = default;
			Program();
			void loop();
};
