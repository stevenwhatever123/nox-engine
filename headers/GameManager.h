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
#include <Singleton.h>
#include <AudioManager.h>
#include <Mesh.h>
#include <MeshScene.h>
#include <Types.h>
#include <Renderer.h>
#include <IOManager.h>
#include <GameState.h>
#include <Utils.h>
#include <FBXFileLoader.h>
#include <EventManager.h>
#include <EventNames.h>

#include <EngineGUI/EngineGUI.h>
#include <EngineGUI/AudioPanel.h>
#include <EngineGUI/AnimationPanel.h>
#include <EngineGUI/ScenePanel.h>
#include <EngineGUI/PresetObjectPanel.h>
#include <EngineGUI/HierarchyPanel.h>
#include <EngineGUI/InspectorPanel.h>
#include <EngineGUI/ImGuizmoTool.h>

#include <RenderableComponent.h>
#include <PositionComponent.h>
#include <GridObject.h>

// TODO: move to a config file
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Nox Engine"


namespace NoxEngine {

	// forward declares
	class IAudioGeometry;
	
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
			void createAudioGeometry(Entity* ent, IAudioGeometry* igeo);

			void addAudioSource(AudioSource audioSource);
			//void addMesh(String str, Mesh m);
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
