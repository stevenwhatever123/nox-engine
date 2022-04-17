#include "EngineGUI/AnimationPanel.h"

// hacky solution to min and max being redefined in Windows.h, causing std::min/std::max
// to give errors. See https://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of
#include <limits>
#define NOMINMAX

#include <Windows.h>
#include <FBXFileLoader.h>

using namespace NoxEngine;

void NoxEngineGUI::updateAnimationPanel(NoxEngine::GameState *game_state) {

	// Variables
	std::string name = PANEL_NAME_MAP[ PanelName::AnimationSettings ];
	Mesh* mesh = nullptr;
	bool isAlreadyLoaded = false;

	// Window Begin
	ImGui::Begin(name.c_str());


	if (ImGui::Button("Load FBX File")) {

		std::string picked_file = IOManager::Instance()->PickFile("All Files\0*.*\0\0");

		if (picked_file.length() > 0)
		{
			const aiScene* pScene = NoxEngine::readFBX(picked_file.c_str());
			if (pScene != nullptr) {
				mesh = NoxEngine::getMesh(pScene);
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


	// If there was a mesh loaded by user
	// TODO: Might be a good idea to move this to ScenePanel?
	//       Have a queue of loaded meshes then add/update in ScenePanel if it's not empty
	if (isAlreadyLoaded) {
		mesh->prepForRenderer();
		game_state->renderer->addObject(mesh);
		game_state->renderer->updateBuffers();
		delete mesh;
	}


	// Window End
	ImGui::End();
}
