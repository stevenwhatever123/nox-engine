#include <EngineGUI/AudioPanel.h>
#include <Managers/IOManager.h>

using namespace NoxEngine;


void NoxEngineGUI::updateAudioPanel(GameState* state, NoxEngineGUI::GUIParams *params) {

	// Variables
	std::string name = PANEL_NAME_MAP[PanelName::AudioPanel];
	AudioManager* am = AudioManager::Instance();

	// Window Begin
	ImGui::Begin(name.c_str());


	/*   Logic   */
	auto startItr = state->audioSources.begin();
	auto endItr = state->audioSources.end();
	
	// Load audio
	ImGui::Checkbox("Load as 3D Sound", &params->soundIs3d);

	/*
	if (ImGui::Button("Load")) {
		std::string file_path = IOManager::Instance()->PickFile("All Files\0*.*\0\0");

		std::string filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		state->audioSources.emplace(
			file_path,
			AudioSource{ 
				.name = filename, 
				.file = file_path, 
				.position = glm::vec3{0, 0, 0}, 
				.sourceVolume = 0.5,
				.is3d = params->soundIs3d
			});

		am->loadSound(file_path, params->soundIs3d);

	}

	// Display list of loaded audios & manipulation
	if (state->audioSources.size() > 0 && ImGui::TreeNode("Audios")) {

		while (startItr != endItr) {
			AudioSource* audioSource = &startItr->second;
			if (ImGui::TreeNode(audioSource->name.c_str())) {

				ImGui::Text("File Name: %s", audioSource->file.c_str());

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

				if (ImGui::TreeNode("Position")) {
					ImGui::DragFloat3("Position", &audioSource->position[0], 0.0001f, -1.0f, 1.0f, "%.4f");
					ImGui::TreePop();
				}

				ImGui::SliderFloat("Volume", &audioSource->sourceVolume, 0.0f, 1.0f, "%.1f");
				if (ImGui::Button("Play")) {
					//am->LoadSound(audio_source->file, true);
					am->playSounds(audioSource->file);
				}

				ImGui::BeginDisabled();
				ImGui::Checkbox("Is 3D", &audioSource->is3d);
				ImGui::EndDisabled();

				ImGui::SameLine();

				if (ImGui::Button("Stop")) {

					am->unLoadSound(audioSource->file);
				}

				ImGui::TreePop();
			}

			startItr++;
		}

		ImGui::TreePop();
	//}
	*/


	// Window End
	ImGui::End();
}
