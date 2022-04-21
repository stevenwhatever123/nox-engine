#include <EngineGUI/InspectorPanel.h>

#include <Entity.h>
#include <IComponent.h>
#include <PositionComponent.h>
#include <RenderableComponent.h>

using namespace NoxEngine;


void NoxEngineGUI::updateInspectorPanel(NoxEngine::GameState* state, GUIParams *params) {

	// Variables
	std::string name = PANEL_NAME_MAP[PanelName::Inspector];

	// Window Begin
	ImGui::Begin(name.c_str());


	/*   Logic   */

	// No entity is selected - show text
	if (params->selectedEntity < 0 || params->selectedEntity+1 > state->activeScene->entities.size()) {
		ImGui::Text("No entity selected.");
	}

	// Otherwise, loop through the components of the selected entity
	else {
		Entity* selectedEntity = state->activeScene->entities[params->selectedEntity];

		// Entity name
		if (ImGui::CollapsingHeader(selectedEntity->name.c_str())) {

			// TODO: Make this nicer w/ enum?
			IComponent* comp;

			// PositionComponent
			if (comp = selectedEntity->getComp(1); nullptr != comp) {

				if (ImGui::TreeNode("Position")) {

					IPosition* pos = reinterpret_cast<IPosition*>(comp->CastType(2));

					ImGui::DragFloat3("XYZ", &pos->x, 0.01f);
					ImGui::TreePop();
				}
			}
			ImGui::Separator();

			// RenderableComponent
			if (comp = selectedEntity->getComp(2); nullptr != comp) {

				if (ImGui::TreeNode("Renderable")) {
					ImGui::Text("Some rendering parameters");
					ImGui::TreePop();
				}

			}
		}


	}

	// Finally, show a bottom-centered button for adding entities


	// Window End
	ImGui::End();
}