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


#include <Components/RenderableComponent.h>
#include <Components/TransformComponent.h>
#include <EngineGUI/EngineGUI.h>
#include <Core/GridObject.h>

// TODO: move to a config file
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Nox Engine"


namespace NoxEngine {

	// forward declares
	class IAudioGeometry;
	class IAudioSource;
	
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
			bool playSound(Entity *ent, IAudioSource *);
			bool stopSound(Entity* ent, IAudioSource *);
			bool loadAudioSource(Entity* ent, IAudioSource *);
			bool loadAudioGeometry(Entity* ent, IAudioGeometry *);

			inline Renderer* GetRenderer() { return renderer; };
			inline bool KeepRunning() { return !should_close; }

			time_type currentTime;
			time_type deltaTime;
			time_type lastTime;
			i32 postprocess_framebuffer;
			u32 post_process_quad_index;
			u32 post_process_vao;


			Camera* camera;
			bool updateNeededECS;
			bool should_close;

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
			void init_scripts();
			void init_postprocess();

			void update_livereloads();
			void update_inputs();
			void update_ecs();
			void update_gui();
			void update_audio();
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
			GameState game_state;
			ImFont* font;
			Array<GLProgram> programs;
			GLProgram *current_program;
			GUIParams ui_params;
	};

}
