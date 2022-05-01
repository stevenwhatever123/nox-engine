#include <EngineGUI/AudioPanel.h>


void NoxEngineGUI::updateAudioPanel(NoxEngine::GameState* game_state) {

	// Variables
	std::string name = PANEL_NAME_MAP[PanelName::AudioSource];

	// Window Begin
	ImGui::Begin(name.c_str());


	/*   Logic   */
	auto startItr = game_state->audioSources.begin();
	auto endItr = game_state->audioSources.end();

	// static bool show_demo_window = true;
	if (ImGui::Button("Load")) {
		std::string file_path = NoxEngine::IOManager::Instance()->PickFile("All Files\0*.*\0\0");

		std::string filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		game_state->audioSources.emplace(
			file_path,
			NoxEngine::AudioSource{ filename, file_path, glm::vec3{0, 0, 0}, 0.5 });

		NoxEngine::AudioManager::Instance()->LoadSound(file_path, true);

	}

	if (game_state->audioSources.size() > 0 && ImGui::TreeNode("Audios")) {

		while (startItr != endItr) {
			NoxEngine::AudioSource* audio_source = &startItr->second;
			if (ImGui::TreeNode(audio_source->name.c_str())) {
				ImGui::Text("File Name: %s", audio_source->file.c_str());
				if (ImGui::TreeNode("Position")) {
					ImGui::DragFloat3("Position", &audio_source->position[0], 0.0001f, -1.0f, 1.0f, "%.4f");
					ImGui::TreePop();
				}

				ImGui::SliderFloat("Volume", &audio_source->sourceVolume, 0.0f, 1.0f, "%.1f");
				if (ImGui::Button("Play")) {
					//NoxEngine::AudioManager::Instance()->LoadSound(audio_source->file, true);
					NoxEngine::AudioManager::Instance()->PlaySounds(audio_source->file);
				}

				ImGui::SameLine();

				if (ImGui::Button("Stop")) {

					NoxEngine::AudioManager::Instance()->UnLoadSound(audio_source->file);
				}

				ImGui::TreePop();
			}

			startItr++;
		}

		ImGui::TreePop();
	}


	// Window End
	ImGui::End();
}
