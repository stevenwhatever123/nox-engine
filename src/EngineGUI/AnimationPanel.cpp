#include "EngineGUI/AnimationPanel.h"

// hacky solution to min and max being redefined in Windows.h, causing std::min/std::max
// to give errors. See https://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of
#include <limits>
#define NOMINMAX

#include <Windows.h>
#include <math.h>
#include <FBXFileLoader.h>
#include <EventManager.h>
#include <EventNames.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <MeshScene.h>

#include <EngineGUI/AnimationSequencer.h>

using namespace NoxEngine;

void NoxEngineGUI::updateAnimationPanel(NoxEngine::GameState* game_state) {

	// Variables
	std::string name = PANEL_NAME_MAP[PanelName::AnimationSettings];
	bool isAlreadyLoaded = false;

	// Window Begin
	ImGui::Begin(name.c_str());


	if (ImGui::Button("Load FBX File")) {
		String picked_file = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
		if (picked_file.length() > 0)
		{
			SIGNAL_EVENT(EventNames::meshAdded, picked_file);
		}
	}

	auto startItr = game_state->meshScenes.begin();
	auto endItr = game_state->meshScenes.end();

	u32 sceneID = 0;
	while (startItr != endItr)
	{
		std::string filename = startItr->first.substr(startItr->first.find_last_of("/\\") + 1);
		size_t lastIndex = filename.find_last_of(".");
		std::string base_filename = filename.substr(0, lastIndex);

		// Directly modify the element in the map
		MeshScene& currentScene = startItr->second;

		ImGui::PushID(sceneID);

		if (!currentScene.hasAnimations())
		{
			ImGui::Text(base_filename.c_str());
			startItr++;
			sceneID++;
			ImGui::PopID();
			continue;
		}

		if (ImGui::TreeNode(base_filename.c_str()))
		{
			ImGui::Text("Number of Animations: %i", currentScene.getNumOfAnimations());
			ImGui::Text("Current Animation: %i", currentScene.animationIndex);
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				currentScene.setAnimationIndex(currentScene.animationIndex + 1);
				currentScene.resetAnimation();
			}
			ImGui::SameLine();
			if (ImGui::Button("-"))
			{
				currentScene.setAnimationIndex(currentScene.animationIndex - 1);
				currentScene.resetAnimation();
			}
			ImGui::Text("Animation name: %s", currentScene.animations[currentScene.animationIndex]->mName.C_Str());
			ImGui::Text("Frame index: %i", currentScene.frameIndex);
			ImGui::Text("Number of ticks: %i", currentScene.numTicks[currentScene.animationIndex] - 1);
			ImGui::Text("Duration: %.6f", currentScene.animationDuration[currentScene.animationIndex]);

			f32 duration = currentScene.animationDuration[currentScene.animationIndex];

			if (ImGui::SliderFloat("Duration", &duration, 0.0f, 50.0f))
				currentScene.resetAnimation();

			currentScene.animationDuration[currentScene.animationIndex] = static_cast<double>(duration);

			ImGui::Text("Progress Bar");
			ImGui::SameLine();

			// Progress has to be between 0 and 1
			f32 progress = 0.0f;
			progress = (float)currentScene.frameIndex / (currentScene.numTicks[currentScene.animationIndex] - 1);
			ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

			if (currentScene.hasAnimations())
			{
				// Let try putting the sequencer here
				if (ImGui::CollapsingHeader("Sequencer"))
				{
					AnimationSequencer mySequence(&currentScene);
					mySequence.mFrameMin = 0;
					mySequence.mFrameMax = currentScene.numTicks[currentScene.animationIndex] - 1;
					//mySequence.myItems.push_back(MySequenceItem{0, 10, 30, false});

					// Let's create the sequencer
					static int selectedEntry = -1;
					static int firstFrame = 0;
					static bool expanded = true;
					static int selectedFrame = 0;

					ImGui::PushItemWidth(130);
					ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
					ImGui::SameLine();
					ImGui::InputInt("Frame ", &selectedFrame);
					ImGui::SameLine();
					ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
					ImGui::PopItemWidth();
					// Safe guard
					if (selectedFrame < 0)
						selectedFrame = 0;
					if (selectedFrame > currentScene.numTicks[currentScene.animationIndex] - 1)
						selectedFrame = currentScene.numTicks[currentScene.animationIndex] - 1;

					Sequencer(&mySequence, &selectedFrame, &expanded, &selectedEntry,
						&firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND
						| ImSequencer::SEQUENCER_CHANGE_FRAME);

					//add a UI to edit that particular item
					if (selectedEntry != -1)
					{
						if (currentScene.allNodes[selectedEntry]->hasAnimations())
						{
							ImGui::Text("Translation");
							glm::mat4 translationMatrix = currentScene.allNodes[selectedEntry]
								->nodeAnimTranslationMatrices[currentScene.animationIndex][selectedFrame];
							float translate[3] = { translationMatrix[0][3], translationMatrix[1][3], translationMatrix [2][3]};
							ImGui::DragFloat3("Translation", translate, 0.005f, -100, 100, "%.3f", 0);
							//Apply value
							(currentScene.allNodes[selectedEntry]
								->nodeAnimTranslationMatrices[currentScene.animationIndex][selectedFrame])[0][3] = translate[0];
							(currentScene.allNodes[selectedEntry]
								->nodeAnimTranslationMatrices[currentScene.animationIndex][selectedFrame])[1][3] = translate[1];
							(currentScene.allNodes[selectedEntry]
								->nodeAnimTranslationMatrices[currentScene.animationIndex][selectedFrame])[2][3] = translate[2];


							ImGui::Text("Rotation");
							float rotation[3] = { currentScene.allNodes[selectedEntry]->eulerAngleXYZ[currentScene.animationIndex][selectedFrame][0],
								currentScene.allNodes[selectedEntry]->eulerAngleXYZ[currentScene.animationIndex][selectedFrame][1] ,
								currentScene.allNodes[selectedEntry]->eulerAngleXYZ[currentScene.animationIndex][selectedFrame][2] };
							ImGui::DragFloat3("Rotation", rotation, 0.005f, -10, 10, "%.3f", 0);
							//Apply value
							currentScene.allNodes[selectedEntry]
								->eulerAngleXYZ[currentScene.animationIndex][selectedFrame][0] = rotation[0];
							currentScene.allNodes[selectedEntry]
								->eulerAngleXYZ[currentScene.animationIndex][selectedFrame][1] = rotation[1];
							currentScene.allNodes[selectedEntry]
								->eulerAngleXYZ[currentScene.animationIndex][selectedFrame][2] = rotation[2];


							ImGui::Text("Scaling");
							glm::mat4 scalingMatrix = currentScene.allNodes[selectedEntry]
								->nodeAnimScalingMatrices[currentScene.animationIndex][selectedFrame];
							float scaling[3] = { scalingMatrix[0][0], scalingMatrix[1][1], scalingMatrix[2][2]};
							ImGui::DragFloat3("Scaling", scaling, 0.005f, 1, 100, "%.3f", 0);
							//Apply value
							(currentScene.allNodes[selectedEntry]
								->nodeAnimScalingMatrices[currentScene.animationIndex][selectedFrame])[0][0] = scaling[0];
							(currentScene.allNodes[selectedEntry]
								->nodeAnimScalingMatrices[currentScene.animationIndex][selectedFrame])[1][1] = scaling[1];
							(currentScene.allNodes[selectedEntry]
								->nodeAnimScalingMatrices[currentScene.animationIndex][selectedFrame])[2][2] = scaling[2];



							currentScene.allNodes[selectedEntry]->updateTransformation();

							glm::mat4 transformation = currentScene.allNodes[selectedEntry]
								->getGlobalTransformation(selectedFrame, currentScene.animationIndex);

							ImGui::Text("Transformation Matrix");
							if (ImGui::BeginTable("Transformations", 4,
								ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
								| ImGuiSelectableFlags_SpanAllColumns))
							{
								ImGui::TableNextRow();
								for (u32 i = 0; i < transformation.length(); i++)
								{
									ImGui::TableNextRow();
									for (u32 j = 0; j < transformation[i].length(); j++)
									{
										ImGui::TableNextColumn();
										ImGui::Text("%.6f", transformation[i][j]);
									}
								}
								ImGui::EndTable();
							}
						}
					}


				}
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
		sceneID++;
		startItr++;
	}


	// If there was a mesh loaded by user
	// TODO: Might be a good idea to move this to ScenePanel?
	//       Have a queue of loaded meshes then add/update in ScenePanel if it's not empty
	// if (isAlreadyLoaded) {
	// 	mesh->prepForRenderer();
	// 	game_state->renderer->addObject(mesh);
	// 	game_state->renderer->updateBuffers();
	// 	delete mesh;
	// }


	// Window End
	ImGui::End();
}
