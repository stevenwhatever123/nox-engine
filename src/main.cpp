#define GLEW_STATIC
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

#include "../FBXFileLoader.h"
#include "../ImGuiCustomWindow.h"
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

#include "AudioManager.h"


//#include "Shader.h"
#include "Renderer.h"
#include "RenderableComp.h"


unsigned int winWidth = 1300 , winHeight = 1000;

GLFWwindow* initialize_window() {
	if (!glfwInit()) {
		std::cout << "Error initializing glfw...exiting.";
		exit(1);
	}
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* win = glfwCreateWindow(winWidth, winHeight, "Nox Engine", nullptr, nullptr);

	glfwMakeContextCurrent(win);
	
	if (win == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		exit(1);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << " Error Initializing Glew" << std::endl;
		exit(1);
	}

	return win;
}


AudioManager* init_audio(std::string audioFilePath) {
	// Initialize audio system
	// TODO: Change to singleton
	AudioManager* audioManager = new AudioManager();
	audioManager->Init();

	// Set listener. TODO: Move this inside the engine loop
	audioManager->Set3dListenerAttributes(
			{ 0.0f, 0.0f, 0.0f },		// Position
			{ 0.0f, 0.0f, 0.0f },		// velocity (TODO: calculate)
			{ 0.0f, 0.0f, 1.0f },		// Forward
			{ 0.0f, 1.0f, 0.0f }		// Up
			);

	// Load an audio file
	
	audioManager->LoadSound(audioFilePath, true);

	return audioManager;
}


void init_imgui(GLFWwindow *win) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOther(win, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

Renderer* init_renderer(Camera* camera) {
	// ------------------------------ Set up of the render --------------------------
	
	
	

	// Create Renderer
	Renderer* renderer = new Renderer(winWidth, winHeight, camera);



	// Add objects to Renderer
	//renderer->addObject(obj);
	//renderer->addObject(obj2);
	//renderer->addObject(obj3);

	// Prep renderers buffers after setting uo objs
	//renderer->updateBuffers();
	renderer->setFrameBufferToTexture();

	return renderer;
}

int main(int argc, char** argv) {
	
	


	// Initialize GLFW
	GLFWwindow* win = initialize_window();
	bool should_close = false;
	glClearColor(0.2f, 0.6f, 0.5f, 1.0f);

	bool open_demo = true;
	glewInit();
	
	std::string audioFilePath = "assets/bgm.ogg";
	AudioManager* audioManager = init_audio(audioFilePath);

	init_imgui(win);
	


	ImGuiCustomWindow* customWindow = new ImGuiCustomWindow();

	Mesh* mesh = NULL;
	bool isAlreadyLoaded = false;

	// Used to detect change in window size
	int locWidth = winWidth, locHeight = winHeight, prevWidth = winWidth, prevHeight = winHeight;

	// Create Renderable Objects
	RenderableComp* obj = new RenderableComp(0.0f, 0.0f, 0.0f, "textures/Solid_white.png");
	RenderableComp* obj2 = new RenderableComp(3.0f, 0.0f, 2.0f, "textures/Red.png");
	RenderableComp* obj3 = new RenderableComp(-3.0f, 0.0f, -2.0f, "textures/beije.png");
	
	// Create Camera
	Camera* camera = new Camera(glm::vec3(0.0f, 10.0f, 200.0f));

	Renderer* renderer = init_renderer(camera);

	while (!should_close) {
		
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) {
			should_close = true;
		}

		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
			audioManager->PlaySounds(audioFilePath);
		}

		// Audio
		//future<void> fut = async(audioUpdate, audioManager);   // doesn't work?
		audioManager->Update();		// choppy audio sometimes for some reason?

		// GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Sliders to control the 3D sound position
		static float soundX = 0.0f, soundY = 0.0f, soundZ = 0.0f;
		static float soundVolume = 0.0f;
		ImGui::SliderFloat("Sound X", &soundX, -1.0f, 1.0f, "X = %.3f");
		ImGui::SliderFloat("Sound Y", &soundY, -1.0f, 1.0f, "Y = %.3f");
		ImGui::SliderFloat("Sound Z", &soundZ, -1.0f, 1.0f, "Z = %.3f");
		ImGui::SliderFloat("Sound Volume", &soundVolume, -20.0f, 2.0f, "Volume (dB) = %.3f");

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

	return 0;
}
