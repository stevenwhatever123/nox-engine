#define GLEW_STATIC
#include <Windows.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "EngineGUI.h"

//#include "../ImGuiCustomWindow.h"
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

#include "AudioManager.h"


//#include "Shader.h"
#include "Renderer.h"
#include "RenderableComp.h"


GLFWwindow* initialize_window() {
	if (!glfwInit()) {
		std::cout << "Error initializing glfw...exiting.";
		exit(1);
	}
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Nox Engine", nullptr, nullptr);

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


Renderer* init_renderer(Camera* camera) {
	// ------------------------------ Set up of the render --------------------------

	// Create Renderer
	Renderer* renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, camera);



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

	glewInit();
	
	std::string audioFilePath = "assets/bgm.ogg";
	AudioManager* audioManager = init_audio(audioFilePath);

	// new event system in imgui
	NoxEngine::EngineGUI::init_imgui(win);
	

	// Create Renderable Objects
	RenderableComp* obj = new RenderableComp(0.0f, 0.0f, 0.0f, "textures/Solid_white.png");
	RenderableComp* obj2 = new RenderableComp(3.0f, 0.0f, 2.0f, "textures/Red.png");
	RenderableComp* obj3 = new RenderableComp(-3.0f, 0.0f, -2.0f, "textures/beije.png");
	
	// Create Camera
	Camera* camera = new Camera(glm::vec3(0.0f, 10.0f, 200.0f));

	Renderer* renderer = init_renderer(camera);

	// Wrap everything in a struct for the GUI (not the best solution?)
	NoxEngine::EngineGUI::GUIParams aGUIParams{
		.audioManager = audioManager,
		.renderer = renderer
	};


	// Main loop
	while (!should_close) {
		
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) {
			should_close = true;
		}

		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
			audioManager->PlaySounds(audioFilePath);
		}

		// Audio
		audioManager->Update();

		// GUI
		NoxEngine::EngineGUI::updateGUI(&aGUIParams);


		// Next slide please
		glfwSwapBuffers(win);
	}


	// Clean up
	NoxEngine::EngineGUI::cleanupImGui();

	glfwDestroyWindow(win);
	glfwTerminate();

	audioManager->Destroy();
	delete obj, obj2, obj3, camera, renderer;

	return 0;
}
