#include "EngineGUI/AnimationPanel.h"

#include <math.h>

#include <Utils/FBXFileLoader.h>
#include <Managers/EventManager.h>
#include <Managers/EventNames.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Core/MeshScene.h>


using namespace NoxEngine;

void NoxEngineGUI::initAnimationPanel()
{
}

void NoxEngineGUI::updateAnimationPanel(NoxEngine::GameState* game_state, GUIParams* params) {

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

	if (params->selectedEntity < 0 || params->selectedEntity + 1 > game_state->activeScene->entities.size()) {
		ImGui::Text("No entity selected.");
	}
	else {
		Entity* ent = game_state->activeScene->entities[params->selectedEntity];
		
		if (ent->containsComps(AnimationFlag))
		{
			AnimationComponent* animComp = ent->getComp<AnimationComponent>();

			if(animComp->editing)
				updateSequencer(game_state, animComp);
		}
	}
	// Window End
	ImGui::End();
}

void NoxEngineGUI::updateSequencer(NoxEngine::GameState* game_state, AnimationComponent* animComp)
{
	// Let try putting the sequencer here
	AnimationSequencer mySequence(animComp, game_state);
	mySequence.mFrameMin = 0;
	mySequence.mFrameMax = animComp->maximumFrame[animComp->animationIndex] - 1;

	// Let's create the sequencer
	static int selectedEntry = 0;
	static int firstFrame = 0;
	static bool expanded = true;
	i32 selectedFrame = animComp->frameIndex;
	bool insertFrame = false;

	ImGui::PushItemWidth(130);
	ImGui::InputInt("Frame ", &selectedFrame);
	ImGui::SameLine();
	ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
	animComp->maximumFrame[animComp->animationIndex] = mySequence.mFrameMax + 1;
	animComp->updateNumTicks(animComp->animationIndex, mySequence.mFrameMax + 1);
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
	if (selectedFrame > animComp->maximumFrame[animComp->animationIndex] - 1)
		selectedFrame = animComp->maximumFrame[animComp->animationIndex] - 1;

	Sequencer(&mySequence, &selectedFrame, &expanded, &selectedEntry,
		&firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND
		| ImSequencer::SEQUENCER_CHANGE_FRAME
		| ImSequencer::SEQUENCER_ADD);

	animComp->frameIndex = selectedFrame;

	////add a UI to edit that particular item
	//ImGui::Text("Translation");
	//glm::mat4 translationMatrix = animComp->translationMatrices[animComp->animationIndex][selectedFrame];
	//float translate[3] = { translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2] };
	//ImGui::DragFloat3("Translation", translate, 0.005f, -100, 100, "%.3f", 0);
	//glm::vec3 translateVec(translate[0], translate[1], translate[2]);

	////Apply value
	//// Turns out glm does translation differently and it follows the glsl format
	//glm::mat4 translateMatrix = glm::translate(glm::mat4(1), translateVec);
	//animComp->translationMatrices[animComp->animationIndex][selectedFrame] = translateMatrix;

	//ImGui::Text("Rotation");
	//float rotation[3] = { animComp->eulerAngleXYZ[animComp->animationIndex][selectedFrame][0],
	//	animComp->eulerAngleXYZ[animComp->animationIndex][selectedFrame][1] ,
	//	animComp->eulerAngleXYZ[animComp->animationIndex][selectedFrame][2] };
	//ImGui::DragFloat3("Rotation", rotation, 0.005f, -10, 10, "%.3f", 0);
	////Apply value
	//animComp->eulerAngleXYZ[animComp->animationIndex][selectedFrame][0] = rotation[0];
	//animComp->eulerAngleXYZ[animComp->animationIndex][selectedFrame][1] = rotation[1];
	//animComp->eulerAngleXYZ[animComp->animationIndex][selectedFrame][2] = rotation[2];


	//ImGui::Text("Scaling");
	//glm::mat4 scalingMatrix = animComp->scalingMatrices[animComp->animationIndex][selectedFrame];
	//float scaling[3] = { scalingMatrix[0][0], scalingMatrix[1][1], scalingMatrix[2][2] };
	//ImGui::DragFloat3("Scaling", scaling, 0.005f, 1, 100, "%.3f", 0);
	////Apply value
	//(animComp->scalingMatrices[animComp->animationIndex][selectedFrame])[0][0] = scaling[0];
	//(animComp->scalingMatrices[animComp->animationIndex][selectedFrame])[1][1] = scaling[1];
	//(animComp->scalingMatrices[animComp->animationIndex][selectedFrame])[2][2] = scaling[2];



	//animComp->updateTransformation();

	//glm::mat4 transformation = animComp->getTransformation();

	//ImGui::Text("Transformation Matrix");
	//if (ImGui::BeginTable("Transformations", 4,
	//	ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
	//	| ImGuiSelectableFlags_SpanAllColumns))
	//{
	//	ImGui::TableNextRow();
	//	for (i32 i = 0; i < transformation.length(); i++)
	//	{
	//		ImGui::TableNextRow();
	//		for (i32 j = 0; j < transformation[i].length(); j++)
	//		{
	//			ImGui::TableNextColumn();
	//			ImGui::Text("%.6f", transformation[i][j]);
	//		}
	//	}
	//	ImGui::EndTable();
	//}

	if (insertFrame)
	{
		animComp->insertFrame(animComp->animationIndex, selectedFrame);
	}



	insertFrame = false;
}
