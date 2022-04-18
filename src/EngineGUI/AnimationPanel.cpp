#include "EngineGUI/AnimationPanel.h"

// hacky solution to min and max being redefined in Windows.h, causing std::min/std::max
// to give errors. See https://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of
#include <limits>
#define NOMINMAX

#include <Windows.h>
#include <FBXFileLoader.h>
#include <EventManager.h>

#include <glm/gtx/string_cast.hpp>

using namespace NoxEngine;

void NoxEngineGUI::updateAnimationPanel(NoxEngine::GameState *game_state) {

	// Variables
	std::string name = PANEL_NAME_MAP[ PanelName::AnimationSettings ];
	bool isAlreadyLoaded = false;

	// Window Begin
	ImGui::Begin(name.c_str());


	if (ImGui::Button("Load FBX File")) {

		String picked_file = IOManager::Instance()->PickFile("All Files\0*.*\0\0");

		if (picked_file.length() > 0)
		{
			const aiScene* pScene = NoxEngine::readFBX(picked_file.c_str());
			if (pScene != nullptr) {
				game_state->meshes.emplace(picked_file, pScene);
				
				// game_state->renderer->addObject(mesh);
				// game_state->renderer->updateBuffers();
				EventManager::Instance()->signal("mesh_added", picked_file, game_state->meshes.rbegin()->second);
			}
			else
			{
				printf("Error: converting sence to mesh data");
			}
		}
		else
		{
			printf("Error: reading fbx file");
		}
	}

	auto startItr = game_state->meshes.begin();
	auto endItr = game_state->meshes.end();

	u32 meshID = 0;
	while (startItr != endItr)
	{
		if (startItr->second.getNumOfAnimations() == 0)
		{
			ImGui::Text(startItr->second.nodeHierarchy.name.c_str());
			startItr++;
			continue;
		}

		if (ImGui::TreeNode(startItr->second.nodeHierarchy.name.c_str()))
		{
			ImGui::Text("Number of Animation: %i", startItr->second.getNumOfAnimations());
			ImGui::Text("Current Animation: %i", startItr->second.animationIndex);
			ImGui::SameLine();
			ImGui::PushID(meshID);
			if (ImGui::Button("+"))
			{
				startItr->second.setAnimationIndex(startItr->second.animationIndex + 1);
				startItr->second.resetFrameIndex();
				std::cout << "+" << "\n";
			}
			ImGui::SameLine();
			if (ImGui::Button("-"))
			{
				startItr->second.setAnimationIndex(startItr->second.animationIndex - 1);
				startItr->second.resetFrameIndex();
				std::cout << "-" << "\n";
			}
			ImGui::Text("Name: %s", startItr->second.animations[startItr->second.animationIndex]->mName.C_Str());

			ImGui::Text("Frame index: %i", startItr->second.frameIndex);
			ImGui::Text("Number of ticks: %i", startItr->second.numTicks[startItr->second.animationIndex] - 1);
			
			ImGui::Text("Duration: %.6f",
				startItr->second.animationDuration[startItr->second.animationIndex]);

			ImGui::Text("Progress Bar");
			ImGui::SameLine();
			// Progress has to be between 0 and 1
			f32 progress = 0.0f;
			progress = (float)startItr->second.frameIndex /
				(startItr->second.numTicks[startItr->second.animationIndex] - 1);
			ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

			std::string someString = "Animation " 
				+ std::to_string(startItr->second.animationIndex) + " data: Node 0";

			if (ImGui::TreeNode(someString.c_str()))
			{
				if (ImGui::BeginTable("Table", 2,
					ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
					| ImGuiSelectableFlags_SpanAllColumns))
				{
					for (i32 row = 0; row <
						startItr->second.nodeAnimTransformation[startItr->second.animationIndex][0].size(); row++)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Frame %d", row);
						ImGui::TableNextColumn();

						ImGui::Text(glm::to_string(
							startItr->second.nodeAnimTransformation[startItr->second.animationIndex][0][row]).c_str());
					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();

			ImGui::TreePop();
		}
		meshID++;
		
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
