#include <EngineGUI/InspectorPanel.h>

#include <Entity.h>
#include <IComponent.h>
#include <TransformComponent.h>
#include <RenderableComponent.h>
#include <AnimationComponent.h>

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
			if (ent->containsComps(TransformFlag)) {

				bool enable = ent->isEnabled<TransformComponent>();
				bool expand = ImGui::TreeNode("Transform");		// TODO (Vincent): How to change the width of treenode?

				ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
				ImGui::Checkbox("##EnablePos", &enable);

				ImGui::SameLine();
				bool remove = ImGui::SmallButton("-##RemovePos");	//TODO: Use ImageButton?

				ent->setEnabled<TransformComponent>(enable);

				if (expand) {

					ITransform* transform = ent->getComp<TransformComponent>()->CastType<ITransform>();

					// Begin: grey out
					ImGui::BeginDisabled(!enable);

					// TODO: Hook it up with RendObj in the renderer
					// 1. create transform matrix
					// 2. submit it to renderer updateTransformationMatrix(matrix, IRenderable)
					//	  (impl: loop through rendObjs, check pointers match. O(N))

					ImGui::DragFloat3("Position", &transform->x, 0.01f);

					ImGui::DragFloat3("Rotation", &transform->rx, 0.01f);

					ImGui::DragFloat3("Scale", &transform->sx, 0.01f);

					ImGui::TreePop();

					// End: grey out
					ImGui::EndDisabled();
				}

				if (remove) {
					ent->removeComp<TransformComponent>();
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
					RenderableComponent* rendComp = ent->getComp<RenderableComponent>();

					ImGui::Text("Ambient Texture");
					// Select Ambient Texture
					ImGui::SameLine();
					if (ImGui::Button("Select"))
					{
						String picked_file = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
						rendComp->ambientTexture = picked_file;
						state->renderer->changeTexture(ent);
					}

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

			// Animation Component
			if (ent->containsComps(AnimationFlag))
			{
				bool enable = ent->isEnabled<AnimationComponent>();
				bool expand = ImGui::TreeNode("Animation");

				ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
				ImGui::Checkbox("##EnableAnim", &enable);

				ImGui::SameLine();
				bool remove = ImGui::SmallButton("-##RemoveAnim");

				ent->setEnabled<AnimationComponent>(enable);

				if (expand)
				{
					// Begin: grey out
					ImGui::BeginDisabled(!enable);

					AnimationComponent* animComp = ent->getComp<AnimationComponent>();

					ImGui::Text("Number of Animations: %i", animComp->getNumOfAnimations());
					ImGui::Text("Current Animation: %i", animComp->animationIndex);
					ImGui::SameLine();

					if (ImGui::Button("+##Anim"))
					{
						animComp->setAnimationIndex(animComp->animationIndex + 1);
						animComp->resetAnimation();
					}
					ImGui::SameLine();
					if (ImGui::Button("-##Anim"))
					{
						animComp->setAnimationIndex(animComp->animationIndex - 1);
						animComp->resetAnimation();
					}

					ImGui::Text("Animation name: %s", animComp->animationName[animComp->animationIndex].c_str());
					ImGui::Text("Frame index: %i", animComp->frameIndex);
					ImGui::Text("Number of ticks: %i", animComp->numTicks[animComp->animationIndex] - 1);
					ImGui::Text("Duration: %.6f", animComp->animationDuration[animComp->animationIndex]);

					f32 duration = (f32)animComp->animationDuration[animComp->animationIndex];

					if (ImGui::SliderFloat("Duration", &duration, 0.0f, 50.0f))
						animComp->resetAnimation();

					if (!animComp->playAnimation)
					{
						if (ImGui::Button("Play"))
						{
							animComp->playAnimation = !animComp->playAnimation;
						}
					}
					else
					{
						if (ImGui::Button("Stop"))
						{
							animComp->playAnimation = !animComp->playAnimation;
						}
					}

					animComp->animationDuration[animComp->animationIndex] = static_cast<double>(duration);

					ImGui::Text("Progress Bar");
					ImGui::SameLine();

					// Progress has to be between 0 and 1
					f32 progress = 0.0f;
					progress = (float)animComp->frameIndex / (animComp->numTicks[animComp->animationIndex] - 1);
					ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

					ImGui::TreePop();

					// End: grey out
					ImGui::EndDisabled();
				}

				if (remove) {
					ent->removeComp<AnimationComponent>();
				}
				ImGui::Separator();
			}

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