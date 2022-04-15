/*
 * A class that manages the engine. It is responsible for giving right components to the subsystems of the engine
*/

#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "Scene.h"
#include"UIState.h"
#include"UI.h"

// Systems
#include "Renderer.h"
#include "Camera.h"

class Manager
{
private:

	Renderer * renderer;
	Camera * camera;

	UI* ui;

	Scene* curScene;

public:

	UIState* state;

	Manager(UIState *st, GLFWwindow* win)
	{
		state = st;

		camera = new Camera(glm::vec3(0.0f, 7.0f, 10.0f));
		camera->turnVerBy(20.0f);
		renderer = new Renderer(state->winHeight, state->winWidth, camera);
		renderer->setFrameBufferToTexture();

		ui = new UI(state, win);
	}


	~Manager()
	{
		// Delete all subsystems
		// DON'T delete UIState
		delete renderer;
		delete camera;
		delete ui;
	}




	// TODO: change to a map with names
	void activateScene(Scene *scene) 
	{
		curScene = scene;

		// For all entities in the scene, submit their respective comp to sybsystems
		for (unsigned int i = 0; i < curScene->entities.size(); i++)
		{
			// Add components to respective systems
			addCompToSubSys(i);
			
		}

		// Prep renderers buffers after setting uo objs
		renderer->updateBuffers();

	}












	// Two main working functions of the manager

	// Get Input for all subsystems
	void getInput()
	{
		ui->getInput();

		// move to renderer->getInput(); ?
		// if the window size was changed - > update projection
		if (state->isWinSizeChanged)
		{
			state->isWinSizeChanged = false;
			renderer->updateProjection(state->winWidth, state->winHeight);
		}

		renderer->updateLightPos(state->lightX, state->lightY, state->lightZ);
		// ------------------
	}


	// Update the subsystems
	void update() 
	{

		// ------------ move to renderer->update() ?


		renderer->fillBackground(1.0f, 0.5f, 0.9f);
		renderer->draw();
		// -----------------

		state->textureToRenderTo = renderer->getTexture();

		ui->update();
	}




	private:
		void addCompToSubSys(int ind)
		{
			Entity* gameObj = curScene->entities[ind];

			// Check what comp the entity has (bitmask)
			// Renderer
			if (gameObj->hasComp & 2)
				renderer->addObject(reinterpret_cast<IRenderable*>(gameObj->getComp(2)->CastType(2)), reinterpret_cast<IPosition*>(gameObj->getComp(1)->CastType(2)));
			// Light
			//if (gameObj->hasComp & 4)

			// Audio
			//if (gameObj->hasComp & 8)


		}


};