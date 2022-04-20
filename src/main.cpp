#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

// System/std includes
#include <Windows.h>

// Engine Include
#include <GameManager.h>
#include "script.h"
using NoxEngine::GameManager;

int main(int argc, char** argv) {

#if 0
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

	bool playAnimation = false;
	glm::vec3 targetPosition(0, 0, -10);

#endif
	GameManager gm(1280, 720, "Nox Engine");
	gm.init();

	while(gm.KeepRunning()) {


		gm.update();

	}
	//lua();
	return 0;
}

