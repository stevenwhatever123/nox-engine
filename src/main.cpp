#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <future>
#include <vector>

#include <GL/glew.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <Types.h>
#include <Utils.h>
#include <EventManager.h>
#include <MemAllocator.h>
#include <IOManager.h>

#include <FBXFileLoader.h>
#include <ImGuiCustomWindow.h>

#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>
#include <AudioManager.h>

#include <GLProgram.h>
#include <Renderer.h>
#include <RenderableComp.h>
#include <Mesh.h>


using NoxEngineUtils::Logger;
using NoxEngine::EventManager;
using NoxEngine::PermanentMemAllocator;
using NoxEngine::AudioManager;
using NoxEngine::Renderer;
using NoxEngine::Camera;
using NoxEngine::IOManager;
using NoxEngine::GLProgram;
using NoxEngine::ShaderFile;
using NoxEngine::Mesh;

struct AudioSource {
	std::string name;
	std::string file;
	glm::vec3 position;
	f32 sourceVolume;
};

// struct MeshSource {
// 	std::string name;
// 	std::string file;
// 	Mesh m;
// };

typedef std::map<std::string, AudioSource> AudioRepo;
typedef std::map<std::string, Mesh> MeshRepo;

struct GameState {
	AudioRepo audioSources;
	MeshRepo meshes;
};


class GameManager {
	public: 

		GameManager(u32 width, u32 height, std::string title) : win_width(width), win_height(height), title(title) {}

		void init() {
			Logger::debug("Initing systems");
			init_window();
			init_audio();
			init_camera();
			init_shaders();
			init_imgui();
			init_renderer();
		}

		void update() {
			glClear(GL_COLOR_BUFFER_BIT);

			update_inputs();
			// update_audio();
			update_renderer();
			update_gui();

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)) {
				should_close = true;
			}

			glfwSwapBuffers(window);

		}

		void addAudioSource(AudioSource audioSource) {

			// TODO(sharo): handle duplicates better
			game_state.audioSources.emplace(audioSource.name, audioSource);
			audioManager->LoadSound(audioSource.file);

		}

		// void addMesh(Mesh)

		inline Renderer* GetRenderer() { return renderer; };

		i8 KeepRunning() { return !should_close; }

		u32 win_height;
		u32 win_width;
		i8 should_close;

	private:

		void init_window() {

			Logger::debug("Initializing Window");

			if (!glfwInit()) {
				std::cout << "Error initializing glfw...exiting.";
				exit(1);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(win_width, win_height, title.c_str(), nullptr, nullptr);

			glfwMakeContextCurrent(window);

			if (window == nullptr) {
				std::cout << "Failed to create window" << std::endl;
				exit(1);
			}

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				std::cout << " Error Initializing Glew" << std::endl;
				exit(1);
			}

		}

		void init_audio() {
			// Initialize audio system

			audioManager = AudioManager::Instance();

			// TODO: Change to singleton
			audioManager->Init();

			// Set listener. TODO: Move this inside the engine loop
			audioManager->Set3dListenerAttributes(
					{ 0.0f, 0.0f, 0.0f },		// Position
					{ 0.0f, 0.0f, 0.0f },		// velocity (TODO: calculate)
					{ 0.0f, 0.0f, 1.0f },		// Forward
					{ 0.0f, 1.0f, 0.0f }		// Up
					);

		}

		void init_camera() {
			camera = new Camera(glm::vec3(0.0f, 260.0f, 0.0f));
			camera->turnVerBy(90.0f);
		}

		void init_shaders() {
			programs.emplace_back(std::vector<ShaderFile>{
					{ "assets/shaders/vShader.glsl", GL_VERTEX_SHADER, 0 },
					{ "assets/shaders/fShader.glsl", GL_FRAGMENT_SHADER, 0 },
					});

			current_program = &programs.back();
		}

		void init_renderer() { 
			// ------------------------------ Set up of the render --------------------------
			// // Create Renderer
			renderer = new Renderer(win_width, win_height, camera);
			renderer->setProgram(current_program);
			renderer->useProgram();

			const aiScene* pScene = NoxEngine::readFBX("assets/meshes/card.fbx");
			Mesh *mesh = new Mesh(pScene);
			mesh->prepForRenderer();
			renderer->addObject(mesh);
			delete mesh;
		}

		void init_imgui() {


			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();

			font = io.Fonts->AddFontFromFileTTF("envy.ttf", 18);
			io.Fonts->Build();

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOther(window, true);
			ImGui_ImplOpenGL3_Init("#version 450");

		}

		void asset_ui() {

			ImGui::Begin("Audio Sources");

			auto startItr = game_state.audioSources.begin();
			auto endItr = game_state.audioSources.end();

			static bool show_demo_window = true;
			if(ImGui::Button("load")) {
				std::string file_name = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
				Logger::debug("name %s", file_name.c_str());
			}

			if(ImGui::TreeNode("Audios")) {

				while(startItr != endItr) {
					AudioSource *audio_source = &startItr->second;
					if(ImGui::TreeNode(audio_source->name.c_str())) {
						ImGui::Text("File Name: %s", audio_source->file.c_str());
						if(ImGui::TreeNode("Position")) {
							ImGui::DragFloat3("Position", &audio_source->position[0]);
							ImGui::TreePop();
						}

						ImGui::SliderFloat("Volume: ", &audio_source->sourceVolume, -20.0f, 2.0f, "Volume (dB) = %.3f");

						ImGui::TreePop();
					}

					startItr++;
				}

				ImGui::TreePop();
			}

			ImGui::End();

		}

		void main_contex_ui() {

			ImGuiWindowFlags flags =
				ImGuiWindowFlags_NoTitleBar  |
				ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoCollapse;

			ImGui::SetNextWindowPos(ImVec2( 100, 100 ), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2( 100, 100 ), ImGuiCond_FirstUseEver);

			ImGui::Begin("Player", NULL, flags);

			ImVec2 wsize = ImGui::GetWindowSize();
			f32 locWidth = wsize.x;
			f32 locHeight = wsize.y;


			// Pass texture rendered to to ImGUI
			ImGui::Image((ImTextureID)renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
		}

		void update_gui() {

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::PushFont(font);

			asset_ui();
			main_contex_ui();

			ImGui::PopFont();
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}


		void update_audio() { }

		void update_inputs() {
			glfwPollEvents();
		}

		void update_renderer() {
			renderer->updateLightPos(1, 1, 1);
			renderer->updateBuffers();
			renderer->fillBackground(0.1, 0.2, 0.5);
			renderer->draw();
		}

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
		GLProgram *current_program;

};


int main(int argc, char** argv) {

	GameManager gm(1280, 720, "Nox Engine");
	gm.init();


	while(gm.KeepRunning()) {
		gm.update();
	}

	return 0;

}

