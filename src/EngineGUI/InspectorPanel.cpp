#include <EngineGUI/InspectorPanel.h>

#include <Managers/IOManager.h>
#include <Managers/EventNames.h>

#include <Core/Entity.h>
#include <Components/IComponent.h>
#include <Components/PositionComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AudioGeometryComponent.h>

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

		// Retrieve all components (could be nullptr)
		PositionComponent*		posComp		= ent->getComp<PositionComponent>();
		RenderableComponent*	rendComp	= ent->getComp<RenderableComponent>();
		AudioGeometryComponent* geoComp		= ent->getComp<AudioGeometryComponent>();

		// Entity name
		if (ImGui::CollapsingHeader(ent->name)) {

			int width = ImGui::GetContentRegionAvail().x;

			// PositionComponent
			if (ent->containsComps<PositionComponent>()) {

				bool enable = ent->isEnabled<PositionComponent>();
				bool expand = ImGui::TreeNode("Position");		// TODO (Vincent): How to change the width of treenode?

				ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
				ImGui::Checkbox("##EnablePos", &enable);

				ImGui::SameLine();
				bool remove = ImGui::SmallButton("-##RemovePos");	//TODO: Use ImageButton?

				ent->setEnabled<PositionComponent>(enable);

				if (expand) {

					IPosition* pos = posComp->CastType<IPosition>();

					// Begin: grey out
					ImGui::BeginDisabled(!enable);

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
			if (ent->containsComps<RenderableComponent>()) {

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

					ImGui::Text("Ambient Texture");
					ImGui::Text("Diffuse Map");
					ImGui::Text("Specular Map");
					ImGui::DragFloat3("Colour", rendComp->color);

					ImGui::TreePop();

					// End: grey out
					ImGui::EndDisabled();
				}

				if (remove) {
					ent->removeComp<RenderableComponent>();
				}
				ImGui::Separator();
			}

			// AudioGeometryComponent
			if (ent->containsComps<AudioGeometryComponent>()) {

				bool geometryGenerated = false;

				bool enable = ent->isEnabled<AudioGeometryComponent>();
				bool expand = ImGui::TreeNode("Audio Geometry");		// TODO (Vincent): How to change the width of treenode?

				ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
				ImGui::Checkbox("##EnableRend", &enable);

				ImGui::SameLine();
				bool remove = ImGui::SmallButton("-##RemoveAudioObs");	// TODO: Use ImageButton?

				ent->setEnabled<AudioGeometryComponent>(enable);

				if (expand) {

					// Begin: grey out
					ImGui::BeginDisabled(!enable);

					IAudioGeometry* igeo = geoComp->CastType<IAudioGeometry>();

					const char* shapeNames[] = {"##geometry_undefined", "Plane", "Box", "Custom"};

					if (ImGui::BeginCombo("Shape", shapeNames[(int)igeo->shape])) {

						for (int i = 0; i < (int)IAudioGeometry::Shape::Count; i++) {

							bool isSelected = ((int)igeo->shape == i);
							if (ImGui::Selectable(shapeNames[i], isSelected)) {
								igeo->shape = static_cast<IAudioGeometry::Shape>(i);
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (isSelected) ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					ImGui::SameLine();

					// Generate / Load buttons. For any choice, 
					// 1. TODO: Remove it from the audio engine and renderer
					// 2. Generate the selected geometry
					// 3. Add to audio engine and renderer (TODO: for plane and box)
					// 4. Set geometryId of the component
					if (igeo->shape == IAudioGeometry::Shape::Plane) {

						ImGui::DragFloat3("v1##audio_plane_v1", &geoComp->v1.x);
						ImGui::DragFloat3("v2##audio_plane_v2", &geoComp->v2.x);
						ImGui::DragFloat3("v3##audio_plane_v3", &geoComp->v3.x);
						ImGui::DragFloat3("v4##audio_plane_v4", &geoComp->v4.x);

						if (ImGui::Button("Generate##gen_audio_plane")) {

							geoComp->generatePlane(geoComp->v1, geoComp->v2, geoComp->v3, geoComp->v4);
							geometryGenerated = true;
						}
					}
					if (igeo->shape == IAudioGeometry::Shape::Box) {

						// Start: No renderable component - grey out the Generate button
						ImGui::BeginDisabled(!ent->containsComps<RenderableComponent>());

						if (ImGui::Button("Generate##gen_audio_box")) {

							geoComp->generateBoundingBox( rendComp->getVertices() );
							geometryGenerated = true;
						}

						// End: No renderable component - grey out the Generate button
						ImGui::EndDisabled();
					}
					if (igeo->shape == IAudioGeometry::Shape::Custom) {

						if (ImGui::Button("Load##load_audio_geometry")) {

							String picked_file = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
							if (picked_file.length() > 0) {
								SIGNAL_EVENT(EventNames::audioGeometryLoaded, picked_file.c_str(), ent);
							}
							geometryGenerated = true;
						}
					}

					// Add to audio engine and renderer
					if (geometryGenerated) SIGNAL_EVENT(EventNames::createAudioGeometry, ent, igeo);

					// Show geometry status
					if (igeo->geometryId == -1) ImGui::Text("Audio Geometry: Undefined");
					else {
						if (igeo->shape == IAudioGeometry::Shape::Plane)	ImGui::Text("Audio Geometry: Plane");
						if (igeo->shape == IAudioGeometry::Shape::Box)		ImGui::Text("Audio Geometry: Auto-generated bounding box");
						if (igeo->shape == IAudioGeometry::Shape::Custom)	ImGui::Text("Audio Geometry: Custom geometry");
					}

					// Mesh loaded: Show tune-able parameters
					if (igeo->geometryId != -1) {
						ImGui::Checkbox("Show outline?", &igeo->render);
						ImGui::Text("Per face direct occlusion");
						ImGui::Text("Per face reverb occlusion");
					}

					ImGui::TreePop();

					// End: grey out
					ImGui::EndDisabled();
				}

				if (remove) {
					ent->removeComp<AudioGeometryComponent>();
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
