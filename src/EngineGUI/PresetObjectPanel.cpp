#include "EngineGUI/PresetObjectPanel.h"


namespace NoxEngineGUI {

	// Initialize the categories / names of preset objects
	// Must be called once before `updatePresetObjectPanel()`
	void initPresetObjectPanel() {

		// Give categories names
		PRESET_CATEGORY_NAMES_MAP = {
			{ PresetCategory::Geometry, "Geometry" },
			{ PresetCategory::Cards,	"Cards" },
			{ PresetCategory::Lights,	"Lights" },
			{ PresetCategory::Audio,	"Audio" },
			{ PresetCategory::UI,		"UI" },
			{ PresetCategory::All,		"All Categories" }
		};

		// Give objects names
		PRESET_OBJECT_NAMES_MAP = {
			{ PresetObject::EmptyGameObject,	"Empty Game Object" },
			{ PresetObject::Transform,			"Transform Component" },
			{ PresetObject::Cube,				"Cube" },
			{ PresetObject::Sphere,				"Sphere" },
			{ PresetObject::RectangleCard,		"Rectangle Card" },
			{ PresetObject::Hand,				"Hand" },
			{ PresetObject::Deck,				"Deck" },
			{ PresetObject::PointLight,			"Point Light" },
			{ PresetObject::AreaLight,			"Area Light" },
			{ PresetObject::AudioSourceComponent,"Audio Source Component" },
			{ PresetObject::Text2D,				"2D Text" },
			{ PresetObject::Text3D,				"3D Text" },
			{ PresetObject::Button,				"Button" }
		};

		// Create lists of names
		for (auto itr : PRESET_CATEGORY_NAMES_MAP) {
			PRESET_CATEGORY_NAMES_LIST.emplace_back(itr.second);
		}
		for (auto itr : PRESET_OBJECT_NAMES_MAP) {
			PRESET_OBJECT_NAMES_LIST.emplace_back(itr.second);
		}

		// Form relationship between category and preset objects
		PRESET_OBJECTS = {
			{ PresetCategory::Geometry, { 
				PresetObject::EmptyGameObject, 
				PresetObject::Transform, 
				PresetObject::Cube, 
				PresetObject::Sphere }
			},
			{ PresetCategory::Cards, { 
				PresetObject::RectangleCard, 
				PresetObject::Hand, 
				PresetObject::Deck }
			},
			{ PresetCategory::Lights, { 
				PresetObject::PointLight, 
				PresetObject::AreaLight }
			},
			{ PresetCategory::Audio, { 
				PresetObject::AudioSourceComponent 
			}},
			{ PresetCategory::UI, { 
				PresetObject::Text2D, 
				PresetObject::Text3D, 
				PresetObject::Button }
			}
			// "All" is just the values in the map, so we don't define them
		};

		
		// asserts to make sure all categories/objects are associated
		// Important note: Need to update the end condition if more enum values are added after these values
		for (int i = 0; i <= PresetCategory::All; i++) {
			assert( PRESET_CATEGORY_NAMES_MAP.find(static_cast<PresetCategory>(i)) != PRESET_CATEGORY_NAMES_MAP.end() && "A preset category must have a name as a string - define it!");
		}
		for (int i = 0; i <= PresetObject::Button; i++) {
			assert( PRESET_OBJECT_NAMES_MAP.find(static_cast<PresetObject>(i)) != PRESET_OBJECT_NAMES_MAP.end() && "A preset object must have a name as a string - define it!");
		}
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

					// Summon a new object in the scene on click
					// TODO: support drag & drop
					if ( ImGui::Button(PRESET_OBJECT_NAMES_MAP[obj].c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50)) ) {
						printf("Selected object %s(%i)\n", PRESET_OBJECT_NAMES_MAP[obj].c_str(), obj);
					}
				}
			}

			// If "All" is selected (or an invalid enum value is somehow set), show all objects
			else {

				for (auto &itr : PRESET_OBJECT_NAMES_MAP) {

					// Summon a new object in the scene on click
					// TODO: support drag & drop
					if ( ImGui::Button(itr.second.c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 50)) ) {
						printf("Selected object %s(%i)\n", itr.second.c_str(), itr.first);
					}
				}
			}

		}


		// Window End
		ImGui::End();

	}

}