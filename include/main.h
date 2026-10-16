#pragma once

#include <window.h>
#include <file_system.h>
#include <camera.h>
#include <render.h>
#include <input.h>
#include <editor.h>
#include <gui.h>

class Window;
class FileSystem;
class Camera;
class Render;
class Input;
class Editor;

class Program
{
	public:
		Window& windowManager = *(new Window());
		FileSystem& file_system = *(new FileSystem());
		Camera& camera = *(new Camera());
		Render& render = *(new Render());
		Input& input = *(new Input());
		Editor editor = *(new Editor());
		Gui& gui = *(new Gui());

		bool programWillClose = false;
		bool quitProgram = false;

		Program(const Program&) = default;
		Program& operator=(const Program&) = default;
		Program();
		void loop();
};
