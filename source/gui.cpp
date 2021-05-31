#include <gui.h>
#include <main.h>

#include <ImGuiFileDialog.h>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <libconfig.h++>

class Program;
extern Program program;

// ran only once when either program starts
void Gui::guiInit(Window* windowManager)
{
	window = windowManager;

	IMGUI_CHECKVERSION();
	// imgui setup
	gui = ImGui::CreateContext();
	guiIO = &ImGui::GetIO();
	// set up platform / renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(windowManager->window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	// apply premade style
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.03f, 0.03f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.03f, 0.03f, 0.07f, 0.94f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.03f, 0.03f, 0.07f, 0.94f);
	colors[ImGuiCol_Border]                 = ImVec4(0.02f, 0.02f, 0.16f, 0.49f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.13f, 0.29f, 0.53f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.09f, 0.32f, 0.66f, 0.54f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.07f, 0.07f, 0.29f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.25f, 0.78f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.07f, 0.05f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.14f, 0.13f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.16f, 0.14f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.14f, 0.12f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.47f, 0.78f, 0.40f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.25f, 0.55f, 0.91f, 1.00f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.00f, 0.51f, 1.00f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator]              = ImVec4(0.04f, 0.04f, 0.17f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Gui::addEditorGui()
{
	bool* p_close = NULL; // null to hide close button!

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	// TODO: scale or make window non-resizable
	// Non-movable level editor panel in the top (or bottom?) middle containing editor tool buttons
	ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 64.0f), ImVec2(window->SCREEN_WIDTH - (s.tileSelectorPaneWidth + s.propertiesPaneWidth), window->SCREEN_HEIGHT / 8));
	ImGui::SetNextWindowSize(ImVec2(window->SCREEN_WIDTH - (s.tileSelectorPaneWidth + s.propertiesPaneWidth), s.editorPaneHeight));
	ImGui::SetNextWindowPos(ImVec2(s.tileSelectorPaneWidth, 0.0f));
	ImGui::Begin("Level editor", p_close, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoScrollbar);
		s.editorPaneHeight = ImGui::GetWindowHeight();
		/// add tool buttons
		float currentToolButtonX = 0.0f;
		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));

		// select tool button
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["select"], s.editorButtonSize))
		{
			// update (to) select tool
			program.editor.setTool(SELECT);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("select tiles with LMB ");
				ImGui::BulletText("multiselect by holding CTRL ");
				ImGui::BulletText("box select by dragging ");
				ImGui::BulletText("single tiles' properties will be shown in the properties tab ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		currentToolButtonX += 66.0f;

		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));
		// place/remove tool button
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["place"], s.editorButtonSize))
		{
			// update (to) place tool
			// not selecting a new texture since none was selected, so just reusing the last one
			program.editor.setTool(PLACE);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("place tiles with LMB or drag to draw ");
				ImGui::BulletText("hold left CTRL to place on a grid ");
				ImGui::BulletText("right click to remove tiles ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		// and so on... currentToolButtonX += 64.0f;

		/// editor toggles section
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - (64 * 2), 0.0f));
		// grid toggle button
		// TODO: texture
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["place"], s.editorButtonSize))
		{
			// TODO: toggle grid
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("toggle visible grid ");
				ImGui::BulletText("shortcut: left CTRL + G ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 64, 0.0f));
		// overlap toggle button
		// TODO: texture
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["place"], s.editorButtonSize))
		{
			// TODO: toggle overlap
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("toggle overlapping tiles ");
				ImGui::BulletText("when enabled, tiles won't be placed if they intersect another ");
				ImGui::BulletText("shortcut: left CTRL + L ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}


		ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);
	ImGui::End();
}

void Gui::addPropertiesGui()
{
	bool* p_close = NULL;
	// Right side non-movable tab containing properties of selected tile
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(window->SCREEN_WIDTH / 4, -1));
	ImGui::SetNextWindowSize(ImVec2(s.propertiesPaneWidth, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::SetNextWindowPos(ImVec2(window->SCREEN_WIDTH - s.propertiesPaneWidth, 0.0));
	ImGui::Begin("Tile properties", p_close);
		s.propertiesPaneWidth = ImGui::GetWindowWidth();
		if (program.editor.selection.size() == 1) // single tile
		{
			ImGui::Text(glm::to_string(program.editor.selection[0]->location.Position).c_str());
		}
		else if (program.editor.selection.size() > 0) // multiselect
		{
			// TODO
			ImGui::Text(("selected tile count: " + std::to_string(program.editor.selection.size())).c_str());
		}
		ImGui::Text(std::to_string(program.input.ctrl_down).c_str());
	ImGui::End();
}

void Gui::addTileSelectorGui()
{
	bool* p_close = NULL;

	ImGui::SetNextWindowSize(ImVec2(s.tileSelectorPaneWidth, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1), ImVec2(-1, -1));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	ImGui::Begin("Tile selector", p_close, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // Left side non-movable tab
		s.tileSelectorPaneWidth = ImGui::GetWindowWidth();

		// content selector button
		if (ImGui::Button("Select content folder", ImVec2(232.0f, 32.0f)))
		{
			openFileDialog(DIR);
		}

		if (tileTextures.size() > 0)
		{
			// tile tiles
			int tilesPerRow = 3;
			int total = 0;

			for (int y = 0; y < (int)(tileTextures.size() / tilesPerRow + 1); y++)
			{
				for (int x = 0; x < tilesPerRow && total < tileTextures.size(); x++)
				{
					if (x > 0)
						ImGui::SameLine();
					ImGui::PushID(y * tilesPerRow + x * tilesPerRow);

					// check if a tile was selected
					if (ImGui::ImageButton((void*)(intptr_t)tileTextures[total]->ID, ImVec2(64, 64)))
					{
						// update (to) place tool
						program.editor.placementTileTexture = program.textureLoader.getAtlasCoords(program.render.textureAtlas, total);
						program.editor.setTool(PLACE);
					}

					ImGui::PopID();
					total++;
					// return so we dont go past the vector size
					if (total >= tileTextures.size()) { break; }
				}
			}
		}

		ImGui::Text("Whatup yo yo its da windo");
	ImGui::End();
}

void Gui::addBottomBarGui()
{
	bool* p_close = NULL;

	// Bottom bar containing useful toggles and information
	ImGui::SetNextWindowSize(ImVec2(window->SCREEN_WIDTH, s.bottomBarHeight));
	//ImGui::SetNextWindowContentSize(ImVec2(32.0f, 16.0f));
	ImGui::SetNextWindowPos(ImVec2(0.0, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::Begin("Bottom bar", p_close, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		if (ImGui::Button("Load", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			openFileDialog(OPEN);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("open a BLF file to edit ");
				ImGui::BulletText("shortcut: left CTRL + I ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(s.bottomBarButtonWidth + 20.0f, 0.0f));
		if (ImGui::Button("Save", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			openFileDialog(SAVE);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("save changes to current file ");
				ImGui::BulletText("shortcut: left CTRL + O ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2((s.bottomBarButtonWidth + 20.0f) * 2, 0.0f));
		if (ImGui::Button("Save as", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			openFileDialog(SAVE_AS);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::BulletText("save currently open blur level as a file ");
				ImGui::BulletText("shortcut: left CTRL + left SHIFT + I ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - s.bottomBarButtonWidth, 0.0f));
		if (ImGui::Button("Status", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			popupToggles[STATUS] = !popupToggles[STATUS];
		}
	ImGui::End();
}

void Gui::addPopupGui()
{
	// Movable info panel showing information such as camera position, placed tile count and FPS
	// TODO: enable using close button (ImGui::CloseButton?)
	if (popupToggles[STATUS])
	{
		bool p_close = true;

		ImGui::Begin("Info panel", &p_close);
			ImGui::Text(("FPS: " + std::to_string(program.render.FPS)).c_str());
			ImGui::Text(("tile count: " + std::to_string(program.editor.tiles.size())).c_str());
			ImGui::Text(("position: " + (glm::to_string(program.camera.cameraPos))).c_str());
			ImGui::Text(("mouse pos: " + (glm::to_string(program.input.mousePos))).c_str());
		ImGui::End();
	}
	// bla bla bla
}

void Gui::openFileDialog(GUI_PROMPT type)
{
	switch (type)
	{
		case DIR:
		{
			promptType = DIR;
			program.file_system.contextOpen = true;
			ImGuiFileDialog::Instance()->OpenDialog("ChooseContentFolder", "Select the content folder", nullptr, "C:\\", 1);
			// visual settings
			ImGuiFileDialog::Instance()->SetExtentionInfos(".", ImVec4(1, 1, 0.5f, 0.9f));
			break;
		}

		case OPEN:
		{
			promptType = OPEN;
			std::string startDir = "C:\\";
			if (program.file_system.blfDir != "")
			{
				startDir = program.file_system.blfDir;
			}
			else
			{
				startDir = program.file_system.contentDir == "" ? startDir : program.file_system.contentDir;
			}
			ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Load a BLF file", ".blf", startDir.c_str(), 1);
			break;
		}

		case SAVE:
		{
			if (program.file_system.blfDir != "") // we have a file open, save changes to it
			{
				program.blf_converter.write_file(program.file_system.blfDir.c_str());
			}
			else // we need to create a new file, open Save as dialog
			{
				openFileDialog(SAVE_AS);
			}
			break;
		}

		case SAVE_AS:
		{
			promptType = SAVE_AS;
			std::string startDir = "C:\\";
			if (program.file_system.blfDir != "")
			{
				startDir = program.file_system.blfDir;
			}
			else
			{
				startDir = program.file_system.contentDir == "" ? startDir : program.file_system.contentDir;
			}

			ImGuiFileDialog::Instance()->OpenDialog("SaveFileAs", "Save current BLF file as...", ".blf", startDir.c_str(), 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
			break;
		}
	};
}

void Gui::checkFileDialog()
{
	// size constraints
	ImVec2 maxSize = ImVec2(window->SCREEN_WIDTH, window->SCREEN_HEIGHT);  // The full display area
	ImVec2 minSize = maxSize * 0.4f;

	switch (promptType)
	{
		case DIR:
		{
			if (ImGuiFileDialog::Instance()->Display("ChooseContentFolder", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					// change content dir
					program.file_system.contentDir = ImGuiFileDialog::Instance()->GetCurrentPath() + "\\";
					program.file_system.changeSetting<std::string>("dirs.content", program.file_system.contentDir, libconfig::Setting::TypeString);
					// load textures from freshly selected directory
					std::vector<std::string>& filesInContent = program.file_system.getInDir(program.file_system.contentDir.c_str());
					tileTextures = program.textureLoader.loadTextures(filesInContent, program.file_system.contentDir);
					program.render.textureAtlas = program.file_system.loadContentAsAtlas();
				}

				// close
				program.file_system.contextOpen = false;
				ImGuiFileDialog::Instance()->Close();
			}
		}
		case OPEN:
		{
			// TODO: check if correct selection
			if (ImGuiFileDialog::Instance()->Display("OpenFile", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string selectedFilename = ImGuiFileDialog::Instance()->GetCurrentFileName();
					if (selectedFilename.substr(selectedFilename.length() - 4, selectedFilename.length() - 3) != "." && selectedFilename.substr(selectedFilename.length() - 4) == ".blf")
					{
						// change currently selected blf
						program.file_system.blfDir = ImGuiFileDialog::Instance()->GetFilePathName();
						program.file_system.changeSetting<std::string>("dirs.blf", program.file_system.blfDir, libconfig::Setting::TypeString);
						// try to load the file
						program.blf_converter.load_file(ImGuiFileDialog::Instance()->GetFilePathName().c_str());

						// close
						program.file_system.contextOpen = false;
						ImGuiFileDialog::Instance()->Close();
					}
				}
				else
				{
					// close
					program.file_system.contextOpen = false;
					ImGuiFileDialog::Instance()->Close();
				}
			}
		}
		case SAVE_AS:
		{
			// TODO: check if correct selection
			if (ImGuiFileDialog::Instance()->Display("SaveFileAs", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string selectedFilename = ImGuiFileDialog::Instance()->GetCurrentFileName();
					if (selectedFilename.substr(selectedFilename.length() - 4, selectedFilename.length() - 3) != "." && selectedFilename.substr(selectedFilename.length() - 4) == ".blf")
					{
						// change currently selected blf
						program.file_system.blfDir = ImGuiFileDialog::Instance()->GetFilePathName();
						program.file_system.changeSetting<std::string>("dirs.blf", program.file_system.blfDir, libconfig::Setting::TypeString);
						// try to load the file
						program.blf_converter.write_file(ImGuiFileDialog::Instance()->GetFilePathName().c_str());

						// close
						program.file_system.contextOpen = false;
						ImGuiFileDialog::Instance()->Close();
					}
				}
				else
				{
					// close
					program.file_system.contextOpen = false;
					ImGuiFileDialog::Instance()->Close();
				}
			}
		}
	};
}

void Gui::drawGui()
{
	// new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// add guis to be rendered
	addEditorGui();
	addPropertiesGui();
	addTileSelectorGui();
	addBottomBarGui();
	addPopupGui();

	// check if any window is focused and/or hovered (for input capturing purposes)
	guiHovered = guiIO->WantCaptureMouse;
	guiFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);

	checkFileDialog();

	// TEMP
	ImGui::ShowDemoWindow();
	ImGui::Render();
	// check for right / middle click defocus
	if (gui->HoveredWindow == NULL && gui->NavWindow != NULL && (guiIO->MouseClicked[1] || guiIO->MouseClicked[2]) /* could cause issues, who cares? && GetFrontMostPopupModal() == NULL*/)
	{
		ImGui::FocusWindow(NULL);
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::setStatus(const char* status)
{
	statusMessage = status;
}
