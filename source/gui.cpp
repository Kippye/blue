#include <gui.h>
#include <main.h>

#include <ImGuiFileDialog.h>
#include <glm/gtx/string_cast.hpp>
#include <libconfig.h++>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <filesystem>
#include <string.h>

class Program;
extern Program program;

// ran only once when either program starts
void Gui::guiInit(Window* windowManager)
{
	std::cout << "Initializing program GUI..." << std::endl;

	window = windowManager;

	IMGUI_CHECKVERSION();
	// imgui setup
	gui = ImGui::CreateContext();
	guiIO = &ImGui::GetIO();
	// set up platform / renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(windowManager->window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
	guiIO->Fonts->AddFontFromFileTTF("./fonts/Noto_Sans_Mono/NotoSansMono-VariableFont.ttf", 16.0f);

	// apply premade style
	// ImVec4* colors = ImGui::GetStyle().Colors;
	// colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	// colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	// colors[ImGuiCol_WindowBg]               = ImVec4(0.03f, 0.03f, 0.06f, 0.94f);
	// colors[ImGuiCol_ChildBg]                = ImVec4(0.03f, 0.03f, 0.07f, 0.94f);
	// colors[ImGuiCol_PopupBg]                = ImVec4(0.03f, 0.03f, 0.07f, 0.94f);
	// colors[ImGuiCol_Border]                 = ImVec4(0.02f, 0.02f, 0.16f, 0.49f);
	// colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	// colors[ImGuiCol_FrameBg]                = ImVec4(0.13f, 0.29f, 0.53f, 0.54f);
	// colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.09f, 0.32f, 0.66f, 0.54f);
	// colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	// colors[ImGuiCol_TitleBg]                = ImVec4(0.07f, 0.07f, 0.29f, 1.00f);
	// colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.25f, 0.78f, 1.00f);
	// colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	// colors[ImGuiCol_MenuBarBg]              = ImVec4(0.07f, 0.05f, 0.22f, 1.00f);
	// colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	// colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.14f, 0.13f, 0.31f, 1.00f);
	// colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.16f, 0.14f, 0.41f, 1.00f);
	// colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.14f, 0.12f, 0.49f, 1.00f);
	// colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	// colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	// colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	// colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.47f, 0.78f, 0.40f);
	// colors[ImGuiCol_ButtonHovered]          = ImVec4(0.25f, 0.55f, 0.91f, 1.00f);
	// colors[ImGuiCol_ButtonActive]           = ImVec4(0.00f, 0.51f, 1.00f, 1.00f);
	// colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	// colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	// colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	// colors[ImGuiCol_Separator]              = ImVec4(0.04f, 0.04f, 0.17f, 0.50f);
	// colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	// colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	// colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	// colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	// colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	// colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
	// colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	// colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
	// colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	// colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	// colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	// colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	// colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	// colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	// colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	// colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	// colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	// colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	// colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	// colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	// colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	// colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	// colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	// colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	// colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.05f, 0.39f, 0.92f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.16f, 0.48f, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.39f, 0.50f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.03f, 0.27f, 0.62f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.12f, 0.40f, 0.81f, 0.71f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.24f, 0.50f, 0.88f, 0.47f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.01f, 0.00f, 0.18f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.03f, 0.00f, 0.61f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.10f, 0.12f, 0.27f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.13f, 0.15f, 0.49f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.20f, 0.22f, 0.56f, 0.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.23f, 0.25f, 0.58f, 0.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.92f, 0.96f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button]                 = ImVec4(0.04f, 0.32f, 0.73f, 0.54f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.12f, 0.40f, 0.81f, 0.31f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.24f, 0.50f, 0.88f, 0.12f);
	colors[ImGuiCol_Header]                 = ImVec4(0.01f, 0.03f, 1.00f, 0.57f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.15f, 0.17f, 1.00f, 0.27f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.31f, 0.33f, 0.99f, 0.16f);
	colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
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

	gd.normalButtonColor = colors[ImGuiCol_Button];

	std::cout << "Program GUI initialized" << std::endl;
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
	ImGui::Begin("Level editor", p_close, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav);
		s.editorPaneHeight = ImGui::GetWindowHeight();
		/// add tool buttons
		float currentToolButtonX = 0.0f;
		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));

		if (tileTextures.size() == 0) 
		{
			ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding); 
			ImGui::End(); 
			return;
		}

		// select tool button
		if (program.editor.getTool() == SELECT)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.selectedButtonColor);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.normalButtonColor);
		}
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["select"], s.editorButtonSize))
		{
			// update (to) select tool
			program.editor.setTool(SELECT);
		}
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("select tool ");
				ImGui::BulletText("select tiles with LMB (hold SHIFT to add to selection) ");
				ImGui::BulletText("add / remove single tiles from selection with RMB ");
				ImGui::BulletText("box select by dragging LMB (hold SHIFT to add to selection) ");
				ImGui::BulletText("selected tiles' properties will be shown in the properties tab ");
				ImGui::BulletText("shortcut: 1 ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		currentToolButtonX += s.editorButtonDistance + s.editorButtonOffset;

		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));
		// place/remove tool button
		if (program.editor.getTool() == PLACE)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.selectedButtonColor);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.normalButtonColor);
		}
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["place"], s.editorButtonSize))
		{
			// update (to) place tool
			program.editor.setTool(PLACE);
		}
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("place tool ");
				ImGui::BulletText("place tiles with LMB (hold left CTRL to snap to grid) ");
				ImGui::BulletText("tiles will be created with the options set in the new tile options gui ");
				ImGui::BulletText("remove tiles with RMB ");
				ImGui::BulletText("shortcut: 2 ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		currentToolButtonX += s.editorButtonDistance + s.editorButtonOffset;

		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));
		// box draw tool button
		if (program.editor.getTool() == BOX)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.selectedButtonColor);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.normalButtonColor);
		}
		if (ImGui::ImageButton((void*)(intptr_t)guiTextures["box"], s.editorButtonSize))
		{
			// update (to) box tool
			program.editor.setTool(BOX);
		}
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("box draw tool ");
				ImGui::BulletText("drag a box with lmb to create a tile that size ");
				ImGui::BulletText("drag a box with rmb to fill the area with tiles using the size in new tile options ");
				ImGui::BulletText("hold left CTRL to snap the box start and end points to the grid ");
				ImGui::BulletText("shortcut: 3 ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		currentToolButtonX += s.editorButtonDistance + s.editorButtonOffset;

		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));

		/// editor buttons section
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, guiOpacities.at(std::string("Select by texture"))->current);
		if (program.editor.getTool() == SELECT)
		{
			GuiTween* gt = guiOpacities.at(std::string("Select by texture"));
			if (gt->enabled == false && gt->current != 1.0f)
			{
				guiOpacities.at(std::string("Select by texture"))->Start(0.0f, 1.0f, 0.25f);
			}
			ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));
			ImGui::SetNextItemWidth(s.editorComboWidth);
			// texture select button
			if (ImGui::Button("Select by texture", ImVec2(s.editorComboWidth, s.editorButtonSize.y / 2)))
			{
				program.editor.select_by_texture(program.editor.nextTile.visuals.textureName);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("select by texture ");
				ImGui::BulletText("selects every tile with the texture currently chosen in the texture selector ");
				ImGui::BulletText("shortcut: left CTRL + T ");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::SetCursorPos(ImVec2(currentToolButtonX, s.editorButtonSize.y / 2 + 3.5f));
			ImGui::SetNextItemWidth(s.editorComboWidth);
			// select all button
			if (ImGui::Button("Select all", ImVec2(s.editorComboWidth, s.editorButtonSize.y / 2)))
			{
				program.editor.select_all();
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("select all ");
				ImGui::BulletText("selects every existing tile ");
				ImGui::BulletText("shortcut: left CTRL + 1 ");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
		else
		{
			GuiTween* gt = guiOpacities.at(std::string("Select by texture"));
			if (gt->enabled == false && gt->current != 0.0f)
			{
				// TEMP: since it gets hidden instantly anyway, it just tweens instantly
				gt->Start(1.0f, 0.0f, 0.0f);
			}
		}
		ImGui::PopStyleVar(1);

		currentToolButtonX += s.editorComboWidth + s.editorButtonOffset;

		ImGui::SetCursorPos(ImVec2(currentToolButtonX, 0.0f));

		ImGui::SetNextItemWidth(s.editorComboWidth);
		// push to back button
		if (ImGui::Button("Copy", ImVec2(s.editorComboWidth, s.editorButtonSize.y / 2)))
		{
			program.editor.copy_selection();
		}

		ImGui::SetCursorPos(ImVec2(currentToolButtonX, s.editorButtonSize.y / 2 + 3.5f));

		ImGui::SetNextItemWidth(s.editorComboWidth);
		// push to back button
		if (ImGui::Button("Cut", ImVec2(s.editorComboWidth, s.editorButtonSize.y / 2)))
		{
			program.editor.cut_selection();
		}

		currentToolButtonX += s.editorComboWidth + s.editorButtonOffset;

		ImGui::SetCursorPos(ImVec2(currentToolButtonX + 4.0f, 0.0f));
		ImGui::SetNextItemWidth(s.editorComboWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		bool _perspectiveEnabled = program.editor.perspectiveEnabled;
		if (ImGui::Selectable("Perspective", &_perspectiveEnabled, 0, ImVec2(s.editorComboWidth - 8.0f, s.editorButtonSize.y / 2 - 4.0f)))
		{
			program.editor.set_perspective_enabled(_perspectiveEnabled);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("perspective ");
			ImGui::BulletText("if enabled, a perspective camera projection is used ");
			ImGui::BulletText("if disabled, an orthographic camera projection is used ");
			ImGui::BulletText("shortcut: left CTRL + P ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::PopStyleVar();
		ImGui::SameLine();
		if (ImGui::Checkbox("##pe", &_perspectiveEnabled))
		{
			program.editor.set_perspective_enabled(_perspectiveEnabled);
		}

		ImGui::SetCursorPos(ImVec2(currentToolButtonX + 4.0f, s.editorButtonSize.y / 2 + 5.5f));
		ImGui::SetNextItemWidth(s.editorComboWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		bool _gridVisible = program.editor.gridVisible;
		if (ImGui::Selectable("Show grid", &_gridVisible, 0, ImVec2(s.editorComboWidth - 8.0f, s.editorButtonSize.y / 2 - 4.0f)))
		{
			program.editor.set_grid_visible(_gridVisible);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("grid ");
			ImGui::BulletText("shows or hides the grid ");
			ImGui::BulletText("shortcut: G ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::PopStyleVar();
		ImGui::SameLine();
		if (ImGui::Checkbox("##gv", &_gridVisible))
		{
			program.editor.set_grid_visible(_gridVisible);
		}

		/// editor toggles section
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 256, 0.0f));
		ImGui::SetNextItemWidth(s.editorComboWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		ImGui::Selectable("Overlap", &program.editor.overlap, 0, ImVec2(s.editorComboWidth, s.editorButtonSize.y / 2));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("overlap ");
			ImGui::BulletText("whether or not new tiles are allowed to overlap pre-existing ones ");
			ImGui::BulletText("shortcut: left CTRL + L ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::PopStyleVar();
		ImGui::SameLine();
		if (ImGui::Checkbox("##ol", &program.editor.overlap)){}

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 256, s.editorButtonSize.y / 2 + 3.5f));
		ImGui::SetNextItemWidth(s.editorComboWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
		ImGui::Selectable("Auto-snap", &program.editor.autosnap, 0, ImVec2(s.editorComboWidth, s.editorButtonSize.y / 2));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("auto-snap ");
			ImGui::BulletText("essentialy holds left CTRL automatically to snap positions and sizes to the grid ");
			ImGui::BulletText("shortcut: left CTRL + K ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::PopStyleVar();
		ImGui::SameLine();
		if (ImGui::Checkbox("##as", &program.editor.autosnap)){}
		
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

	ImGui::Begin("Properties", p_close, ImGuiWindowFlags_NoNav);
		s.propertiesPaneWidth = ImGui::GetWindowWidth();
		guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

		if (ImGui::BeginTabBar("Tabs"))
            {
                if (ImGui::BeginTabItem("Tiles"))
                {
					/// SELECTION SECTION
					std::vector<E_Tile*>& selection = program.editor.selection;
					if (selection.size() == 1) // single tile
					{
						if (showDebugInfo) { ImGui::Text(("ID: " + std::to_string(selection[0]->ID)).c_str()); }
						/// location
						ImGui::Text("Transform");
						float pos[3] = { selection[0]->location.Position.x, selection[0]->location.Position.y, selection[0]->location.Position.z };
						// position
						if (ImGui::DragFloat3("Position", pos))
						{
							program.editor.moveTile(selection[0]->ID, glm::vec3(pos[0], pos[1], pos[2]));
						}
						float size[2] = { selection[0]->location.Size.x, selection[0]->location.Size.y };
						// size
						if (ImGui::DragFloat2("Size", size))
						{
							program.editor.resizeTile(selection[0]->ID, glm::vec2(size[0], size[1]));
						}
						float angle[1] = { mymath::deg(selection[0]->location.Angle) };
						// angle
						if (ImGui::DragFloat("Angle", angle, 1.0F, 0.0f, 360.0f))
						{
							program.editor.rotateTile(selection[0]->ID, mymath::rad((double)*angle));
						}

						/// physics
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, s.propertySectionSeparator));
						ImGui::Text("Physics");
						ImGui::Checkbox("CollisionsEnabled", &selection[0]->physics.CollisionsEnabled);
						ImGui::Checkbox("Static", &selection[0]->physics.Static);
						ImGui::DragFloat("Bounce", &selection[0]->physics.Bounce, 0.1F);
						ImGui::DragFloat("Density", &selection[0]->physics.Density, 0.1F);
						ImGui::DragFloat("Friction", &selection[0]->physics.Friction, 0.1F);

						/// visual
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, s.propertySectionSeparator));
						ImGui::Text("Visual");
						// texture icon
						int tileTextureIndex = 0;
						for (int i = 0; i < tileTextures.size(); i++)
						{
							if (selection[0]->visuals.textureName == tileTextures[i]->path)
							{
								tileTextureIndex = i;
								break;
							}
						}
						ImGui::Image((void*)(intptr_t)tileTextures[tileTextureIndex]->ID, ImVec2(64, 64));
						// texture name
						ImGui::Text(("Texture: " + selection[0]->visuals.textureName + " (" + std::to_string(selection[0]->visuals.atlasLocation.z) + "x" + std::to_string(selection[0]->visuals.atlasLocation.w) + ")").c_str());
						// texturemode
						if (ImGui::Combo("TextureMode", &(int)selection[0]->visuals.TextureMode, se.tileTextureModeOptions, 2))
						{
							program.editor.updateTileVisuals(selection[0]->ID);
						}
						float textureSize[2] = { selection[0]->visuals.TextureSize.x, selection[0]->visuals.TextureSize.y };
						// TextureSize
						if (ImGui::DragFloat2("TextureSize", textureSize))
						{
							selection[0]->visuals.TextureSize.x = textureSize[0];
							selection[0]->visuals.TextureSize.y = textureSize[1];
							program.editor.updateTileVisuals(selection[0]->ID);
						}
						float color[3] = { selection[0]->visuals.Color.x, selection[0]->visuals.Color.y, selection[0]->visuals.Color.z };
						// color
						if (ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB))
						{
							selection[0]->visuals.Color.x = color[0];
							selection[0]->visuals.Color.y = color[1];
							selection[0]->visuals.Color.z = color[2];
							program.editor.updateTileVisuals(selection[0]->ID);
						}
						float opacity[1] = { selection[0]->visuals.Opacity };
						// opacity
						if (ImGui::DragFloat("Opacity", opacity, 0.1F, 0.0f, 1.0f))
						{
							selection[0]->visuals.Opacity = *opacity;
							program.editor.updateTileVisuals(selection[0]->ID);
						}

						/// tags
						if (ImGui::CollapsingHeader("Tags")) 
						{
							for (size_t i = 0; i < MAX_TAGS; i++)
							{
								// declaring character array (+1 for null terminator)
								char* buf = new char[program.editor.tags[i].length() + 1];
							
								// copying the contents of the
								// string to char array
								strcpy(buf, program.editor.tags[i].c_str());

								if (ImGui::InputText(std::string("##").append(std::to_string(i)).c_str(), buf, 32))
								{
									program.editor.tags[i].assign(buf);
								}

								ImGui::SameLine();

								if (ImGui::Checkbox(std::string("##c").append(std::to_string(i)).c_str(), &selection[0]->tags[i]))
								{

								}
							}
							se.tagsVisible = true;
						}
						else
						{
							se.tagsVisible = false;
						}
					}
					else if (selection.size() > 0) // multiselect
					{
						ImGui::Text(("selected tile count: " + std::to_string(program.editor.selection.size())).c_str());
						/// location
						ImGui::Text("Transform");
						float pos[3] = { 0.0f, 0.0f, 0.0f };
						// position
						if (ImGui::InputFloat3("Offset", pos, "", ImGuiInputTextFlags_EnterReturnsTrue))
						{
							// add this offset to every selected tile's position
							program.editor.moveSelectedTiles(glm::vec3(pos[0], pos[1], pos[2]));

							pos[0] = 0.0f, pos[1] = 0.0f, pos[2] = 0.0f;
						}
						float size[2] = { selection[0]->location.Size.x, selection[0]->location.Size.y };
						// size
						if (ImGui::DragFloat2("Size", size))
						{
							program.editor.resizeSelectedTiles(glm::vec2(size[0], size[1]));
						}
						float angle[1] = { mymath::deg(selection[0]->location.Angle) };
						// angle
						if (ImGui::DragFloat("Angle", angle, 1.0F, 0.0f, 360.0f))
						{
							program.editor.rotateSelectedTiles(mymath::rad((double)*angle));
						}

						/// physics
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, s.propertySectionSeparator));
						ImGui::Text("Physics");
						if (ImGui::Checkbox("CollisionsEnabled", &selection[0]->physics.CollisionsEnabled))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->physics.CollisionsEnabled = selection[0]->physics.CollisionsEnabled;
							}
						}			
						if (ImGui::Checkbox("Static", &selection[0]->physics.Static))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->physics.Static = selection[0]->physics.Static;
							}
						}
						if (ImGui::DragFloat("Bounce", &selection[0]->physics.Bounce, 0.1F))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->physics.Bounce = selection[0]->physics.Bounce;
							}
						}
						if (ImGui::DragFloat("Density", &selection[0]->physics.Density, 0.1F))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->physics.Density = selection[0]->physics.Density;
							}
						}
						if (ImGui::DragFloat("Friction", &selection[0]->physics.Friction, 0.1F))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->physics.Friction = selection[0]->physics.Friction;
							}
						}

						/// visual
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, s.propertySectionSeparator));
						ImGui::Text("Visual");

						// texturemode
						if (ImGui::Combo("TextureMode", &(int)selection[0]->visuals.TextureMode, se.tileTextureModeOptions, 2))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->visuals.TextureMode = selection[0]->visuals.TextureMode;
							}
							program.editor.updateSelectedTilesVisuals();
						}

						float textureSize[2] = { selection[0]->visuals.TextureSize.x, selection[0]->visuals.TextureSize.y };
						// TextureSize
						if (ImGui::DragFloat2("TextureSize", textureSize))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->visuals.TextureSize.x = textureSize[0];
								selection[i]->visuals.TextureSize.y = textureSize[1];
							}
							program.editor.updateSelectedTilesVisuals();
						}

						float color[3] = { selection[0]->visuals.Color.x, selection[0]->visuals.Color.y, selection[0]->visuals.Color.z };
						// color
						if (ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->visuals.Color.x = color[0];
								selection[i]->visuals.Color.y = color[1];
								selection[i]->visuals.Color.z = color[2];
							}
							program.editor.updateSelectedTilesVisuals();
						}

						float opacity[1] = { selection[0]->visuals.Opacity };
						// opacity
						if (ImGui::DragFloat("Opacity", opacity, 0.1F, 0.0f, 1.0f))
						{
							for (size_t i = 0; i < selection.size(); i++)
							{
								selection[i]->visuals.Opacity = *opacity;
							}
							program.editor.updateSelectedTilesVisuals();
						}

						/// tags
						if (ImGui::CollapsingHeader("Tags")) 
						{
							for (size_t i = 0; i < MAX_TAGS; i++)
							{
								// declaring character array (+1 for null terminator)
								char* buf = new char[program.editor.tags[i].length() + 1];
							
								// copying the contents of the
								// string to char array
								strcpy(buf, program.editor.tags[i].c_str());

								if (ImGui::InputText(std::string("##").append(std::to_string(i)).c_str(), buf, 32))
								{
									program.editor.tags[i].assign(buf);
								}

								ImGui::SameLine();

								if (ImGui::Checkbox(std::string("##c").append(std::to_string(i)).c_str(), &selection[0]->tags[i]))
								{
									for (size_t j = 1; j < selection.size(); j++)
									{
										selection[j]->tags[i] = selection[0]->tags[i];
									}
								}
							}
							se.tagsVisible = true;
						}
						else
						{
							se.tagsVisible = false;
						}
					}
					// 0 tiles selected
					else 
					{
						ImGui::Text("no tiles selected");
					}

					if (selection.size() == 0)
					{
						ImGui::SetCursorPosY(558.0f);
					}
					else if (selection.size() > 1)
					{
						if (se.tagsVisible == false)
						{
							ImGui::SetCursorPosY(558.0f - 64.0f - 4.0f);
						}
						else
						{
							ImGui::SetCursorPosY(558.0f + (MAX_TAGS * (22.0f + 4.0f) + 4.0f) - 64.0f - 4.0f);
						}
					}
					else if (selection.size() == 1)
					{
						if (se.tagsVisible == false)
						{
							ImGui::SetCursorPosY(600.0f);
						}
						else
						{
							ImGui::SetCursorPosY(600.0f + MAX_TAGS * (22.0f + 4.0f) + 4.0f);
						}
					}

					/// OPTIONS SECTION
					if (program.render.textureAtlas != nullptr && ImGui::CollapsingHeader("New tile options"))
					{
						ImGui::Text("Transform");
						ImGui::SameLine();
						// reset button
						if (ImGui::Button("Default settings"))
						{
							program.editor.reset_next_tile();
						}

						/// location
						float posZ = program.editor.nextTile.location.Position.z;
						// z position
						if (ImGui::DragFloat("Z Position##m", &posZ))
						{
							program.editor.nextTile.location.Position.z = posZ;
						}
						/// location
						float size[2] = { program.editor.nextTile.location.Size.x, program.editor.nextTile.location.Size.y };
						// size
						if (ImGui::DragFloat2("Size##m", size))
						{
							program.editor.nextTile.location.Size.x = size[0];
							program.editor.nextTile.location.Size.y = size[1];
							if (program.editor.getTool() == PLACE)
							{
								if (program.editor.placeCursorID != -1)
								{
									int index = program.editor.ID_to_gizmo_index(program.editor.placeCursorID);
									program.editor.resizeGizmo(index, program.editor.nextTile.location.Size);
								}
							}
						}
						float angle[1] = { mymath::deg(program.editor.nextTile.location.Angle) };
						// angle
						if (ImGui::DragFloat("Angle##", angle, 1.0F, 0.0f, 360.0f))
						{
							program.editor.nextTile.location.Angle = mymath::rad((double)*angle);
						}

						/// physics
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, s.propertySectionSeparator));
						ImGui::Text("Physics");
						ImGui::Checkbox("CollisionsEnabled##m", &program.editor.nextTile.physics.CollisionsEnabled);
						ImGui::Checkbox("Static##m", &program.editor.nextTile.physics.Static);
						ImGui::DragFloat("Bounce##m", &program.editor.nextTile.physics.Bounce, 0.1F);
						ImGui::DragFloat("Density##m", &program.editor.nextTile.physics.Density, 0.1F);
						ImGui::DragFloat("Friction##m", &program.editor.nextTile.physics.Friction, 0.1F);

						/// visual
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, s.propertySectionSeparator));
						ImGui::Text("Visual");
						// texture icon
						int tileTextureIndex = 0;
						for (int i = 0; i < tileTextures.size(); i++)
						{
							if (program.editor.nextTile.visuals.textureName == tileTextures[i]->path)
							{
								tileTextureIndex = i;
								break;
							}
						}
						ImGui::Image((void*)(intptr_t)tileTextures[tileTextureIndex]->ID, ImVec2(64, 64));
						// texture name
						ImGui::Text(("Texture: " + program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, program.editor.nextTile.visuals.atlasLocation) + " (" + std::to_string(program.editor.nextTile.visuals.atlasLocation.z) + "x" + std::to_string(program.editor.nextTile.visuals.atlasLocation.w) + ")").c_str());
						// texturemode
						ImGui::Combo("TextureMode##m", &se.currentTextureModeSelection, se.tileTextureModeOptions, 2);
						program.editor.nextTile.visuals.TextureMode = (TEXTUREMODE)se.currentTextureModeSelection;
						if (program.editor.placeCursorID != -1)
						{
							int index;
							Gizmo* placeCursor = program.editor.ID_to_gizmo(program.editor.placeCursorID, index);
							placeCursor->visuals.TextureMode = program.editor.nextTile.visuals.TextureMode;
							program.editor.updateGizmoVisuals(index);
						}
						float textureSize[2] = { program.editor.nextTile.visuals.TextureSize.x, program.editor.nextTile.visuals.TextureSize.y };
						// TextureSize
						if (ImGui::DragFloat2("TextureSize##m", textureSize))
						{
							program.editor.nextTile.visuals.TextureSize.x = textureSize[0];
							program.editor.nextTile.visuals.TextureSize.y = textureSize[1];
							if (program.editor.placeCursorID != -1)
							{
								int index;
								Gizmo* placeCursor = program.editor.ID_to_gizmo(program.editor.placeCursorID, index);
								placeCursor->visuals = program.editor.nextTile.visuals;
								program.editor.updateGizmoVisuals(index);
							}
						}
						float color[3] = { program.editor.nextTile.visuals.Color.x, program.editor.nextTile.visuals.Color.y, program.editor.nextTile.visuals.Color.z };
						// color
						if (ImGui::ColorEdit3("Color##m", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB))
						{
							program.editor.nextTile.visuals.Color.x = color[0];
							program.editor.nextTile.visuals.Color.y = color[1];
							program.editor.nextTile.visuals.Color.z = color[2];
							if (program.editor.placeCursorID != -1)
							{
								int index;
								Gizmo* placeCursor = program.editor.ID_to_gizmo(program.editor.placeCursorID, index);
								placeCursor->visuals = program.editor.nextTile.visuals;
								program.editor.updateGizmoVisuals(index);
							}
						}
						float opacity[1] = { program.editor.nextTile.visuals.Opacity };
						// opacity
						if (ImGui::DragFloat("Opacity##m", opacity, 0.1F, 0.0f, 1.0f))
						{
							program.editor.nextTile.visuals.Opacity = opacity[0];
							if (program.editor.placeCursorID != -1)
							{
								int index;
								Gizmo* placeCursor = program.editor.ID_to_gizmo(program.editor.placeCursorID, index);
								placeCursor->visuals.Opacity = program.editor.nextTile.visuals.Opacity;
								program.editor.updateGizmoVisuals(index);
							}
						}

						if (ImGui::CollapsingHeader("Tags")) 
						{
							for (int i = 0; i < MAX_TAGS; i++)
							{
								// declaring character array (+1 for null terminator)
								char* buf = new char[program.editor.tags[i].length() + 1];
							
								// copying the contents of the
								// string to char array
								strcpy(buf, program.editor.tags[i].c_str());

								if (ImGui::InputText(std::string("##m").append(std::to_string(i)).c_str(), buf, 32))
								{
									program.editor.tags[i].assign(buf);
								}

								ImGui::SameLine();

								if (ImGui::Checkbox(std::string("##cm").append(std::to_string(i)).c_str(), &program.editor.nextTile.tags[i]))
								{

								}
							}
							se.tagsVisible = true;
						}
						else
						{
							se.tagsVisible = false;
						}
						se.newTileOptionsVisible = !se.newTileOptionsVisible;
					}
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Editor"))
				{
					float color[3] = { program.editor.backgroundColor.x, program.editor.backgroundColor.y, program.editor.backgroundColor.z };
					// color
					if (ImGui::ColorEdit3("Background color", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB))
					{
						program.editor.backgroundColor.x = color[0];
						program.editor.backgroundColor.y = color[1];
						program.editor.backgroundColor.z = color[2];
						
						program.file_system.update_editor_config();
					}

					if (program.render.textureAtlas == nullptr)
					{
						ImGui::PushStyleColor(ImGuiCol_FrameBg, gd.buttonDisabled);
						ImGui::PushStyleColor(ImGuiCol_FrameBgActive, gd.buttonDisabledActive);
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, gd.buttonDisabledHovered);
					}
					float gridSize[1] = { mymath::gridSize };
					// griddy size
					if (ImGui::DragFloat("Grid size", gridSize, 0.05F, 0.05f, 100.0f))
					{
						if (program.render.textureAtlas == nullptr)
						{
							popupToggles[CONTENT_LACK_WARNING] = true;
						}
						else
						{
							popupToggles[CONTENT_LACK_WARNING] = false;
							program.editor.update_grid_size(std::clamp(gridSize[0], 0.05f, 100.0f));
						}
					}
					if (program.render.textureAtlas == nullptr)
					{
						ImGui::PopStyleColor(3);
					}
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

		
	ImGui::End();
}

void Gui::addTextureSelectorGui()
{
	gd.displayedTextureButtons = std::min(gd.displayedTextureButtons + 1, (int)tileTextures.size());
	bool* p_close = NULL;

	ImGui::SetNextWindowSize(ImVec2(s.tileSelectorPaneWidth, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1), ImVec2(-1, -1));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	ImGui::Begin("Texture selector", p_close, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav); // Left side non-movable tab
		s.tileSelectorPaneWidth = ImGui::GetWindowWidth();
		std::vector<E_Tile*>& selection = program.editor.selection;
		// content selector button
		if (ImGui::Button("Select content folder", ImVec2(232.0f, 32.0f)))
		{
			openFileDialog(DIR);
		}

		ImGui::Text(("textures: " + std::to_string(tileTextures.size())).c_str());

		if (tileTextures.size() > 0)
		{
			// tile tiles
			int tilesPerRow = 3;
			size_t total = 0;

			for (int y = 0; y < (int)(tileTextures.size() / tilesPerRow + 1); y++)
			{
				for (int x = 0; x < tilesPerRow && total < tileTextures.size(); x++)
				{
					// TODO: fix the bug where this causes a bug if (total >= gd.displayedTextureButtons) { break; }
					if (x > 0)
						ImGui::SameLine();
					ImGui::PushID(y * tilesPerRow + x * tilesPerRow);

					if (se.currentTextureSelection == total)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, gd.selectedButtonColor);
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Button, gd.normalButtonColor);
					}

					// check if this texture was selected
					if (ImGui::ImageButton((void*)(intptr_t)tileTextures[total]->ID, ImVec2(64, 64)))
					{
						// change texture of selection (pretty crappy but works ig)
						for (size_t i = 0; i < selection.size(); i++)
						{
							selection[i]->visuals.atlasLocation.x = tileTextures[total]->atlasLocation.x;
							selection[i]->visuals.atlasLocation.y = tileTextures[total]->atlasLocation.y;
							selection[i]->visuals.atlasLocation.z = tileTextures[total]->atlasLocation.z;
							selection[i]->visuals.atlasLocation.w = tileTextures[total]->atlasLocation.w;
							selection[i]->visuals.textureName = tileTextures[total]->path;
							program.editor.updateTileVisuals(selection[i]->ID);
						}
						// update nexttile regardless
						program.editor.nextTile.visuals.atlasLocation = glm::uvec4(tileTextures[total]->atlasLocation);
						program.editor.nextTile.visuals.textureName = tileTextures[total]->path;
						// add highlight to see which is selected
						se.currentTextureSelection = total; //program.textureLoader.getAtlasTextureIndex(program.render.textureAtlas, program.editor.nextTile.visuals.atlasLocation);
						// update place cursor
						if (program.editor.placeCursorID != -1)
						{
							int index;
							Gizmo* placeCursor = program.editor.ID_to_gizmo(program.editor.placeCursorID, index);
							placeCursor->visuals.atlasLocation = program.editor.nextTile.visuals.atlasLocation;
							placeCursor->visuals.textureName = program.editor.nextTile.visuals.textureName;
							program.editor.updateGizmoVisuals(index);
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::Text(tileTextures[total]->path.c_str());
							ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					ImGui::PopStyleColor();
					ImGui::PopID();
					total++;
					// return so we dont go past the vector size
					if (total >= tileTextures.size()) { break; }
				}
			}
		}
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
		
		if (program.render.textureAtlas == nullptr)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gd.buttonDisabled);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, gd.buttonDisabledActive);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, gd.buttonDisabledHovered);
		}
		if (ImGui::Button("New", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			if (program.render.textureAtlas == nullptr)
			{
				popupToggles[CONTENT_LACK_WARNING] = true;
			}
			else
			{
				popupToggles[CONTENT_LACK_WARNING] = false;
				// save changes first?
				if (program.editor.getDirtiness())
				{
					program.gui.popupToggles[SAVE_CONTEXT] = true;
				}
				else
				{
					program.file_system.startNewFile();
				}
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("start a new unsaved BLF file ");
			ImGui::BulletText("shortcut: left CTRL + N ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(s.bottomBarButtonWidth + 20.0f, 0.0f));
		if (ImGui::Button("Load", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			if (program.render.textureAtlas == nullptr)
			{
				popupToggles[CONTENT_LACK_WARNING] = true;
			}
			else
			{
				popupToggles[CONTENT_LACK_WARNING] = false;
				openFileDialog(OPEN);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("open a BLF file to edit ");
				ImGui::BulletText("shortcut: left CTRL + I ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2((s.bottomBarButtonWidth + 20.0f) * 2, 0.0f));
		if (ImGui::Button("Save", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			if (program.render.textureAtlas == nullptr)
			{
				popupToggles[CONTENT_LACK_WARNING] = true;
			}
			else
			{
				popupToggles[CONTENT_LACK_WARNING] = false;
				openFileDialog(SAVE);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("save changes to current file ");
				ImGui::BulletText("shortcut: left CTRL + O ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2((s.bottomBarButtonWidth + 20.0f) * 3, 0.0f));
		if (ImGui::Button("Save as", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			if (program.render.textureAtlas == nullptr)
			{
				popupToggles[CONTENT_LACK_WARNING] = true;
			}
			else
			{
				popupToggles[CONTENT_LACK_WARNING] = false;
				openFileDialog(SAVE_AS);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("save currently open blur level as a file ");
				ImGui::BulletText("shortcut: left CTRL + left SHIFT + O ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - ((s.bottomBarButtonWidth * 4) + 60), 0.0f));
		if (ImGui::Button("Reload textures", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			if (program.render.textureAtlas == nullptr)
			{
				popupToggles[CONTENT_LACK_WARNING] = true;
			}
			else
			{
				popupToggles[CONTENT_LACK_WARNING] = false;
				program.file_system.updateTextures();
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("reload textures from the content folder ");
			ImGui::BulletText("applies any changes made to the content folder ");
			ImGui::BulletText("tries to give tiles textures with the same name ");
			ImGui::BulletText("shortcut: left CTRL + R ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		if (program.render.textureAtlas == nullptr)
		{
			ImGui::PopStyleColor(3);
		}

		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - ((s.bottomBarButtonWidth * 3) + 40), 0.0f));
		if (ImGui::Button("Reset camera", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			program.camera.cameraPos = glm::vec3(0, 0, program.camera.blurZoomLevel);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("reset the camera's position to the default in BlurEngine ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - ((s.bottomBarButtonWidth * 2) + 20), 0.0f));
		if (ImGui::Button("Ignore list", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			program.file_system.vectorToIgnoreBuffer();
			popupToggles[IGNORE_LIST] = !popupToggles[IGNORE_LIST];
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("enter names of folders or files to ignore when loading textures ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - s.bottomBarButtonWidth, 0.0f));
		if (ImGui::Button("About", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			popupToggles[ABOUT] = !popupToggles[ABOUT];
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("information about the editor ");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	ImGui::End();
}

void Gui::addPopupGui()
{
	ImVec2 centeredPos = ImVec2(program.windowManager.SCREEN_WIDTH / 2, program.windowManager.SCREEN_HEIGHT / 2);

	// Movable info panel showing information such as camera position, placed tile count and FPS
	if (popupToggles[ABOUT])
	{
		if (ImGui::Begin("About", &popupToggles[ABOUT], ImGuiWindowFlags_AlwaysAutoResize))
		{
			guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

			ImGui::Text(("FPS: " + std::to_string(program.render.FPS)).c_str());
			ImGui::Text(("tile count: " + std::to_string(program.editor.tiles.size())).c_str());
			ImGui::Text(("position: " + (glm::to_string(program.camera.cameraPos))).c_str());
			ImGui::Text(("mouse pos: " + (glm::to_string(program.input.mousePos))).c_str());
			ImGui::Text(("instance updates / frame: " + std::to_string(program.render.instanceDataUpdates)).c_str());
			ImGui::Text(("blue version: " + std::string(BLUE_VERSION) + (BLUE_EXPERIMENTAL ? " (exp)" : "")).c_str());
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	if (popupToggles[IGNORE_LIST])
	{
		if (ImGui::Begin("Ignore list", &popupToggles[IGNORE_LIST], ImGuiWindowFlags_AlwaysAutoResize))
		{
			guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

			ImGui::InputTextMultiline("filenames & directories to ignore", program.file_system.ignoreListBuffer, IM_ARRAYSIZE(program.file_system.ignoreListBuffer), ImVec2(600, 400));
			if (ImGui::Button("Apply", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				program.file_system.ignore_buffer_to_vector();
				program.file_system.updateTextures();
			}
			ImGui::SameLine();
			if (ImGui::Button("Default", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				std::copy(std::begin(program.file_system.defaultIgnoreListBuffer), std::end(program.file_system.defaultIgnoreListBuffer), std::begin(program.file_system.ignoreListBuffer));
			}
			ImGui::SameLine();
			if (ImGui::Button("Clear", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				std::fill(std::begin(program.file_system.ignoreListBuffer), std::end(program.file_system.ignoreListBuffer), '\0');
			}
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	if (popupToggles[SAVE_CONTEXT])
	{
		ImGui::SetNextWindowPos(centeredPos);
		if (ImGui::Begin("Unsaved changes", &popupToggles[SAVE_CONTEXT], ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

			if (program.programWillClose)
			{
				if (ImGui::Button("Save and exit", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
				{
					openFileDialog(SAVE);
				}
			}
			else
			{
				if (ImGui::Button("Save", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
				{
					openFileDialog(SAVE);
				}
			}
			ImGui::SameLine();
			// clicking the exit button will cause the program to close.
			if (program.programWillClose)
			{
				if (ImGui::Button("Exit", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
				{
					popupToggles[SAVE_CONTEXT] = false;
					program.quitProgram = true;
				}
			}
			else
			{
				if (ImGui::Button("Discard", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
				{
					popupToggles[SAVE_CONTEXT] = false;
					program.file_system.startNewFile();
				}
			}
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	if (popupToggles[REOPEN_CONTEXT])
	{
		ImGui::SetNextWindowPos(centeredPos, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(("Reopen file (" + std::filesystem::path(program.file_system.blfFile).filename().string() + ")?").c_str(), &popupToggles[REOPEN_CONTEXT], ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

			if (program.render.textureAtlas == nullptr)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, gd.buttonDisabled);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, gd.buttonDisabledActive);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, gd.buttonDisabledHovered);
			}
			if (ImGui::Button("Yes", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				if (program.render.textureAtlas == nullptr)
				{
					popupToggles[CONTENT_LACK_WARNING] = true;
				}
				else
				{
					popupToggles[CONTENT_LACK_WARNING] = false;
					popupToggles[REOPEN_CONTEXT] = false;
					program.blf_converter.load_file(program.file_system.blfFile.c_str());
				}
			}
			if (program.render.textureAtlas == nullptr)
			{
				ImGui::PopStyleColor(3);
			}
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(128.0F, 1.0F));
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				popupToggles[REOPEN_CONTEXT] = false;
				program.file_system.startNewFile();
				program.file_system.trySaveConfigs();
			}
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	if (popupToggles[CONTENT_LACK_WARNING])
	{
		ImGui::OpenPopup("Select a content folder first!", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal);
	}
	ImGui::SetNextWindowPos(centeredPos, ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("Select a content folder first!", &popupToggles[CONTENT_LACK_WARNING], ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal))
	{
		guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

		ImGui::Text("A content folder must be loaded to use this feature");

		if (ImGui::Button("Ok", ImVec2(ImGui::GetWindowSize().x, s.bottomBarHeight)))
		{
			ImGui::CloseCurrentPopup();
			popupToggles[CONTENT_LACK_WARNING] = false;
		}
		ImGui::EndPopup();
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
			ImGuiFileDialog::Instance()->OpenDialog("ChooseContentFolder", "Select the content folder", nullptr, program.file_system.contentDir == "" ? "C:\\" : program.file_system.contentDir, 1);
			// visual settings
			ImGuiFileDialog::Instance()->SetExtentionInfos(".", ImVec4(1, 1, 0.5f, 0.9f));
			break;
		}

		case OPEN:
		{
			promptType = OPEN;
			std::string startDir = "C:\\";
			if (program.file_system.blfFile != "")
			{
				startDir = program.file_system.blfFile;
			}
			else if (program.file_system.blfDir != "")
			{
				startDir = program.file_system.blfDir;
			}
			else
			{
				startDir = program.file_system.contentDir == "" ? startDir : program.file_system.contentDir;
			}
			ImGuiFileDialog::Instance()->SetExtentionInfos(".blf", gd.blfFileColor, "");
			ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Load a BLF file", ".blf", startDir.c_str(), 1);
			break;
		}

		case SAVE:
		{
			if (program.file_system.blfFile != "") // we have a file open, save changes to it
			{
				program.blf_converter.write_file(program.file_system.blfFile.c_str());
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

			ImGuiFileDialog::Instance()->SetExtentionInfos(".blf", gd.blfFileColor, "");
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
					program.file_system.trySaveConfigs();
					// load textures from freshly selected directory
					program.file_system.updateTextures();
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
						program.file_system.blfFile = ImGuiFileDialog::Instance()->GetFilePathName();
						program.file_system.blfDir = std::filesystem::path(ImGuiFileDialog::Instance()->GetFilePathName()).parent_path().string() + "\\";

						program.file_system.changeSetting<std::string>("dirs.blf", program.file_system.blfFile, libconfig::Setting::TypeString);
						program.file_system.changeSetting<std::string>("dirs.blfDir", program.file_system.blfDir, libconfig::Setting::TypeString);
						program.file_system.trySaveConfigs();
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
						program.file_system.blfFile = ImGuiFileDialog::Instance()->GetFilePathName();
						program.file_system.blfDir = std::filesystem::path(ImGuiFileDialog::Instance()->GetFilePathName()).parent_path().string() + "\\";

						program.file_system.changeSetting<std::string>("dirs.blf", program.file_system.blfFile, libconfig::Setting::TypeString);
						program.file_system.changeSetting<std::string>("dirs.blfDir", program.file_system.blfDir, libconfig::Setting::TypeString);
						program.file_system.trySaveConfigs();
						// try to save the file
						program.blf_converter.write_file(ImGuiFileDialog::Instance()->GetFilePathName().c_str());

						// update title
						std::filesystem::path blfPath = std::filesystem::path(ImGuiFileDialog::Instance()->GetFilePathName());
						program.windowManager.setTitle(blfPath.filename().string().c_str());

						// close
						program.file_system.contextOpen = false;
						ImGuiFileDialog::Instance()->Close();
						// close the program if this was our last save
						if (glfwWindowShouldClose(program.windowManager.window))
						{
							program.quitProgram = true;
						}
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
	guiWantKeyboard = false;
	// new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// add guis to be rendered
	addEditorGui();
	addPropertiesGui();
	addTextureSelectorGui();
	addBottomBarGui();
	addPopupGui();

	// check if any window is focused and/or hovered (for input capturing purposes)
	guiHovered = guiIO->WantCaptureMouse;
	guiFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);

	for (auto& [gui, tween] : guiOpacities)
	{
		tween->Step(program.render.deltaTime);
	}

	checkFileDialog();

	// TEMP
	//ImGui::ShowDemoWindow();
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
