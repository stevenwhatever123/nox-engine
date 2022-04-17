#include <EngineGUI/AudioPanel.h>


void NoxEngineGUI::updateAudioPanel(NoxEngine::GameState *game_state) {

	// Variables
	std::string name = PANEL_NAME_MAP[PanelName::AudioSource];

	// Window Begin
	ImGui::Begin(name.c_str());

	// Sliders to control the 3D sound position
	static float soundX = 0.0f, soundY = 0.0f, soundZ = 0.0f;
	static float soundVolume = 0.0f;
	ImGui::SliderFloat("Sound X", &soundX, -1.0f, 1.0f, "X = %.3f");
	ImGui::SliderFloat("Sound Y", &soundY, -1.0f, 1.0f, "Y = %.3f");
	ImGui::SliderFloat("Sound Z", &soundZ, -1.0f, 1.0f, "Z = %.3f");
	ImGui::SliderFloat("Sound Volume", &soundVolume, -20.0f, 2.0f, "Volume (dB) = %.3f");


	auto startItr = game_state->audioSources.begin();
	auto endItr = game_state->audioSources.end();

	// static bool show_demo_window = true;
	if(ImGui::Button("load")) {
		std::string file_path = NoxEngine::IOManager::Instance()->PickFile("All Files\0*.*\0\0");

		std::string filename = file_path.substr(file_path.find_last_of("/\\") + 1);

		game_state->audioSources.emplace(
				file_path,
				NoxEngine::AudioSource{filename, file_path, glm::vec3{0, 0, 0}, 0.2});

			NoxEngine::AudioManager::Instance()->LoadSound(file_path, true);

	}

	if(ImGui::TreeNode("Audios")) {

		while(startItr != endItr) {
			NoxEngine::AudioSource *audio_source = &startItr->second;
			if(ImGui::TreeNode(audio_source->name.c_str())) {
				ImGui::Text("File Name: %s", audio_source->file.c_str());
				if(ImGui::TreeNode("Position")) {
					ImGui::DragFloat3("Position", &audio_source->position[0]);
					ImGui::TreePop();
				}

				ImGui::SliderFloat("Volume: ", &audio_source->sourceVolume, -20.0f, 2.0f, "Volume (dB) = %.3f");
				if(ImGui::Button("Play")) {
					NoxEngine::AudioManager::Instance()->LoadSound(audio_source->file, true);
					NoxEngine::AudioManager::Instance()->PlaySounds(audio_source->file);
				}


				if(ImGui::Button("Stop")) {

					NoxEngine::AudioManager::Instance()->UnLoadSound(audio_source->file);
				}

				ImGui::TreePop();
			}

			startItr++;
		}

		ImGui::TreePop();
	}


	// Update systems with UI values
	// params->audioManager->SetChannel3dPosition(0, {soundX, soundY, soundZ});
	// params->audioManager->SetChannelVolume(0, soundVolume);
	//customWindow->showFBXLoaderMenu(mesh);

	// Window End
	ImGui::End();
}
