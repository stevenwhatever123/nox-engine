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
#include <AudioManager.h>
#include <Types.h>
#include <Renderer.h>
#include <IOManager.h>

namespace NoxEngine {

	struct AudioSource {
		std::string name;
		std::string file;
		glm::vec3 position;
		f32 sourceVolume;
	};

	typedef std::map<std::string, AudioSource> AudioRepo;

	struct GameState {
		AudioRepo audioSources;
	};

	class GameManager {
		public: 
			GameManager(u32 width, u32 height, std::string title);
			void init();
			void update();
			void addAudioSource(AudioSource audioSource);
			inline Renderer* GetRenderer() { return renderer; };
			i8 KeepRunning() { return !should_close; }
			u32 win_height;
			u32 win_width;
			i8 should_close;

		private:

			void init_window();
			void init_audio();
			void init_camera();
			void init_shaders();
			void init_renderer();
			void init_imgui();

			void asset_ui();
			void main_contex_ui();

			void update_gui();
			void update_audio();
			void update_inputs();

			void update_renderer();

			// Window
			GLFWwindow *window;
			std::string title;
			u8 keys[256];

			// Audio
			AudioManager* audioManager;
			Renderer* renderer;
			Camera* camera;
			GameState game_state;
			ImFont* font;
			std::vector<GLProgram> programs;
			f32 light[3];
			GLProgram *current_program;

	};

}
