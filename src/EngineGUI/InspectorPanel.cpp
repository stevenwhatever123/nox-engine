#include <EngineGUI/InspectorPanel.h>

#include <Managers/EventNames.h>
#include <Managers/GameManager.h>

#include <Core/Entity.h>
#include <Components/IComponent.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AnimationComponent.h>
#include <Components/AudioSourceComponent.h>
#include <Components/AudioGeometryComponent.h>
#include <Components/ScriptComponent.h>
#include <Components/CameraComponent.h>

using namespace NoxEngine;


void NoxEngineGUI::updateInspectorPanel(NoxEngine::GameState* state, GUIParams *params) {

	// Variables
	std::string name = kPanelNameMap[PanelName::Inspector];

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
		TransformComponent*		transComp			= ent->getComp<TransformComponent>();
		RenderableComponent*	rendComp		= ent->getComp<RenderableComponent>();
		AnimationComponent*		animComp 		= ent->getComp<AnimationComponent>();
		AudioSourceComponent*	audioSrcComp	= ent->getComp<AudioSourceComponent>();
		AudioGeometryComponent* geoComp			= ent->getComp<AudioGeometryComponent>();
		ScriptComponent*		scriptComp		= ent->getComp<ScriptComponent>();
		CameraComponent*		cameraComp		= ent->getComp<CameraComponent>();

		bool animationEditing = animComp ? animComp->editing : false;

		// Entity name
		if (ImGui::CollapsingHeader(ent->name)) {

			int width = (i32)ImGui::GetContentRegionAvail().x;

			// Use inspector as editor (?)
			if (animationEditing) {

				// Transform
				if (ImGui::TreeNode("Animation Transform")) {

					//add a UI to edit that particular item
					glm::mat4 translationMatrix = animComp->translationMatrices[animComp->animationIndex][animComp->frameIndex];
					float translate[3] = { translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2] };
					ImGui::DragFloat3("Translation", translate, 0.005f, -100, 100, "%.3f", 0);
					glm::vec3 translateVec(translate[0], translate[1], translate[2]);

					// Apply value
					// Turns out glm does translation differently and it follows the glsl format
					glm::mat4 translateMatrix = glm::translate(glm::mat4(1), translateVec);
					animComp->translationMatrices[animComp->animationIndex][animComp->frameIndex] = translateMatrix;

					float rotation[3] = { 
						animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][0],
						animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][1] ,
						animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][2] 
					};
					ImGui::DragFloat3("Rotation", rotation, 0.005f, -10, 10, "%.3f", 0);

					// Apply value
					animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][0] = rotation[0];
					animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][1] = rotation[1];
					animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][2] = rotation[2];


					glm::mat4 scalingMatrix = animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex];
					float scaling[3] = { scalingMatrix[0][0], scalingMatrix[1][1], scalingMatrix[2][2] };
					ImGui::DragFloat3("Scaling", scaling, 0.005f, 1, 100, "%.3f", 0);

					// Apply value
					(animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex])[0][0] = scaling[0];
					(animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex])[1][1] = scaling[1];
					(animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex])[2][2] = scaling[2];

					animComp->updateTransformation();

					ImGui::TreePop();
				}

				ImGui::Separator();

				if (ImGui::TreeNode("Animation Controls")) {
					ImGui::Text("Number of Animations: %i", animComp->getNumOfAnimations());
					ImGui::Text("Current Animation: %i", animComp->animationIndex);
					ImGui::SameLine();

					if (ImGui::Button("+##Anim")) {
						animComp->setAnimationIndex(animComp->animationIndex + 1);
						animComp->resetAnimation();
					}

					ImGui::SameLine();

					if (ImGui::Button("-##Anim")) {
						animComp->setAnimationIndex(animComp->animationIndex - 1);
						animComp->resetAnimation();
					}

					ImGui::Text("Animation name: %s", animComp->animationName[animComp->animationIndex].c_str());
					ImGui::Text("Frame index: %i", animComp->frameIndex);
					ImGui::Text("Total number of ticks: %i", animComp->numTicks[animComp->animationIndex] - 1);
					ImGui::Text("Play up to tick: %i", animComp->maximumFrame[animComp->animationIndex] - 1);
					ImGui::Text("Duration: %.6f", animComp->animationDuration[animComp->animationIndex]);

					f32 duration = (f32)animComp->animationDuration[animComp->animationIndex];

					if (ImGui::SliderFloat("Duration", &duration, 0.0f, 50.0f)) animComp->resetAnimation();

					if (!animComp->playAnimation) {
						if (ImGui::Button("Play")) animComp->playAnimation = true;
					} else {
						if (ImGui::Button("Stop")) animComp->playAnimation = false;
					}

					animComp->animationDuration[animComp->animationIndex] = static_cast<double>(duration);

					ImGui::Text("Progress Bar");
					ImGui::SameLine();

					// Progress has to be between 0 and 1
					f32 progress = 0.0f;
					progress = (float)animComp->frameIndex / (animComp->numTicks[animComp->animationIndex] - 1);
					ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

					if (ImGui::Button("Stop Edit")) animComp->editing = false;
					
					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			// Display all components
			else {

				// TransformComponent
				if (transComp) {

					bool enable = ent->isEnabled<TransformComponent>();
					bool expand = ImGui::TreeNode("Transform");		// TODO (Vincent): How to change the width of treenode?

					ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
					ImGui::Checkbox("##EnableTrans", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveTrans");	//TODO: Use ImageButton?

					ent->setEnabled<TransformComponent>(enable);

					if (expand) {
						
						// Begin: grey out
						ImGui::BeginDisabled(!enable);

						// TODO: Hook it up with RendObj in the renderer
						// 1. create transform matrix
						// 2. submit it to renderer updateTransformationMatrix(matrix, IRenderable)
						//	  (impl: loop through rendObjs, check pointers match. O(N))

						ImGui::DragFloat3("Position", &transComp->x, 0.01f);
						ImGui::DragFloat3("Rotation", &transComp->rx, 0.01f);
						ImGui::DragFloat3("Scale", &transComp->sx, 0.01f);

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
				if (rendComp) {

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

				// AnimationComponent
				if (animComp) {

					bool enable = ent->isEnabled<AnimationComponent>();
					bool expand = ImGui::TreeNode("Animation");

					ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
					ImGui::Checkbox("##EnableAnim", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveAnim");

					ent->setEnabled<AnimationComponent>(enable);

					if (expand) {

						// Begin: grey out
						ImGui::BeginDisabled(!enable);

						ImGui::Text("Number of Animations: %i", animComp->getNumOfAnimations());
						ImGui::Text("Current Animation: %i", animComp->animationIndex);
						ImGui::SameLine();

						if (ImGui::Button("+##Anim")) {
							animComp->setAnimationIndex(animComp->animationIndex + 1);
							animComp->resetAnimation();
						}

						ImGui::SameLine();

						if (ImGui::Button("-##Anim")) {
							animComp->setAnimationIndex(animComp->animationIndex - 1);
							animComp->resetAnimation();
						}

						if (!animComp->playAnimation) { 
						    if (ImGui::Button("Play")) animComp->playAnimation = true;
						} else {
							if (ImGui::Button("Stop")) animComp->playAnimation = false;
						}

						ImGui::Text("Progress Bar");
						ImGui::SameLine();

						// Progress has to be between 0 and 1
						f32 progress = 0.0f;
						progress = (float)animComp->frameIndex / (animComp->numTicks[animComp->animationIndex] - 1);
						ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

						if (!animationEditing) { 
							if (ImGui::Button("Edit")) animComp->editing = true;
						}
						ImGui::TreePop();

						// End: grey out
						ImGui::EndDisabled();
					}

					if (remove) {
						ent->removeComp<AnimationComponent>();
					}
					ImGui::Separator();
				}

				// AudioSourceComponent
				if (audioSrcComp) {

					bool enable = ent->isEnabled<AudioSourceComponent>();
					bool expand = ImGui::TreeNode("Audio Source");		// TODO (Vincent): How to change the width of treenode?

					ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
					ImGui::Checkbox("##EnablePos", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemovePos");	//TODO: Use ImageButton?

					ent->setEnabled<AudioSourceComponent>(enable);

					if (expand) {

						// Begin: grey out
						ImGui::BeginDisabled(!enable);

						// Show options to load audio
						if (!audioSrcComp->loaded) {
							ImGui::Checkbox("Load as 3D Audio?", &audioSrcComp->is3D);

							if (ImGui::Button("Load##load_audio")) {

								GameManager::Instance()->loadAudioSource(ent, audioSrcComp);

							}
						}

						// Audio loaded, show changable parameters
						else {

							ImGui::Text("File path: %s", audioSrcComp->filePath.c_str());

							if (ImGui::TreeNode("DSP Filter")) {

								// Dropdown list of all DSP filter enums
								//const char* filterNames[] = { "Pitch Shift", "Echo" };
								//ImGui::BeginCombo()


								if (ImGui::Button("Create and apply##audio_dsp_create_apply")) {

								}

								ImGui::TreePop();
							}

							ImGui::SameLine();
							bool enableDSP;   ImGui::Checkbox("##enable_DSP", &enableDSP);

							ImGui::SliderFloat("Volume", &audioSrcComp->volume, 0.0f, 1.0f, "%.1f");
							if (ImGui::Button("Play")) {

								GameManager::Instance()->playSound(ent, audioSrcComp);
							}

							ImGui::BeginDisabled();
							ImGui::Checkbox("3D", &audioSrcComp->is3D);
							ImGui::Checkbox("Looping", &audioSrcComp->isLooping);
							ImGui::Checkbox("Stream", &audioSrcComp->isStream);
							ImGui::EndDisabled();

							ImGui::SameLine();

							if (ImGui::Button("Stop")) {

								GameManager::Instance()->stopSound(ent, audioSrcComp);
							}

						}

						ImGui::TreePop();

						// End: grey out
						ImGui::EndDisabled();
					}

					if (remove) {
						ent->removeComp<AudioSourceComponent>();
					}
					ImGui::Separator();
				}

				// AudioGeometryComponent
				if (geoComp) {

					bool geometryGenerated = false;

					bool enable = ent->isEnabled<AudioGeometryComponent>();
					bool expand = ImGui::TreeNode("Audio Geometry");		// TODO (Vincent): How to change the width of treenode?

					ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
					ImGui::Checkbox("##EnableGeometry", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveAudioObs");	// TODO: Use ImageButton?

					ent->setEnabled<AudioGeometryComponent>(enable);

					if (expand) {

						// Begin: grey out
						ImGui::BeginDisabled(!enable);

						IAudioGeometry* igeo = geoComp->CastType<IAudioGeometry>();

						const char* shapeNames[] = {"##geometry_undefined", "Plane", "Bounding Box", "Custom"};

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

							ImGui::NewLine();

							ImGui::DragFloat3("v1##audio_plane_v1", &geoComp->v1.x);
							ImGui::DragFloat3("v2##audio_plane_v2", &geoComp->v2.x);
							ImGui::DragFloat3("v3##audio_plane_v3", &geoComp->v3.x);
							ImGui::DragFloat3("v4##audio_plane_v4", &geoComp->v4.x);

							if (ImGui::Button("Generate##gen_audio_plane")) {

								geometryGenerated = geoComp->generatePlane(geoComp->v1, geoComp->v2, geoComp->v3, geoComp->v4);
							}
						}
						if (igeo->shape == IAudioGeometry::Shape::Box) {

							// Start: No renderable component - grey out the Generate button
							ImGui::BeginDisabled(!ent->containsComps<RenderableComponent>());

							if (ImGui::Button("Generate##gen_audio_box")) {

								// TODO: fix
								//geometryGenerated = geoComp->generateBoundingBox( rendComp->getVertices() );
							}

							// End: No renderable component - grey out the Generate button
							ImGui::EndDisabled();
						}
						if (igeo->shape == IAudioGeometry::Shape::Custom) {

							if (ImGui::Button("Load##load_audio_geometry")) {

								geometryGenerated = GameManager::Instance()->loadAudioGeometry(ent, geoComp);
							}
						}

						// Add to audio engine and renderer
						if (geometryGenerated) {
							GameManager::Instance()->createAudioGeometry(ent, igeo);
						}

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

				// ScriptComponent
				if (scriptComp) {

					if (ImGui::TreeNode("Script")) {

						bool enable = ent->isEnabled<AudioGeometryComponent>();
					
						ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
						ImGui::Checkbox("##EnableScript", &enable);

						ImGui::SameLine();
						bool remove = ImGui::SmallButton("-##RemoveScript");	// TODO: Use ImageButton?

						ImGui::Text("Script File: ");
						ImGui::SameLine();

						ImGui::PushID("TextInput");

						if(ImGui::GetActiveID() == ImGui::GetID("TextInput")) {
							ImGui::ClearActiveID();
						}

						ImGui::InputText("", (char*)scriptComp->getScript(), 256, ImGuiInputTextFlags_ReadOnly);
						ImGui::PopID();
						ImGui::SameLine();

						if(ImGui::Button("Change")) {
							String file = IOManager::Instance()->PickFile();
							scriptComp->setScript(file.c_str());
						}

						if(remove) {
							ent->removeComp<ScriptComponent>();
						}
						ImGui::TreePop();
					}
				}

				if (cameraComp) {

					bool tree_open = ImGui::TreeNode("Camera");
					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveCamera");

					if (tree_open) {

						cameraComp->getCamera()->GetCameraYawPitchRoll();
						auto vals = cameraComp->getCamera()->GetCameraYawPitchRoll();
						ImGui::DragFloat("Yaw", &vals[0]);
						ImGui::DragFloat("Pitch", &vals[1]);
						auto& m = cameraComp->getCamera()->GetCameraYawPitchRoll();
						cameraComp->getCamera()->setYaw(vals.x);
						cameraComp->getCamera()->setPitch(vals.y);

						ImGui::DragFloat("X", &cameraComp->getCamera()->GetCameraPosition()[0]);
						ImGui::DragFloat("Y", &cameraComp->getCamera()->GetCameraPosition()[1]);
						ImGui::DragFloat("Z", &cameraComp->getCamera()->GetCameraPosition()[2]);

						if(state->renderer->getCamera() == cameraComp->getCamera()) {
							if(ImGui::Button("View Main Camera")) {
								state->renderer->setCamera(state->cameras[0]);
							}
						} else {
							if(ImGui::Button("View Camera")) {
								state->renderer->setCamera(cameraComp->getCamera());
							}
						}

						if(remove) {
							ent->removeComp<CameraComponent>();
						}

						ImGui::TreePop();
					}
				}

			
			}

		}
		ImGui::Separator();

		// Add a button that allows the user to add new components
		if (ImGui::Button("Add Component..."))
			ImGui::OpenPopup("Component List Popup");

		if (ImGui::BeginPopup("Component List Popup")) {
			ImGui::Text("                          ");

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
