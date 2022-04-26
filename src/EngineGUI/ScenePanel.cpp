#include "EngineGUI/ScenePanel.h"


void NoxEngineGUI::updateScenePanel(GameState* params) {

	// Variables
	std::string name = PANEL_NAME_MAP[ PanelName::Scene ];

	// Window Begin
	ImGui::Begin(name.c_str());

	ImRect bb = ImGui::GetCurrentWindow()->WorkRect;
	ImGuiID id; ImGui::GetCurrentWindow()->GetID(&id);

	// Receive drag/drop payload from preset objects panel
	if (ImGui::BeginDragDropTargetCustom(bb, id)) {
	
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PRESET_OBJECT_DND")) {
			IM_ASSERT(payload->DataSize == sizeof(PresetObject));
	
			PresetObject payloadObject = *(const PresetObject*)payload->Data;
			printf("Dropped %s (%i)\n", PRESET_OBJECT_NAMES_MAP[payloadObject].c_str(), payloadObject);
		

			// TODO: Logic for translating 2D cursor position to 3D world coordinates

		}
		ImGui::EndDragDropTarget();
	}

	// // Render to texture	
	// if (params->locWidth != params->prevWidth || params->locHeight != params->prevHeight) {
	// 	params->renderer->updateProjection(params->locWidth, params->locHeight);
	// 	params->prevWidth = params->locWidth;
	// 	params->prevHeight = params->locHeight;
	// }

	// Render background of the app
	//params->renderer->fillBackground(1.0f, 0.5f, 0.9f);
	//params->renderer->drawSkyBox();
	//params->renderer->draw();

	// Use IMGUI to show rendered to framebuffer 

	// Get the size of the child (i.e. the whole draw size of the windows).
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

