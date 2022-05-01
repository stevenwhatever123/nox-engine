#pragma once
// std libs
#include <string>
#include <map>
#include <vector>

// 3rd Party Include
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <assimp/Importer.hpp>
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

// Engine Include
#include <Singleton.h>
#include <AudioManager.h>
#include <Mesh.h>
#include <Types.h>
#include <Renderer.h>
#include <IOManager.h>
#include <GameState.h>
#include <Utils.h>
#include <FBXFileLoader.h>
#include <EventManager.h>
#include <EventNames.h>
#include <ScriptsManager.h>

#include <EngineGUI/EngineGUI.h>
#include <EngineGUI/AudioPanel.h>
#include <EngineGUI/AnimationPanel.h>
#include <EngineGUI/ScenePanel.h>
#include <EngineGUI/PresetObjectPanel.h>
#include <EngineGUI/ImGuizmoTool.h>

#include <Scene.h>
#include <Entity.h>

#include <RenderableComponent.h>
#include <PositionComponent.h>


namespace NoxEngine {
	
	using NoxEngineGUI::GUIParams;
	using NoxEngine::GameState;
	using NoxEngine::AudioSource;
	using NoxEngine::Scene;

	class GameManager {
		public: 
			GameManager(u32 width, u32 height, String title);
			void init();
			void update();

			void addAudioSource(AudioSource audioSource);
			void addMesh(String str, Mesh m);
			inline Renderer* GetRenderer() { return renderer; };
			i8 KeepRunning() { return !should_close; }
			u32 win_height;
			u32 win_width;
			i8 should_close;

			time_type currentTime;
			time_type deltaTime;
			time_type lastTime;

		private:

			void init_window();
			void init_events();
			void init_audio();
			void init_camera();
			void init_shaders();
			void init_animation();
			void init_renderer();
			void init_imgui();
			void init_scripts();

			void asset_ui();
			void main_contex_ui();

			void update_gui();
			void update_audio();
			void update_inputs();
			void update_animation();
			void update_renderer();

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
			Scene scene;

			//scripts
			ScriptsManager* scriptsManager;
	public:
			static void exportLua();

	};

}
