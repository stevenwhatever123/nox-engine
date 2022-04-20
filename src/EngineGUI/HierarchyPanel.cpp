#include <EngineGUI/HierarchyPanel.h>

#include <Entity.h>
#include <IComponent.h>
#include <PositionComponent.h>
#include <RenderableComponent.h>

using namespace NoxEngine;


void NoxEngineGUI::updateHierarchyPanel(NoxEngine::GameState* state) {

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

		for (auto entItr = state->activeScene->entities.begin(); entItr != state->activeScene->entities.end(); entItr++) {

			// Entity name
			if (ImGui::TreeNode((*entItr)->name.c_str())) {

				// TODO: Children entities?
				
				// TODO: Make this nicer w/ enum?
				IComponent* comp;

				// PositionComponent
				if (comp = (*entItr)->getComp(1); nullptr != comp) {

					if (ImGui::TreeNode("Position")) {

						IPosition* pos = reinterpret_cast<IPosition*>(comp->CastType(2));

						ImGui::DragFloat3("XYZ", &pos->x, 0.001f, -10.0f, 10.0f, "%.3f");
						ImGui::TreePop();
					}
				}
				ImGui::Separator();

				// RenderableComponent
				if (comp = (*entItr)->getComp(2); nullptr != comp) {

					if (ImGui::TreeNode("Renderable")) {
						ImGui::Text("Some rendering parameters");
						ImGui::TreePop();
					}

				}

				ImGui::TreePop();
			}


		}
	}

	// Finally, show a bottom-centered button for adding entities


	// Window End
	ImGui::End();
}