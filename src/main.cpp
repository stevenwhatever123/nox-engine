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

#include "FBXFileLoader.h"
#include "ImGuiCustomWindow.h"
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

#include "AudioManager.h"
#include"Entity.h"

#include"UIState.h"
#include "Manager.h"


//#include "Shader.h"
//#include "Renderer.h"
#include "RenderableComp.h"
#include "PositionComp.h"


unsigned int winWidth = 1900 , winHeight = 1000;

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


int main(int argc, char** argv) {

	// Initialize GLFW
	GLFWwindow* win = initialize_window();

	// Used to detect change in window size
	int locWidth = winWidth, locHeight = winHeight, prevWidth = winWidth, prevHeight = winHeight;



	// Create UIState - model in control-model-view
	UIState state = UIState();
	state.winHeight = winHeight;
	state.winWidth = winWidth;
	state.isRunning = true;

	// Create Manager - core of the engine
	Manager manager = Manager(&state, win);
	

	// Create a game obj
	Entity gameobj = Entity();

	// Create a mesh for it
	RenderableComp* geom = new RenderableComp(0.0f, 0.0f, 0.0f, "assets/meshes/textures/Terracotta_Tiles_002_Base_Color.jpg");

	// Create its position
	PositionComp* pos = new PositionComp(0.0, 2.0, 0.0);

	// Add components to the entity
	gameobj.addComp(geom);
	gameobj.addComp(pos);


	// Add obj to the scene
	Scene* curScene = new Scene();
	curScene->addEntity(&gameobj);

	// Add scene to manager
	manager.activateScene(curScene);

	
	// Game loop
	while (state.isRunning) {

		manager.getInput();
		manager.update();
	}


	return 0;
}
