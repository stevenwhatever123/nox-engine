#include <Core/Entity.h>
#include <EngineGUI/HierarchyPanel.h>
#include <EngineGUI/ImGuiWidgets.h>
#include <Components/IComponent.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Managers/GameManager.h>

using namespace NoxEngine;


void NoxEngineGUI::updateHierarchyPanel(NoxEngine::GameState* state, GUIParams *params) {

	// Variables
	std::string name = kPanelNameMap[PanelName::Hierarchy];
	bool removedEntity = false;

	// Window Begin
	ImGui::Begin(name.c_str());


	/*   Logic   */
	// TODO: Show a PLUS button on the same line as the panel for adding new empty entities

	// No entity has been added yet - show text
	if (state->activeScene->entities.size() == 0) {
		ImGui::Text("No entities.");
	}

	// Loop through the entities in the active scene
	else {

		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		// TODO: revisit example if we want multi-selection

		// Available panel width
		f32 width = ImGui::GetContentRegionAvail().x;

		for (int i = 0; i < state->activeScene->entities.size(); i++) {
			
			Entity* ent = state->activeScene->entities[i];

			// Visibility check box
			bool enable = ent->isEntityEnabled();
			ImGui::PushID(i);
			ImGui::Checkbox("##EntityEnableCheckbox", &enable); ImGui::SameLine();
			ImGui::PopID();
			ent->setEntityEnabled(enable);

			char uniqueNameBuf[16];
			snprintf(uniqueNameBuf, 16, "%i", i);

			// Apply grey out: Begin
			ImGui::BeginDisabled(!enable);

			// Draw the widget
			// FIX (Vincent): Text is slightly shifted up?
			if (SelectableInputResult res = ImGui::SelectableInput(uniqueNameBuf, params->selectedEntity == i,
				ImGuiSelectableFlags_None, ent->name, ENTITY_NAME_MAX_LEN); res.selected) {
			
				params->selectedEntity = i;
			}

			// Apply grey out: End
			ImGui::EndDisabled();

			// Remove button
			ImGui::SameLine(width - ImGui::GetStyle().ItemSpacing.x);
			ImGui::PushID(i);
			bool remove = ImGui::SmallButton("-##RemoveEnt");	// TODO: Use ImageButton?
			ImGui::PopID();

			ent->remove   |= remove;
			removedEntity |= remove;
			
		}
	}


	// Tell subsystems we have removed an entity (probably not necessary)
	if (removedEntity) GameManager::Instance()->scheduleUpdateECS();


	// Window End
	ImGui::End();
}
