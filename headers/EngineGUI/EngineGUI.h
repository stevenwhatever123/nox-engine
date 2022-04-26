#pragma once

// hacky solution for 'byte' is ambiguous
// see https://developercommunity.visualstudio.com/t/error-c2872-byte-ambiguous-symbol/93889
// #define _HAS_STD_BYTE 0

// std headers
#include <map>
#include <string>

// 3rd Party Header
#include <imgui/imgui_internal.h>	// for fixed-layout docking
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// Engine Headers

#include <AudioManager.h>
#include <Renderer.h>
#include <Types.h>



namespace NoxEngineGUI {

	// TODO: This is probably not a good idea in terms of code separation -
	//       probably better if each new panel (.h/.cpp) contains whatever 
	//		 classes they need. This is only used for the master updateGUI() function
	struct GUIParams {
		bool firstLoop = true;

		// Used to detect change in window size
		u32 locWidth;
		u32 locHeight;
		u32 prevWidth;
		u32 prevHeight;
	};

	// Each panel should have an enum associated with it
	enum PanelName {

		// File I/O
		FileExplorer,
		// Rendering

		Scene,

		// Scene object manipulation
		PresetObjects,
		Hierarchy,

		// Animation
		AnimationSettings,

		// Audio
		AudioSource

			// Scripting
	};

	// Assign the panel name to the panel's enum
	// TODO: Make this const?
	static std::map< PanelName, std::string > PANEL_NAME_MAP {
		{ PanelName::FileExplorer,	"File Explorer" },
			{ PanelName::Scene,			"Scene" },
			{ PanelName::PresetObjects, "Preset Objects" },
			{ PanelName::Hierarchy,		"Hierarchy" },
			{ PanelName::AnimationSettings,		"Animation Settings" },
			{ PanelName::AudioSource,	"Audio Source" }
	};

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static bool* p_open = nullptr;


	void init_imgui(GLFWwindow* win);
	void loadTextures();
	void setupFixedLayout();
	void cleanupImGui();

	void updateMenu();
	void updateGUI(GUIParams*);

	// Functions that contain the logic for each sub-window ("tool")
	// should belong in a separate header file.
	// TODO: Remove the following - these are only here to show what 
	//       other panels we could have
	//void updateFileExplorerPanel();
	//void updateHierarchyPanel();
	//void updateConsolePanel();

}

