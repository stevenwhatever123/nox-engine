#include "EngineGUI/AudioPanel.h"

using namespace NoxEngine;
using namespace EngineGUI;


namespace NoxEngine {

	namespace EngineGUI {

		void updateAudioPanel(const GUIParams* params) {

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

			// Update systems with UI values
			params->audioManager->SetChannel3dPosition(0, {soundX, soundY, soundZ});
			params->audioManager->SetChannelVolume(0, soundVolume);
			//customWindow->showFBXLoaderMenu(mesh);


			// Window End
			ImGui::End();
		}
	}
}