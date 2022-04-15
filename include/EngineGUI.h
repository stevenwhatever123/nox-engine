#pragma once

// hacky solution for 'byte' is ambiguous
// see https://developercommunity.visualstudio.com/t/error-c2872-byte-ambiguous-symbol/93889
#define _HAS_STD_BYTE 0

#include <map>
#include <string>

#include <imgui/imgui_internal.h>	// for fixed-layout docking
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Audio classes
#include "AudioManager.h"

// Rendering classes
#include "Renderer.h"


#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 1000


namespace NoxEngine {

	namespace EngineGUI {

		// TODO: This is probably not a good idea in terms of code separation -
		//       probably better if each new panel (.h/.cpp) contains whatever 
		//		 classes they need. This is only used for the master updateGUI() function
		struct GUIParams {

			bool firstLoop = true;

			// Used to detect change in window size
			unsigned int locWidth = WINDOW_WIDTH;
			unsigned int locHeight = WINDOW_HEIGHT;
			unsigned int prevWidth = WINDOW_WIDTH;
			unsigned int prevHeight = WINDOW_HEIGHT;

			// Pointers
			AudioManager* audioManager;
			Renderer* renderer;
		};

		// Each panel should have an enum associated with it
		enum PanelName {

			// File I/O
			FileExplorer,
			
			// Rendering
			Scene,

			// Scene object manipulation
			PresetObjects, Hierarchy,

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
		void cleanupImGui();

		void setupFixedLayout();
		void updateMenu();
		void updateGUI(GUIParams*);

		// Functions that contain the logic for each sub-window ("tool")
		// should belong in a separate header file.
		// TODO: Remove the following - these are only here to show what 
		//       other panels we could have
		//void updateScriptingPanel();
		//void updateFileExplorerPanel();
		//void updateHierarchyPanel();
		//void updatePresetObjectsPanel();

	}

}