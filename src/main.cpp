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

#include <Utils.h>
#include <EventManager.h>
#include <MemAllocator.h>

#include "../FBXFileLoader.h"
#include "../ImGuiCustomWindow.h"
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

#include "AudioManager.h"

//#include "Shader.h"
#include "Renderer.h"
#include "RenderableComp.h"

using NoxEngineUtils::Logger;
using NoxEngine::EventManager;
using NoxEngine::PermanentMemAllocator;
using NoxEngine::AudioManager;
using NoxEngine::Renderer;
using NoxEngine::Camera;

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
	void init() {
		Logger::debug("Initing systems");
		init_window();
		init_audio();
		init_camera();
		init_renderer();
		init_imgui();
	}


	GameManager(u32 width, u32 height, std::string title) : win_width(width), win_height(height), title(title) {}


	void update() {

		glClear(GL_COLOR_BUFFER_BIT);

		update_inputs();
		update_gui();
		// update_audio();
		// update_renderer();


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

	void update_gui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::PushFont(font);

		ImGui::Begin("Audio Sources");

		auto startItr = game_state.audioSources.begin();
		auto endItr = game_state.audioSources.end();

		static bool show_demo_window = true;

		if(ImGui::TreeNode("Audios")) {

			while(startItr != endItr) {
				AudioSource *audio_source = &startItr->second;
				if(ImGui::TreeNode(audio_source->name.c_str())) {
					ImGui::Text("File Name: %s", audio_source->file.c_str());
					if(ImGui::TreeNode("Position")) {
						ImGui::Text("Hello World");
						ImGui::SliderFloat3("m", &audio_source->position[0], 0.0f, 10.0f);

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

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::PopFont();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
	}

	void init_camera() {
		camera = new Camera(glm::vec3(0.0f, 10.0f, 200.0f));
	}

	void init_renderer() { 
		// ------------------------------ Set up of the render --------------------------
		// Create Renderer
		// renderer = new Renderer(win_width, win_height, camera);
		// Prep renderers buffers after setting uo objs

	}

	void init_imgui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		font = io.Fonts->AddFontFromFileTTF("envy.ttf", 24);
		io.Fonts->Build();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOther(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void update_audio() {
	}

	void update_inputs() {
		glfwPollEvents();
	}

	void update_renderer() {
		renderer->updateBuffers();
		renderer->setFrameBufferToTexture();
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

};


void init() {
	auto *perm_mem_alloc = PermanentMemAllocator::Instance();
	auto *perm_mem_alloc2 = PermanentMemAllocator::Instance();

	Logger::debug("%p == %p", perm_mem_alloc, perm_mem_alloc2);

	void* mem = perm_mem_alloc->allocate(1024);
	void* mem1 = perm_mem_alloc->allocate(1024);

	auto* allocations = perm_mem_alloc->getAllocations();
	const i64 allocation_count = perm_mem_alloc->getAllocationsCount();

	Logger::debug("allocated: %d", allocation_count);
	for(i32 i = 0; i < allocation_count; i++) {
		Logger::debug("Allocation %d: ptr: %p - size: %lld", i, allocations[i].ptr, allocations[i].size);
	}
}


int main(int argc, char** argv) {
	// Initialize GLFW

	GameManager gm(1280, 720, "Nox Engine");
	gm.init();

	glClearColor(0.2f, 0.6f, 0.5f, 1.0f);

	std::string audioFilePath = "assets/bgm.ogg";

	// Used to detect change in window size
	i32 locWidth = gm.win_width;
	i32 locHeight = gm.win_height;
	i32 prevWidth = locWidth;
	i32 prevHeight = locHeight;

	// Create Renderable Objects
	RenderableComp* obj = new RenderableComp(0.0f, 0.0f, 0.0f, "textures/Solid_white.png");
	RenderableComp* obj2 = new RenderableComp(3.0f, 0.0f, 2.0f, "textures/Red.png");
	RenderableComp* obj3 = new RenderableComp(-3.0f, 0.0f, -2.0f, "textures/beije.png");

	gm.addAudioSource(AudioSource{"TestAudio", "./assets/bgm.ogg", glm::vec3(0, 0, 0), 10});
	gm.addAudioSource(AudioSource{"TestAudio", "./assets/bgm.ogg", glm::vec3(0, 0, 0), 10});
	gm.addAudioSource(AudioSource{"TestAudio", "./assets/bgm.ogg", glm::vec3(0, 0, 0), 10});

	while(gm.KeepRunning()) {

		gm.update();

	}

#if 0
			should_close = true;
		}

		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
			audioManager->PlaySounds(audioFilePath);
		}

		// Audio
		//future<void> fut = async(audioUpdate, audioManager);   // doesn't work?
		audioManager->Update();		// choppy audio sometimes for some reason?

		// GUI
		
		// Sliders to control the 3D sound position
		
		
		// Update systems with UI values
		audioManager->SetChannel3dPosition(0, {soundX, soundY, soundZ});
		audioManager->SetChannelVolume(0, soundVolume);
		//customWindow->showFBXLoaderMenu(mesh);


		ImGui::Begin("Settings");

		if (ImGui::Button("Load FBX File"))
		{
			OPENFILENAME ofn;

			wchar_t szFile[256];

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"Filmbox File (*.fbx)\0*.fbx\0All (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = L"Select FBX file";
			ofn.lpstrDefExt = L"fbx";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			bool readSuccess = GetOpenFileName(&ofn);

			// LPWSTR is different from char
			// Therefore we're converting it
			char filenameBuffer[256];
			wcstombs_s(nullptr, filenameBuffer, sizeof(filenameBuffer), szFile, sizeof(szFile));

			if (readSuccess)
			{
				printf("Read Success\n");

				const aiScene* pScene = FBXFileLoader::readFBX(filenameBuffer);

				// Converting the scene to mesh data
				if (pScene != nullptr)
				{
					mesh = FBXFileLoader::getMesh(pScene);
					isAlreadyLoaded = true;
					//curMesh = mesh;
				}
				else
				{
					printf("Error: converting sence to mesh data");
				}
			}
			else
			{
				printf("Error: reading fbx file");
			}
		}

		ImGui::End();


		// If there was a mesh loaded by user
		if (isAlreadyLoaded){
			// Add mesh to renderer
			mesh->prepForRenderer();
			renderer->addObject(mesh);
			renderer->updateBuffers();
			isAlreadyLoaded = false;

		}


		//	Render to texture	
		if (locWidth != prevWidth || locHeight != prevHeight)
		{
			renderer->updateProjection(locWidth, locHeight);
			prevWidth = locWidth;
			prevHeight = locHeight;
		}

		//Render background of the app
		renderer->fillBackground(1.0f, 0.5f, 0.9f);
		renderer->draw();

		// Use IMGUI to show rendered to framebuffer 
		ImGui::Begin("Demo window"); {

			// Get the size of the child (i.e. the whole draw size of the windows).
			ImVec2 wsize = ImGui::GetWindowSize();
			locWidth = wsize.x;
			locHeight = wsize.y;


			// Pass texture rendered to to ImGUI
			ImGui::Image((ImTextureID)renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		} ImGui::End();

		// One more window just cause
		ImGui::Begin("One more window");
		ImGui::Button("Hello!");

		ImGui::End();

		// Finally show UI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(win);
	}


	// Clean up
	audioManager->Destroy();
	delete obj, obj2, obj3, camera, renderer;

#endif
	return 0;
}
