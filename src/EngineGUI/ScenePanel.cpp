#include "EngineGUI/ScenePanel.h"

void NoxEngineGUI::updateScenePanel(GameState* params) {

	// Variables
	std::string name = PANEL_NAME_MAP[ PanelName::Scene ];

	// Window Begin
	ImGui::Begin(name.c_str());

	// // Render to texture	
	// if (params->locWidth != params->prevWidth || params->locHeight != params->prevHeight) {
	// 	params->renderer->updateProjection(params->locWidth, params->locHeight);
	// 	params->prevWidth = params->locWidth;
	// 	params->prevHeight = params->locHeight;
	// }

	// // Render background of the app
	params->renderer->fillBackground(1.0f, 0.5f, 0.9f);
	params->renderer->draw();

	// // Use IMGUI to show rendered to framebuffer 

	// // Get the size of the child (i.e. the whole draw size of the windows).
	ImVec2 wsize = ImGui::GetWindowSize();

	// TODO: The code below could be replaced with something like this (from Zenith Engine):
	// if (zenith::Game()->ActiveScene()->PlayState() == ZPlayState::Playing) {
	// 	  ImGui::Image((ImTextureID)zenith::Graphics()->SceneBuffer().id, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
	// }
	// else {
	// 	  ImGui::Image((ImTextureID)zenith::Graphics()->ColorBuffer().id, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
	// }

	// Pass texture rendered to to ImGUI
	ImGui::Image((ImTextureID)params->renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));


	// Window end
	ImGui::End();
}

