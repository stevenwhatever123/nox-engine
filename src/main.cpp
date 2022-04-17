#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

// System/std includes
#include <Windows.h>

// Engine Include
#include <GameManager.h>
using NoxEngine::GameManager;

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

	bool playAnimation = false;
	glm::vec3 targetPosition(0, 0, -10);

	GameManager gm(1280, 720, "Nox Engine");
	gm.init();

	while(gm.KeepRunning()) {

		// Update delta Time
		// currentFrame = glfwGetTime();
		// deltaTime = currentFrame - lastFrame;
		// lastFrame = currentFrame;

		//std::cout << "Current Frame: " << currentFrame << "\n";
		//std::cout << "Last Frame: " << lastFrame << "\n";
		//std::cout << "Delta Time: " << deltaTime << "\n";

		gm.update();

		//// If there was a mesh loaded by user
		//if (isAlreadyLoaded){
		//	// Add mesh to renderer
		//	renderer->clearObject();
		//	mesh->verticesPreped.clear();

		//	mesh->prepForRenderer();
		//	renderer->addObject(mesh);
		//	renderer->updateBuffers();
		//	//isAlreadyLoaded = false;

		//	mesh->calculateCenterPosition();

		//	if (playAnimation)
		//	{
		//		mesh->update(deltaTime);
		//	}
		//}

	}

	// if (ImGui::Button("Flip UV"))
	// {
	// 	if (isAlreadyLoaded)
	// 	{
	// 		mesh->flipUV();
	// 	}
	// }

	// if (!playAnimation)
	// {
	// 	if (ImGui::Button("Play Animation"))
	// 	{
	// 		playAnimation = !playAnimation;
	// 	}
	// }
	// else
	// {
	// 	if (ImGui::Button("Stop Animation"))
	// 	{
	// 		playAnimation = !playAnimation;
	// 	}
	// }

	// if (isAlreadyLoaded)
	// {
	// 	if (mesh->getNumOfAnimations() > 0)
	// 	{
	// 		std::string stringDisplay = "Animation Duration: " +
	// 			std::to_string((time_type)mesh->animationDuration[mesh->animationIndex]);
	// 		const char* c_stringDisplay = stringDisplay.c_str();
	// 		ImGui::Text(c_stringDisplay);

	// 		std::string stringDisplay2 = "Number of animation: " +
	// 			std::to_string(mesh->getNumOfAnimations());
	// 		const char* c_stringDisplay2 = stringDisplay2.c_str();
	// 		ImGui::Text(c_stringDisplay2);

	// 		if (ImGui::Button("Increase animation index"))
	// 		{
	// 			mesh->animationIndex =
	// 				mesh->animationIndex >= (mesh->animations.size() / 2) - 1 ?
	// 				mesh->animationIndex : mesh->animationIndex + 1;
	// 		}
	// 		if (ImGui::Button("Decrease animation index"))
	// 		{
	// 			mesh->animationIndex =
	// 				mesh->animationIndex <= 0 ?
	// 				0 : mesh->animationIndex - 1;
	// 		}

	// 		std::string stringDisplay3 = "Current animation index: " +
	// 			std::to_string(mesh->animationIndex);
	// 		const char* c_stringDisplay3 = stringDisplay3.c_str();
	// 		ImGui::Text(c_stringDisplay3);

	// 		float sliderDuration = 0.0f;
	// 		sliderDuration = (float)mesh->animationDuration[mesh->animationIndex];

	// 		ImGui::SliderFloat("Custome Duration",
	// 				&sliderDuration, 1, 50, "%.3f");

	// 		mesh->animationDuration[mesh->animationIndex] =
	// 			(time_type)sliderDuration;
	// 	}
	// 	else
	// 	{
	// 		if (ImGui::Button("Generate animation"))
	// 		{

	// 		}
	// 	}

	// 	if (ImGui::Button("Refresh"))
	// 	{
	// 		mesh->resetFrameIndex();
	// 	}
	// }
	return 0;
}

