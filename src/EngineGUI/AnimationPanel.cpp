#include <math.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Utils/FBXFileLoader.h>
#include <Core/MeshScene.h>
#include <Managers/EventManager.h>
#include <Managers/EventNames.h>
#include <Managers/IOManager.h>
#include <EngineGUI/AnimationSequencer.h>
#include <EngineGUI/AnimationPanel.h>


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
			SIGNAL_EVENT(EventNames::meshAdded, picked_file.c_str());
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

			f32 duration = (f32)currentScene.animationDuration[currentScene.animationIndex];

			if (ImGui::SliderFloat("Duration", &duration, 0.0f, 50.0f))
				currentScene.resetAnimation();

			if (!currentScene.playAnimation)
			{
				if (ImGui::Button("Play"))
				{
					currentScene.playAnimation = !currentScene.playAnimation;
				}
			}
			else
			{
				if (ImGui::Button("Stop"))
				{
					currentScene.playAnimation = !currentScene.playAnimation;
				}
			}

			currentScene.animationDuration[currentScene.animationIndex] = static_cast<double>(duration);

			ImGui::Text("Progress Bar");
			ImGui::SameLine();

			// Progress has to be between 0 and 1
			f32 progress = 0.0f;
			progress = (float)currentScene.frameIndex / (currentScene.numTicks[currentScene.animationIndex] - 1);
			ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

			updateSequencer(currentScene);

			ImGui::TreePop();
		}

		ImGui::PopID();
		sceneID++;
		startItr++;
	}
	// Window End
	ImGui::End();
}

void NoxEngineGUI::updateSequencer(NoxEngine::MeshScene& currentScene)
{
	if (currentScene.hasAnimations())
	{
		// Let try putting the sequencer here
		if (ImGui::CollapsingHeader("Sequencer"))
		{
			AnimationSequencer mySequence(&currentScene);
			mySequence.mFrameMin = 0;
			mySequence.mFrameMax = currentScene.numTicks[currentScene.animationIndex] - 1;

			// Let's create the sequencer
			static int selectedEntry = -1;
			static int firstFrame = 0;
			static bool expanded = true;
			i32 selectedFrame = currentScene.frameIndex;
			bool insertFrame = false;

			ImGui::PushItemWidth(130);
			ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
			ImGui::SameLine();
			ImGui::InputInt("Frame ", &selectedFrame);
			ImGui::SameLine();
			ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
			currentScene.updateNumTicks(currentScene.animationIndex, mySequence.mFrameMax + 1);
			ImGui::SameLine();

			if (ImGui::Button("+##InsertFrame"))
			{
				insertFrame = true;
			}

			ImGui::SameLine();
			ImGui::Text("Insert Frame");
			ImGui::PopItemWidth();

			// Safe guard
			if (selectedFrame < 0)
				selectedFrame = 0;
			if (selectedFrame > currentScene.numTicks[currentScene.animationIndex] - 1)
				selectedFrame = currentScene.numTicks[currentScene.animationIndex] - 1;

			if (selectedEntry < 0)
				selectedEntry = -1;
			if (selectedEntry > currentScene.allNodes.size() - 1)
				selectedEntry = currentScene.allNodes.size() - 1;

			Sequencer(&mySequence, &selectedFrame, &expanded, &selectedEntry,
				&firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND
				| ImSequencer::SEQUENCER_CHANGE_FRAME);

			currentScene.frameIndex = selectedFrame;

			//add a UI to edit that particular item
			if (selectedEntry != -1)
			{
				if (currentScene.allNodes[selectedEntry]->hasAnimations())
				{
					ImGui::Text("Translation");
					glm::mat4 translationMatrix = currentScene.allNodes[selectedEntry]
						->nodeAnimTranslationMatrices[currentScene.animationIndex][selectedFrame];
					float translate[3] = { translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2] };
					ImGui::DragFloat3("Translation", translate, 0.005f, -100, 100, "%.3f", 0);
					glm::vec3 translateVec(translate[0], translate[1], translate[2]);

					//Apply value
					// Turns out glm does translation differently and it follows the glsl format
					glm::mat4 translateMatrix = glm::translate(glm::mat4(1), translateVec);
					currentScene.allNodes[selectedEntry]->nodeAnimTranslationMatrices[currentScene.animationIndex][selectedFrame] = translateMatrix;


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
					float scaling[3] = { scalingMatrix[0][0], scalingMatrix[1][1], scalingMatrix[2][2] };
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
						for (i32 i = 0; i < transformation.length(); i++)
						{
							ImGui::TableNextRow();
							for (i32 j = 0; j < transformation[i].length(); j++)
							{
								ImGui::TableNextColumn();
								ImGui::Text("%.6f", transformation[i][j]);
							}
						}
						ImGui::EndTable();
					}
				}

				if (insertFrame)
				{
					currentScene.insertFrame(currentScene.animationIndex, selectedFrame);
					//currentScene.allNodes[selectedEntry]->insertFrameAfter(currentScene.animationIndex, selectedEntry);
				}
			}

			insertFrame = false;
		}
	}
}
