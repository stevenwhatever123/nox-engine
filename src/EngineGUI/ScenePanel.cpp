#include "EngineGUI/ScenePanel.h"

// move to header
#include "Entity.h"

using namespace NoxEngine;


void NoxEngineGUI::updateScenePanel(GameState* state) {

	String name = PANEL_NAME_MAP[ PanelName::Scene ];

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin(name.c_str(), NULL, flags);

	ImGuiID id;
	ImRect bb = ImGui::GetCurrentWindow()->WorkRect;
	ImGui::GetCurrentWindow()->GetID(&id);

	if (ImGui::BeginDragDropTargetCustom(bb, id)) {
	
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PRESET_OBJECT_DND")) {

			IM_ASSERT(payload->DataSize == sizeof(PresetObject));
	
			PresetObject payloadObject = *(const PresetObject*)payload->Data;
			printf("Dropped %s (%i)\n", PRESET_OBJECT_NAMES_MAP[payloadObject].c_str(), payloadObject);
		
			// TODO: Logic for translating 2D cursor position to 3D world coordinates


			// Add an entity to the active scene
			state->activeScene->addEntity(payloadObject);
		}
		ImGui::EndDragDropTarget();
	}

	ImVec2 wsize = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(u64)state->renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

	// Window end
	ImGui::End();
	ImGui::PopStyleVar();
}


