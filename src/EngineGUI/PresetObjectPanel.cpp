#include "EngineGUI/PresetObjectPanel.h"

namespace NoxEngineGUI {

	/////////////////
	//   helpers   //
	/////////////////
	void beginPresetObjectDrag(PresetObject obj) {

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the index of our item (could be anything)
			ImGui::SetDragDropPayload("PRESET_OBJECT_DND", &obj, sizeof(PresetObject));

			// Display preview
			// TODO: make this look prettier
			ImGui::Text("%s", PRESET_OBJECT_NAMES_MAP[obj].c_str());
			ImGui::EndDragDropSource();
		}
	}


	// Initialize the categories / names of preset objects
	// Must be called once before `updatePresetObjectPanel()`
	void initPresetObjectPanel() {

		initPresetObjectVars();
	}


	void updatePresetObjectPanel(NoxEngine::GameState* state) {

		// Variables
		std::string name = PANEL_NAME_MAP[PanelName::PresetObjects];

		// Window Begin
		ImGui::Begin(name.c_str());


		// Top: categories
		{
			ImGuiComboFlags flags = 0;
			const char* previewValue = PRESET_CATEGORY_NAMES_LIST[selectedCategory].c_str();
			
			if (ImGui::BeginCombo("Preset Categories", previewValue, flags)) {
				for (auto &itr : PRESET_CATEGORY_NAMES_MAP) {

					// change selected category on press
					const bool isSelected = (selectedCategory == itr.first);
					if (ImGui::Selectable(itr.second.c_str(), isSelected))
						selectedCategory = itr.first;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

		}

		// Bottom: preset objects
		{
			// Show the list of preset objects from the selected category
			if (selectedCategory < PresetCategory::All) {

				for (auto &obj : PRESET_OBJECTS[selectedCategory]) {

					// Detect click
					if ( ImGui::Button(PRESET_OBJECT_NAMES_MAP[obj].c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50)) ) {
						printf("Selected object %s(%i)\n", PRESET_OBJECT_NAMES_MAP[obj].c_str(), obj);
					}

					// Object buttons are drag sources
					// Scene panel is the drop destination
					beginPresetObjectDrag(obj);
				}
			}

			// If "All" is selected (or an invalid enum value is somehow set), show all objects
			else {

				for (auto &itr : PRESET_OBJECT_NAMES_MAP) {

					// Detect click
					if ( ImGui::Button(itr.second.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50)) ) {
						printf("Selected object %s(%i)\n", itr.second.c_str(), itr.first);
					}

					// Object buttons are drag sources
					// Scene panel is the drop destination
					beginPresetObjectDrag(itr.first);
				}
			}

		}


		// Window End
		ImGui::End();

	}

}
