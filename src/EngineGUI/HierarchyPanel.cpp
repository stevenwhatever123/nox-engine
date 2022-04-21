#include <EngineGUI/HierarchyPanel.h>

#include <Entity.h>
#include <IComponent.h>
#include <PositionComponent.h>
#include <RenderableComponent.h>

using namespace NoxEngine;


void NoxEngineGUI::updateHierarchyPanel(NoxEngine::GameState* state, GUIParams *params) {

	// Variables
	std::string name = PANEL_NAME_MAP[PanelName::Hierarchy];

	// Window Begin
	ImGui::Begin(name.c_str());


	/*   Logic   */

	// No entity has been added yet - show text
	if (state->activeScene->entities.size() == 0) {
		ImGui::Text("No entities.");
	}

	// Loop through the entities in the active scene
	else {

		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		// TODO: revisit example if we want multi-selection

		for (int i = 0; i < state->activeScene->entities.size(); i++) {
			
			Entity* ent = state->activeScene->entities[i];
			ImGuiTreeNodeFlags nodeFlags = baseFlags;
			bool nodeOpen = false;

			// Check if this was previously selected
			if (params->selectedEntity == i) {
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
			}

			// Node with entity name
			// TODO: Visibility checkbox
			nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, ent->name.c_str());
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				params->selectedEntity = i;
			
			if (nodeOpen) {

				// TODO: Children entities?

				ImGui::TreePop();
			}
		}
	}

	// TODO: Finally, show a bottom-centered button for adding entities


	// Window End
	ImGui::End();
}