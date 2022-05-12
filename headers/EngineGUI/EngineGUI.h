#pragma once

// 3rd Party Header
#include <imgui/imgui_internal.h>	// for fixed-layout docking
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// Engine Headers
#include <Managers/AudioManager.h>
#include <Core/Renderer.h>
#include <Core/Types.h>
#include <Core/Camera.h>
#include <Core/GameState.h>

#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Managers/SaveLoadManager.h>

namespace NoxEngineGUI {

	struct GUIParams {
		bool firstLoop = true;

		u32 locWidth;
		u32 locHeight;
		u32 prevWidth;
		u32 prevHeight;

		int imguizmoMode = 7; // Translation by default

		// the array index of the selected entity in the hierarchy window
		i32 selectedEntity;		
		
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
		AnimationSequencerPanel,

		// Audio
		AudioPanel,

		// Skybox
		SkyboxSettings
	};

	// Assign the panel name to the panel's enum
	// TODO: Make this const?
	static Map< PanelName, String > kPanelNameMap {
		{ PanelName::FileExplorer,      "File Explorer" },
		{ PanelName::Scene,             "Scene" },
		{ PanelName::PresetObjects,     "Preset Objects" },
		{ PanelName::Hierarchy,         "Hierarchy" },
		{ PanelName::Inspector,         "Inspector" },
		{ PanelName::AnimationSequencerPanel, "Animation Sequencer" },
		{ PanelName::AudioPanel,        "Audio Editor" },
		{ PanelName::SkyboxSettings, "Skybox Settings" }
	};

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static bool* p_open = nullptr;

	void init_imgui(GLFWwindow* win);
	void loadTextures();
	void setupFixedLayout();
	void cleanupImGui();

	void updateMenu(NoxEngine::GameState&, GUIParams*);
	void updateGUI(NoxEngine::GameState&, GUIParams*);

	// Functions that contain the logic for each sub-window ("tool")
	// should belong in a separate header file.
	// TODO: Remove the following - these are only here to show what 
	//       other panels we could have
	//void updateConsolePanel();

}

