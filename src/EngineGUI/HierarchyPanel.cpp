#include <EngineGUI/HierarchyPanel.h>

#include <EngineGUI/ImGuiWidgets.h>

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

			// Node with entity name
			// TODO: Visibility checkbox

			// Visibility check box
			ImGui::PushID(i);
			ImGui::Checkbox("", &ent->isVisible);
			ImGui::PopID();

			char uniqueNameBuf[16];
			snprintf(uniqueNameBuf, 16, "%i", i);

			// Draw the widget
			if (SelectableInputResult res = ImGui::SelectableInput(uniqueNameBuf, params->selectedEntity == i,
				ImGuiSelectableFlags_None, ent->name, ENTITY_NAME_MAX_LEN); res.selected) {

				params->selectedEntity = i;
			}

			//if (!strcmp(ent->name, params->modifyingNameBuffer) && res.tempInputStart) {

			//	// On input start, replace the buffer with the entity's name
			//	memset(params->modifyingNameBuffer, 0, 256);
			//	strcpy_s(params->modifyingNameBuffer, 256, ent->name);

			//	printf("Started writing in %i\n", i);
			//}

			// Save: active in previous frame, now not active anymore
			//if (params->pSelectableInputResult.tempInputActive && !res.tempInputActive) {
			//	ent->name = params->modifyingNameBuffer;
			//}

			//if (res.tempInputActive) {
			//	printf("Actively writing in %i. Text: %s\n", i, params->modifyingNameBuffer);
			//}

			// Current result becomes previous result
			//params->pSelectableInputResult = res;
		}
	}

	// TODO: Finally, show a bottom-centered button for adding entities


	// Window End
	ImGui::End();
}