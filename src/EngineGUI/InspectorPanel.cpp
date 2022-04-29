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
		Entity* ent = state->activeScene->entities[params->selectedEntity];

		// Entity name
		if (ImGui::CollapsingHeader(ent->name)) {

			int width = ImGui::GetContentRegionAvail().x;

			// PositionComponent
			if (ent->containsComps(PositionFlag)) {

				bool enable = ent->isEnabled<PositionComponent>();
				bool expand = ImGui::TreeNode("Position");		// TODO (Vincent): How to change the width of treenode?

				ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
				ImGui::Checkbox("##EnablePos", &enable);

				ImGui::SameLine();
				bool remove = ImGui::SmallButton("-##RemovePos");	//TODO: Use ImageButton?

				ent->setEnabled<PositionComponent>(enable);

				if (expand) {

					IPosition* pos = ent->getComp<PositionComponent>()->CastType<IPosition>();

					// Begin: grey out
					ImGui::BeginDisabled(!enable);

					// TODO: Hook it up with RendObj in the renderer
					// 1. create transform matrix
					// 2. submit it to renderer updateTransformationMatrix(matrix, IRenderable)
					//	  (impl: loop through rendObjs, check pointers match. O(N))

					ImGui::DragFloat3("XYZ", &pos->x, 0.01f);
					ImGui::TreePop();

					// End: grey out
					ImGui::EndDisabled();
				}

				if (remove) {
					ent->removeComp<PositionComponent>();
				}
				ImGui::Separator();
			}

			// RenderableComponent
			if (ent->containsComps(RenderableFlag)) {

				bool enable = ent->isEnabled<RenderableComponent>();
				bool expand = ImGui::TreeNode("Renderable");		// TODO (Vincent): How to change the width of treenode?

				ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
				ImGui::Checkbox("##EnableRend", &enable);

				ImGui::SameLine();
				bool remove = ImGui::SmallButton("-##RemoveRend");	// TODO: Use ImageButton?

				ent->setEnabled<RenderableComponent>(enable);

				if (expand) {

					// Begin: grey out
					ImGui::BeginDisabled(!enable);

					IRenderable* rend = ent->getComp<RenderableComponent>()->CastType<IRenderable>();

					ImGui::Text("Ambient Texture");
					ImGui::Text("Diffuse Map");
					ImGui::Text("Specular Map");
					ImGui::DragFloat3("Colour", rend->color);

					ImGui::TreePop();

					// End: grey out
					ImGui::EndDisabled();
				}

				if (remove) {
					ent->removeComp<RenderableComponent>();
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
				bool compExists = ent->containsComps(mask);

				// Gray out: Begin
				ImGui::BeginDisabled(compExists);

				// Draw
				if (ImGui::Button(kComponentTypeNames[type].c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
					ent->addComp(type);
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