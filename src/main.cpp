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
#include "Entity.h"

#include "UIState.h"
#include "Manager.h"


#include "RenderableComp.h"
#include "PositionComp.h"


unsigned int winWidth = 1900;
unsigned int winHeight = 1000;

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

	void activateScene(Scene *scene) 
	{
		curScene = scene;

		// For all entities in the scene, submit their respective comp to sybsystems
		for (u32 i = 0; i < curScene->entities.size(); i++)
		{
			// Add components to respective systems
			addCompToSubSys(i);
		}

		// Prep renderers buffers after setting uo objs
		// renderer->updateBuffers();
	}

	void addCompToSubSys(int ind)
	{
		Entity* gameObj = curScene->entities[ind];

		// Check what comp the entity has (bitmask)
		// Renderer
		if (gameObj->hasComp & 2)
			renderer->addObject(
					reinterpret_cast<IRenderable*>(gameObj->getComp(2)->CastType(2)),
					reinterpret_cast<IPosition*>(gameObj->getComp(1)->CastType(2))
					);
		// Light
		//if (gameObj->hasComp & 4)

		// Audio
		//if (gameObj->hasComp & 8)

	}


	// Game loop
	while (state.isRunning) {
		manager.getInput();
		// manager.update();
	}


	return 0;
}
