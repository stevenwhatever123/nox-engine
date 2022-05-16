#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include <EngineGUI/InspectorPanel.h>

#include <Managers/EventNames.h>
#include <Managers/GameManager.h>

#include <Core/AudioTypes.h>
#include <Core/Entity.h>
#include <Components/IComponent.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AnimationComponent.h>
#include <Components/AudioSourceComponent.h>
#include <Components/AudioListenerComponent.h>
#include <Components/AudioGeometryComponent.h>
#include <Components/ScriptComponent.h>

using namespace NoxEngine;


// TODO: move to widgets
void HoverTooltip(const char* text, float pushTextWrapPos = ImGui::GetFontSize() * 35.0f) {

	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(pushTextWrapPos);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


void NoxEngineGUI::updateInspectorPanel(NoxEngine::GameState* state, GUIParams *params) {

	// Variables
	std::string name = kPanelNameMap[PanelName::Inspector];
	ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;

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
		TransformComponent*		transComp		= ent->getComp<TransformComponent>();
		RenderableComponent*	rendComp		= ent->getComp<RenderableComponent>();
		AnimationComponent*		animComp 		= ent->getComp<AnimationComponent>();
		AudioSourceComponent*	audioSrcComp	= ent->getComp<AudioSourceComponent>();
		AudioListenerComponent* audioLisComp	= ent->getComp<AudioListenerComponent>();
		AudioGeometryComponent* geoComp			= ent->getComp<AudioGeometryComponent>();
		ScriptComponent*		scriptComp		= ent->getComp<ScriptComponent>();

		bool animationEditing = animComp ? animComp->editing : false;

		ImGui::PushID("selected_entity");

		// Entity name
		if (ImGui::CollapsingHeader(ent->name)) {

			int width = ImGui::GetContentRegionAvail().x;

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
			// TODO: refactor each component's UI into functions
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

						ImGui::DragFloat3("Position", &transComp->x, 0.1f);
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
					ImGui::Checkbox("##EnableAudioSrc", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveAudioSrc");	//TODO: Use ImageButton?

					ent->setEnabled<AudioSourceComponent>(enable);

					if (expand) {

						// Begin: grey out
						ImGui::BeginDisabled(!enable);

						// Show options to load audio
						if (!audioSrcComp->loaded) {

							ImGui::Text("Load Options");
							ImGui::Checkbox("3D Audio", &audioSrcComp->is3D);
							ImGui::Checkbox("Looping", &audioSrcComp->isLooping);
							ImGui::Checkbox("Stream from disk", &audioSrcComp->isStream);

							if (ImGui::Button("Load##load_audio")) {

								GameManager::Instance()->loadAudioSource(ent, audioSrcComp);

							}
						}

						// Audio loaded, show changable parameters
						else {

							ImGui::Text("File path: %s", audioSrcComp->filePath.c_str());

							bool editDSP = ImGui::TreeNode("DSP Filter");
							ImGui::SameLine();
							bool enableDSP;   ImGui::Checkbox("##enable_DSP", &enableDSP);

							if (editDSP) {

								// Dropdown list of all DSP filter enums
								const char* filterNames[] = { "Echo", "Flange", "Pitch Shift", "Tremolo" };
								u32 nFilterNames = IM_ARRAYSIZE(filterNames);

								ImGui::Text("Filters");		ImGui::SameLine();
								ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.6f);
								ImGui::Combo("##dsp_filters", &audioSrcComp->selectedDspFilter, filterNames, nFilterNames);

								ImGui::SameLine();

								if (ImGui::Button("Add to filter chain")) {
									int dspID = GameManager::Instance()->createDSP(static_cast<DSP_TYPE>(audioSrcComp->selectedDspFilter));
									audioSrcComp->dspChain.emplace_back(dspID);
								}

								// Display DSP chain, allow reorder
								for (int f = 0; f < audioSrcComp->dspChain.size(); f++) {

									DSP *dsp = GameManager::Instance()->getDSP(audioSrcComp->dspChain[f]);

									// DSP object removed but it still exists in a component. This indicates improper cleanup
									if (dsp == nullptr) continue;

									const char *name = kDSPNamesMap[dsp->type];

									// Unique header
									ImGui::PushID(f);
									bool expandDSP = ImGui::CollapsingHeader(name);
									ImGui::PopID();

									// Header is a drag source: define payload
									if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {

										// Set payload to carry the index of our item (could be anything)
										ImGui::SetDragDropPayload("DSP_FILTER_REORDER_DND", &f, sizeof(int));

										params->lastSwappedDSP = f;

										// Display preview
										ImGui::Text("Reordering %s (Filter %d)...", name, f);
										ImGui::EndDragDropSource();
									}

									// Header can also receive drag payload from other headers
									if (ImGui::BeginDragDropTarget()) {

										const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DSP_FILTER_REORDER_DND", ImGuiDragDropFlags_AcceptBeforeDelivery);
										if (payload && f != params->lastSwappedDSP) {
											IM_ASSERT(payload->DataSize == sizeof(int));
											auto srcPair = *(const int*)payload->Data;
											
											// Swap the two filters in the DSP chain
											// TODO-FIX: this relies on the user hovering over all the filters they want to reorder
											//           if they want to change (0 1 2) to (1 2 0) but they skip hovering over 1, they would get (2 1 0)
											std::swap(audioSrcComp->dspChain[f], audioSrcComp->dspChain[params->lastSwappedDSP]);

											// Don't swap with the source back and forth
											params->lastSwappedDSP = f;
											if (payload->IsDelivery()) params->lastSwappedDSP = -1;		// Payload delivered (mouse released), source dropped
										}
										ImGui::EndDragDropTarget();
									}

									ImGui::PushID(f);
									if (expandDSP) {

										int windowLen, maxChannels;

										// Display different parameters based on filter type
										switch (dsp->type) {
										case Echo:
											ImGui::PushID("Echo");
											ImGui::SliderFloat("Delay", 
												&dsp->params[FMOD_DSP_ECHO_DELAY], 
												1.f, 5000.f);
											HoverTooltip("Echo delay. (Unit: ms)");

											ImGui::SliderFloat("Feedback", 
												&dsp->params[FMOD_DSP_ECHO_FEEDBACK], 
												0.f, 100.f);
											HoverTooltip("Echo decay per delay. 100.0 = No decay, 0.0 = total decay. (Unit: Percentage)"); 

											ImGui::SliderFloat("Dry Level", 
												&dsp->params[FMOD_DSP_ECHO_DRYLEVEL], 
												-80.f, 10.f);
											HoverTooltip("Original sound volume. (Unit: Decibels)"); 

											ImGui::SliderFloat("Wet Level", 
												&dsp->params[FMOD_DSP_ECHO_WETLEVEL], 
												-80.f, 10.f);
											HoverTooltip("Volume of echo signal to pass to output. (Unit: Decibels)"); 
											ImGui::PopID();
											break;

										///////////////////////////////////////////////////
										case Flange:
											ImGui::PushID("Flange");
											ImGui::SliderFloat("Mix", 
												&dsp->params[FMOD_DSP_FLANGE_MIX], 
												1.f, 100.f);
											HoverTooltip("Percentage of wet signal in mix. (Unit: Percentage)");

											ImGui::SliderFloat("Depth", 
												&dsp->params[FMOD_DSP_FLANGE_DEPTH], 
												0.01f, 1.f);
											HoverTooltip("Flange depth. (Unit: Linear)");

											ImGui::SliderFloat("Rate", 
												&dsp->params[FMOD_DSP_FLANGE_RATE], 
												0.f, 20.f);
											HoverTooltip("Flange speed. (Unit: Hertz)");
											ImGui::PopID();
											break;

										///////////////////////////////////////////////////
										case PitchShift:
											ImGui::PushID("PitchShift");
											ImGui::SliderFloat("Pitch (octave)",
												&dsp->params[FMOD_DSP_PITCHSHIFT_PITCH],
												0.5f, 2.f);
											HoverTooltip("Pitch value. 0.5 = one octave down, 2.0 = one octave up. 1.0 does not change the pitch.");

											//windowLen = (int)dsp->params[FMOD_DSP_PITCHSHIFT_FFTSIZE];
											//ImGui::SliderInt("FFT Window Length",
											//	&windowLen,
											//	0, 4,
											//	"%d");
											//HoverTooltip("FFT window size. Increase this to reduce 'smearing'. This effect is a warbling sound similar to when an mp3 is encoded at very low bitrates.");
											//dsp->params[FMOD_DSP_PITCHSHIFT_FFTSIZE] = (float)(1 << (windowLen + 8));

											maxChannels = (int)dsp->params[FMOD_DSP_PITCHSHIFT_MAXCHANNELS];
											ImGui::SliderInt("Max Channels",
												&maxChannels,
												0, 16);
											HoverTooltip("Maximum channels supported. 0 = same as FMOD's default output polyphony, 1 = mono, 2 = stereo etc. It is recommended to leave it at 0.");
											dsp->params[FMOD_DSP_PITCHSHIFT_MAXCHANNELS] = (float)maxChannels;
											ImGui::PopID();
											break;

										case Tremolo:
											ImGui::PushID("Tremolo");
											ImGui::SliderFloat("LFO Frequency",
												&dsp->params[FMOD_DSP_TREMOLO_FREQUENCY],
												0.1f, 20.f,
												"%f Hz");

											ImGui::SliderFloat("Depth",
												&dsp->params[FMOD_DSP_TREMOLO_DEPTH],
												0.f, 1.f);

											ImGui::SliderFloat("Shape",
												&dsp->params[FMOD_DSP_TREMOLO_SHAPE],
												0.f, 1.f);
											HoverTooltip("LFO shape morph between triangle (0) and sine (1). (Unit: Linear)");

											ImGui::SliderFloat("Time-skewing",
												&dsp->params[FMOD_DSP_TREMOLO_SKEW],
												-1.f, 1.f);
											HoverTooltip("Time-skewing of LFO cycle.");

											ImGui::SliderFloat("Duty",
												&dsp->params[FMOD_DSP_TREMOLO_DUTY],
												0.5f, 2.f);
											HoverTooltip("LFO on-time.");

											ImGui::SliderFloat("Square",
												&dsp->params[FMOD_DSP_TREMOLO_SQUARE],
												0.5f, 2.f);
											HoverTooltip("Flatness of the LFO shape.");

											ImGui::SliderFloat("Phase",
												&dsp->params[FMOD_DSP_TREMOLO_PHASE],
												0.5f, 2.f);
											HoverTooltip("Instantaneous LFO phase.");

											ImGui::SliderFloat("Spread",
												&dsp->params[FMOD_DSP_TREMOLO_SPREAD],
												0.5f, 2.f);
											HoverTooltip("Rotation / auto-pan effect.");
											ImGui::PopID();
											break;
										}
									}
									ImGui::PopID();

#if 0
									// TODO: Reorder logic
									ImVec2 itemRectMin = ImGui::GetItemRectMin();
									ImVec2 itemRectMax = ImGui::GetItemRectMax();
									ImVec2 itemRectMaxWithSpacing = ImVec2(itemRectMax.x, itemRectMax.y + itemSpacing.y);

									bool isHovering = ImGui::IsMouseHoveringRect(itemRectMin, itemRectMaxWithSpacing);
									bool isActive = ImGui::IsMouseDown(0) && isHovering;

									printf("%d active=%d ; hover=%d\n", f, isActive, isHovering);

									if (isActive && !isHovering) {

										int n_next = audioSrcComp->dspChain.size() + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
										if (n_next >= 0 && n_next < audioSrcComp->dspChain.size()) {

											std::swap(audioSrcComp->dspChain[f], audioSrcComp->dspChain[n_next]);
											//filterNames[f] = filterNames[n_next];
											//filterNames[n_next] = filterName;
											ImGui::ResetMouseDragDelta();
										}
									}
#endif

								}

								ImGui::TreePop();
							}

							ImGui::Spacing();

							ImGui::SliderFloat("Volume##audio_source_volume", &audioSrcComp->volume, 0.0f, 1.0f, "%.1f");
							if (ImGui::Button(audioSrcComp->channelId == -1 ? "Play" : (audioSrcComp->paused ? "Unpause" : "Pause"))) {

								// first play
								if (audioSrcComp->channelId == -1) GameManager::Instance()->playSound(ent, audioSrcComp);

								// pause state
								else if (!audioSrcComp->stopped) audioSrcComp->paused = GameManager::Instance()->pauseUnpauseSound(ent, audioSrcComp);
								
								//else // TODO: sync channelId to -1 when stopped (!am->isPlaying), so sound can be assigned a new channel
							}

							ImGui::SameLine();

							if (ImGui::Button("Stop")) {
								audioSrcComp->stopped = GameManager::Instance()->stopSound(ent, audioSrcComp);
							}

							ImGui::BeginDisabled();
							ImGui::Checkbox("3D", &audioSrcComp->is3D);
							ImGui::Checkbox("Looping", &audioSrcComp->isLooping);
							ImGui::Checkbox("Stream", &audioSrcComp->isStream);
							ImGui::EndDisabled();

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

				// AudioListenerComponent
				if (audioLisComp) {

					bool enable = ent->isEnabled<AudioListenerComponent>();
					bool expand = ImGui::TreeNode("3D Audio Listener");

					ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
					ImGui::Checkbox("##EnableListen", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveListen");	// TODO: Use ImageButton?

					if (expand) {

						// Begin: grey out
						ImGui::BeginDisabled(!enable);

						if (ImGui::RadioButton("Set as active listener", state->activeAudioListener == ent)) {
							state->activeAudioListener = ent;
						}

						ImGui::DragFloat3("Velocity##audio_listener_velocity", &audioLisComp->vVel.x);
						ImGui::SameLine();
						ImGui::TextDisabled("(?)");
						HoverTooltip("Velocity for Doppler effect");
						ImGui::TreePop();

						// End: grey out
						ImGui::EndDisabled();
					}

					if (remove) {
						ent->removeComp<AudioListenerComponent>();
					}
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

						mat4 transformation = animComp ? animComp->getTransformation() : mat4(1.0f);

						if (igeo->shape == IAudioGeometry::Shape::Plane) {

							ImGui::NewLine();

							ImGui::DragFloat3("v1##audio_plane_v1", &geoComp->v1.x);
							ImGui::DragFloat3("v2##audio_plane_v2", &geoComp->v2.x);
							ImGui::DragFloat3("v3##audio_plane_v3", &geoComp->v3.x);
							ImGui::DragFloat3("v4##audio_plane_v4", &geoComp->v4.x);

							if (ImGui::Button("Generate##gen_audio_plane")) {

								geometryGenerated = geoComp->generatePlane( geoComp->v1, geoComp->v2, geoComp->v3, geoComp->v4, transformation );
							}
						}
						if (igeo->shape == IAudioGeometry::Shape::Box) {

							// Start: No renderable component - grey out the Generate button
							ImGui::BeginDisabled(!ent->containsComps<RenderableComponent>());

							if (ImGui::Button("Generate##gen_audio_box")) {

								geometryGenerated = geoComp->generateBoundingBox( rendComp->getVertices(), transformation );
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

					bool enable = ent->isEnabled<ScriptComponent>();
					bool expand = ImGui::TreeNode("Script");

					ImGui::SameLine(width - 2.0f * ImGui::GetFrameHeight());
					ImGui::Checkbox("##EnableScript", &enable);

					ImGui::SameLine();
					bool remove = ImGui::SmallButton("-##RemoveScript");	// TODO: Use ImageButton?

					ent->setEnabled<ScriptComponent>(enable);

					if (expand) {

						ImGui::Text("Script File: ");
						ImGui::SameLine();

						ImGui::PushID("TextInput");

						if (ImGui::GetActiveID() == ImGui::GetID("TextInput")) {
							ImGui::ClearActiveID();
						}

						ImGui::InputText("", (char*)scriptComp->getScript(), 256, ImGuiInputTextFlags_ReadOnly);
						ImGui::PopID();
						ImGui::SameLine();

						if (ImGui::Button("Change")) {
							String file = IOManager::Instance()->PickFile();
							scriptComp->setScript(file.c_str());
						}


						if(remove) {
							ent->removeComp<ScriptComponent>();
						}
						ImGui::TreePop();
					}
				}
			
			}

		}
		ImGui::Separator();

		ImGui::PopID();

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
