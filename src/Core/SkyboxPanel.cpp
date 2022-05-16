#include <EngineGUI/SkyboxPanel.h>
#include <EngineGUI/EngineGUI.h>
#include <Core/Renderer.h>
#include <Managers/IOManager.h>

#include <glm/gtc/type_ptr.hpp>

using namespace NoxEngine;
const char* skyboxTextureNames[] = {
	"Right",
	"Left",
	"Up",
	"Down",
	"Front",
	"Back"
};
char tempBuf[256];

void NoxEngineGUI::updateSkyboxPanel(NoxEngine::GameState* game_state) {

	String name = kPanelNameMap[PanelName::SkyboxSettings];
	bool isplaying = false;

	// Window Begin
	ImGui::Begin(name.c_str());

	for (int i = 0; i < game_state->lightSources.size(); i++)
	{
		ImGui::PushID(i);
		const char* text = "Light " + i;
		ImGui::SliderFloat3(text, glm::value_ptr(game_state->lightSources[i]), -100.0f, 100.0f);
		ImGui::PopID();
	}

	for(u32 i = 0; i < 6; i++) {
		ImGui::PushID(skyboxTextureNames[i]);

		ImGui::Text("%s", skyboxTextureNames[i]);
		ImGui::SameLine(70.0f);
		ImGui::InputText("", (char*)game_state->renderer->getSkyboxImagePath(i), 256, ImGuiInputTextFlags_ReadOnly);
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
