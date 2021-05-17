#include <gui.h>
#include <main.h>

#include <ImGuiFileDialog.h>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

class Program;
extern Program program;

// ran only once when either program starts
void Gui::guiInit(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	// imgui setup
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// set up platform / renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
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
	// Gui rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool p = true;
	int tilesPerRow = 3;
	int total = 0;
	// begin specifying elements
	ImGui::SetNextWindowSize(ImVec2(64.0f * 4 + 32.0f, window->SCREEN_HEIGHT));
	ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1), ImVec2(-1, -1));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::Begin("Tile selector", &p, ImGuiWindowFlags_NoResize); // Left side non-movable tab
		if (textureAtlas != nullptr)
		{
			ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
			ImVec2 uv1 = ImVec2(1.0f, 1.0f);
			ImGui::Image((void*)(intptr_t)textureAtlas->ID, ImVec2(textureAtlas->width, textureAtlas->height) * 4, uv0, uv1);
		}

		// content selector button
		if (ImGui::Button("Select content folder", ImVec2(128.0f, 32.0f)))
		{
			program.fileSystem.contextOpen = true;
			ImGuiFileDialog::Instance()->OpenDialog("ChooseContentFolder", "Select the content folder", nullptr, "C:\\");
			// visual settings
			ImGuiFileDialog::Instance()->SetExtentionInfos(".", ImVec4(1, 1, 0.5f, 0.9f));
		}
		// tile tiles
		for (int y = 0; y < (int)(tileTextures.size() / tilesPerRow); y++)
		{
			for (int x = 0; x < tilesPerRow && total < tileTextures.size(); x++)
			{
				ImVec2 pos = ImVec2(16 + ((64 + 16) * x), 32 + ((64 + 16) * y));
				ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
				ImVec2 uv1 = ImVec2(1.0f, 1.0f);
				ImGui::SetCursorPos(pos);

				if (ImGui::ImageButton((void*)(intptr_t)tileTextures[total]->ID, ImVec2(64, 64), uv0, uv1))
				{
					program.levelView.place->selectedTile = tileTextures[total];
					program.levelView.select_tool(PLACE);
				}
				total++;
			}
		}

		ImGui::Text("Whatup yo yo its da windo");
		// create a slider for a 3D position
		static float translation[] = { 0.0, 0.0, 0.0 };
		//ImGui::SliderFloat3("position", nullptr, -2.0, 2.0);
		// create a slider for a 3D euler rotation
		static float rotation[] = { 0.0, 0.0, 0.0 };
		//ImGui::SliderFloat3("rotation", nullptr, -5.0, 5.0);

	ImGui::End();

	// Right side non-movable tab containing properties of selected tile
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(window->SCREEN_WIDTH, -1));
	ImGui::SetNextWindowSize(ImVec2(propertiesPaneWidth, window->SCREEN_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(window->SCREEN_WIDTH - propertiesPaneWidth, 0.0));
	ImGui::Begin("Tile properties");
		propertiesPaneWidth = ImGui::GetWindowSize().x;
	ImGui::End();

	// Non-movable level editor panel in the top (or bottom?) middle containing editor tool buttonss
	ImGui::Begin("Level editor");
	ImGui::End();
	// Movable info panel showing information such as camera position, placed tile count and FPS (TODO)
	ImGui::Begin("Info panel");
		ImGui::Text(("FPS: " + std::to_string(program.render.FPS)).c_str());
		ImGui::Text(("position: " + (glm::to_string(program.camera.cameraPos))).c_str());
		ImGui::Text(("tile count: " + std::to_string(program.levelView.currentTiles.size())).c_str());
	ImGui::End();
	// check if any window is focused (for input capturing purposes)
	guiFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
}

void Gui::drawGui()
{
	// size constraints
	ImVec2 maxSize = ImVec2(window->SCREEN_WIDTH, window->SCREEN_HEIGHT);  // The full display area
	ImVec2 minSize = maxSize * 0.4f;

	if (ImGuiFileDialog::Instance()->Display("ChooseContentFolder", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			program.fileSystem.contentDir = ImGuiFileDialog::Instance()->GetCurrentPath() + "\\";
			std::vector<std::string>& filesInContent = program.fileSystem.getInDir(program.fileSystem.contentDir.c_str());
			tileTextures = program.textureLoader.loadTextures(filesInContent, program.fileSystem.contentDir);
			textureAtlas = program.textureLoader.loadTextureAtlas(filesInContent, program.fileSystem.contentDir);
		}

		// close
		program.fileSystem.contextOpen = false;
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::ShowDemoWindow();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// TODO: void Gui::getGuiEvents
