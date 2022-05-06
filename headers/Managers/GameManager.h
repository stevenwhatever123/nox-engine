#pragma once

// 3rd Party Include
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <fmod.hpp>
#include <fmod_errors.h>

// Engine Include
#include <Managers/Singleton.h>
#include <Managers/AudioManager.h>
#include <Core/MeshScene.h>
#include <Core/Types.h>
#include <Core/Renderer.h>
#include <Managers/IOManager.h>
#include <Core/GameState.h>
#include <Utils/Utils.h>
#include <Utils/FBXFileLoader.h>
#include <Managers/EventManager.h>
#include <Managers/EventNames.h>

#include <EngineGUI/EngineGUI.h>
#include <EngineGUI/AudioPanel.h>
#include <EngineGUI/AnimationPanel.h>
#include <EngineGUI/ScenePanel.h>
#include <EngineGUI/PresetObjectPanel.h>
#include <EngineGUI/HierarchyPanel.h>
#include <EngineGUI/InspectorPanel.h>
#include <EngineGUI/ImGuizmoTool.h>

#include <Components/RenderableComponent.h>
#include <Components/TransformComponent.h>
#include <Core/GridObject.h>

// TODO: move to a config file
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Nox Engine"


namespace NoxEngine {
	
	// usings
	using NoxEngineGUI::GUIParams;
	using NoxEngine::GameState;
	using NoxEngine::AudioSource;

	class GameManager : public Singleton<GameManager> {

		friend class Singleton<GameManager>;

		public: 
			void init();
			void update();

			void scheduleUpdateECS();

			void addAudioSource(AudioSource audioSource);
			inline Renderer* GetRenderer() { return renderer; };
			i8 KeepRunning() { return !should_close; }
			u32 win_height;
			u32 win_width;
			i8 should_close;
			bool updateNeededECS;

			time_type currentTime;
			time_type deltaTime;
			time_type lastTime;

		protected:
			// TODO (Vincent): initialize other fields as well to be on the safe side
			GameManager();
			~GameManager() {};

		private:

			void init_window();
			void init_events();
			void init_ecs();
			void init_audio();
			void init_camera();
			void init_shaders();
			void init_animation();
			void init_renderer();
			void init_gui();
			void init_scene();

			void asset_ui();
			void main_contex_ui();

			void update_ecs();
			void update_gui();
			void update_audio();
			void update_inputs();
			void update_animation();
			void update_renderer();
			

			void keyboard_callback(GLFWwindow *, i32 key, i32 scan, i32 action, i32 mods);

			// Window
			GLFWwindow *window;
			String title;
			u8 keys[256];

			// Audio
			AudioManager* audioManager;
			Renderer* renderer;
			Camera* camera;
			GameState game_state;
			ImFont* font;
			Array<GLProgram> programs;
			GLProgram *current_program;
			GUIParams ui_params;
	};

}
