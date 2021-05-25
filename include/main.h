#pragma once

#include <window.h>
#include <filesystem.h>
#include <textures.h>
#include <camera.h>
#include <render.h>
#include <input.h>
#include <editor.h>
#include <gui.h>

class Window;
class FileSystem;
class TextureLoader;
class Camera;
class Render;
class Input;
class Editor;

class Program
{
	public:
		Window& windowManager = *(new Window());
		FileSystem& fileSystem = *(new FileSystem());
		TextureLoader& textureLoader = *(new TextureLoader());
		Camera& camera = *(new Camera());
		Render& render = *(new Render());
		Input& input = *(new Input());
		Editor editor = *(new Editor());
		Gui& gui = *(new Gui());

		Program(const Program&) = default;
		Program& operator=(const Program&) = default;
		Program();
		void loop();
};
