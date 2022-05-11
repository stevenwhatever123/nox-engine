#include "EngineGUI/SkyboxPanel.h"
#include "Renderer.h"

using namespace NoxEngine;

const char* skyboxTextureNames[] = {
	"Right",
	"Left",
	"Up",
	"Down",
	"Front",
	"Back"
};

void NoxEngineGUI::updateSkyboxPanel(NoxEngine::GameState* game_state) {

	String name = PANEL_NAME_MAP[PanelName::SkyboxSettings];
	bool isplaying = false;

	// Window Begin
	ImGui::Begin(name.c_str());


	for(u32 i = 0; i < 6; i++) {
		ImGui::PushID(skyboxTextureNames[i]);

		ImGui::Text("Skybox %s %s", skyboxTextureNames[i], game_state->renderer->getSkyboxImagePath(i));
		ImGui::SameLine();
		if(ImGui::Button("Change")) {
			String new_file = IOManager::Instance()->PickFile();

			if(!new_file.empty()) {
				game_state->renderer->setSkyboxImage(new_file.c_str(), i);
			}
		}

		ImGui::PopID();
	}

	ImGui::End();

}
