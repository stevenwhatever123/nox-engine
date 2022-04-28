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
		if (ImGui::CollapsingHeader(selectedEntity->name)) {

			// TODO: Make this nicer w/ enum?
			IComponent* comp;

			// PositionComponent
			if (selectedEntity->containsComps(PositionFlag)) {

				if (ImGui::TreeNode("Position")) {

					IPosition* pos = selectedEntity->getComp<PositionComponent>()->CastType<IPosition>();

					// TODO: Hook it up with RendObj in the renderer
					// 1. create transform matrix
					// 2. submit it to renderer updateTransformationMatrix(matrix, IRenderable)
					//	  (impl: loop through rendObjs, check pointers match. O(N))

					ImGui::DragFloat3("XYZ", &pos->x, 0.01f);
					ImGui::TreePop();
				}
				ImGui::Separator();
			}

			// RenderableComponent
			if (selectedEntity->containsComps(RenderableFlag)) {

				if (ImGui::TreeNode("Renderable")) {

					IRenderable* rend = selectedEntity->getComp<RenderableComponent>()->CastType<IRenderable>();

					ImGui::Text("Some rendering parameters");
					ImGui::TreePop();
				}
				ImGui::Separator();
			}

			// LightSourceComponent
			// TODO: implement

		}
		ImGui::Separator();

		// Add a button that allows the user to add new components
		if (ImGui::Button("Add Component..."))
			ImGui::OpenPopup("Component List Popup");

		if (ImGui::BeginPopup("Component List Popup")) {

			// Loop through all available components,
			// gray out the ones that this entity has
			for (int i = 1; i < ComponentType::ComponentTypeCount; i++) {

				ComponentType type = static_cast<ComponentType>(i);

				HasCompBitMask mask = (1 << (i - 1));
				bool compExists = selectedEntity->containsComps(mask);

				// Gray out: Begin
				ImGui::BeginDisabled(compExists);

				// Draw
				if (ImGui::Button(kComponentTypeNames[type].c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
					selectedEntity->addComp(type);
				}

				// Gray out: End
				ImGui::EndDisabled();
			}

			ImGui::EndPopup();
		}

	}


	// Window End
	ImGui::End();
}