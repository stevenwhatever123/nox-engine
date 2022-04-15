#include "EngineGUI/PresetObjectPanel.h"


namespace NoxEngine {

	namespace EngineGUI {

		void updatePresetObjectPanel(const GUIParams* params) {

			// Variables
			std::string name = PANEL_NAME_MAP[PanelName::PresetObjects];

			// Window Begin
			ImGui::Begin(name.c_str());


			// Left side: categories
			{
				ImGui::BeginChild("Preset Categories", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetWindowContentRegionWidth() * 0.45f), false);

				for (auto itr : PRESET_OBJECT_CATEGORY_NAMES) {

					// change selected category on button press
					if ( ImGui::Button(itr.second.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50)) ) {
						selectedCategory = itr.first;
					}
				}

				ImGui::EndChild();
			}

			ImGui::SameLine();

			// Right side: preset objects
			{
				ImGui::BeginChild("Preset Objects", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, ImGui::GetWindowContentRegionWidth() * 0.45f), false);

				// Show the list of preset objects from the selected category
				if (selectedCategory < PresetObjectCategory::All) {

					for (auto name : PRESET_OBJECTS[selectedCategory]) {

						// Summon a new object in the select on click
						// TODO: support drag & drop
						if (ImGui::Button(name.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50))) {
							std::cout << name << "\n";
						}
					}
				}

				// If "All" is selected (or an invalid enum value is chosen), show all objects
				else {
					// TODO: implement
				}

				ImGui::EndChild();
			}


			// Window End
			ImGui::End();
		}

	}
}