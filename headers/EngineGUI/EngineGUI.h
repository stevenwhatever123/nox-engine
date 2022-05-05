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
#include <Camera.h>




namespace NoxEngineGUI {

	struct GUIParams {
		bool firstLoop = true;

		// Used to detect change in window size
		u32 locWidth;
		u32 locHeight;
		u32 prevWidth;
		u32 prevHeight;

		// Variables to communicate between windows
		i32 selectedEntity;		// the array index of the selected entity in the hierarchy window
		
		u32 sceneBackgroundColor;
		NoxEngine::Camera *current_cam;

		// TODO: make is3d a checkbox. if updated, unload and create a new sound
		bool soundIs3d;
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
		Inspector,

		// Animation
		AnimationSettings,

		// Audio
		AudioPanel

			// Scripting
	};

	// Assign the panel name to the panel's enum
	// TODO: Make this const?
	static std::map< PanelName, std::string > PANEL_NAME_MAP {
		{ PanelName::FileExplorer,	"File Explorer" },
		{ PanelName::Scene,			"Scene" },
		{ PanelName::PresetObjects, "Preset Objects" },
		{ PanelName::Hierarchy,		"Hierarchy" },
		{ PanelName::Inspector,		"Inspector" },
		{ PanelName::AnimationSettings,		"Animation Settings" },
		{ PanelName::AudioPanel,	"Audio Source" }
	};

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static bool* p_open = nullptr;


	void init_imgui(GLFWwindow* win);
	void loadTextures();
	void setupFixedLayout();
	void cleanupImGui();

	void updateMenu(GUIParams*);
	void updateGUI(GUIParams*);

	// Functions that contain the logic for each sub-window ("tool")
	// should belong in a separate header file.
	// TODO: Remove the following - these are only here to show what 
	//       other panels we could have
	//void updateConsolePanel();

}

