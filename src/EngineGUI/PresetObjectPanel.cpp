#include "EngineGUI/PresetObjectPanel.h"

#include "EngineGUI/ImGuiWidgets.h"


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
			auto iduv = PRESET_OBJECT_TEXTCOORDS_MAP[obj];

			ImGui::Image(
				allTextures[std::get<0>(iduv)], 
				ImVec2(48, 48), 
				ImVec2(std::get<1>(iduv).x, std::get<1>(iduv).y),
				ImVec2(std::get<2>(iduv).x, std::get<2>(iduv).y)
			);

			//ImGui::Text("%s", PRESET_OBJECT_NAMES_MAP[obj].c_str());
			ImGui::EndDragDropSource();
		}
	}

	void loadAtlasTexture(const char* path)
	{
		// Load the texture
		// Load from file
		int image_width = 0;
		int image_height = 0;
		unsigned char* image = stbi_load(path, &image_width, &image_height, NULL, 4);

		// Pass to opengl and get the texture id
		GLuint imageId;
		glGenTextures(1, &imageId);
		glBindTexture(GL_TEXTURE_2D, imageId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			image);
		stbi_image_free(image);

		allTextures.push_back((ImTextureID)(intptr_t)imageId);
	}

	void initIcons()
	{
		// Load the texture
		const char* filename = "assets/ui/iconsDouble.png";
		loadAtlasTexture(filename);

		const char* filename2 = "assets/ui/sheet_white2x.png";
		loadAtlasTexture(filename2);
	}


	// Initialize the categories / names of preset objects
	// Must be called once before `updatePresetObjectPanel()`
	void initPresetObjectPanel() {
		initPresetObjectVars();
		initIcons();
	}


	void updatePresetObjectPanel(NoxEngine::GameState* state) {

		// Variables
		std::string name = kPanelNameMap[PanelName::PresetObjects];

		// Window Begin
		ImGui::Begin(name.c_str());


		// Top: categories
		{
			ImGuiComboFlags flags = 0;
			const char* previewValue = PRESET_CATEGORY_NAMES_LIST[selectedCategory].c_str();
			
			ImGui::Text("Category");	ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo("##preset_categories", previewValue, flags)) {
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
			std::map< PresetObject, std::tuple<i32, vec2, vec2> > textureMap = PRESET_OBJECT_TEXTCOORDS_MAP;

			// Show the list of preset objects from the selected category
			if (selectedCategory < PresetCategory::All) {

				for (auto& obj : PRESET_OBJECTS[selectedCategory]) {

					if (ImGui::ImageButtonWithText(allTextures[std::get<0>(textureMap[obj])], PRESET_OBJECT_NAMES_MAP[obj].c_str(),
						ImVec2((f32)iconWidth, (f32)iconHeight), 
						ImVec2(std::get<1>(textureMap[obj]).x, std::get<1>(textureMap[obj]).y),
						ImVec2(std::get<2>(textureMap[obj]).x, std::get<2>(textureMap[obj]).y)))
					{
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
					if (ImGui::ImageButtonWithText(allTextures[std::get<0>(textureMap[itr.first])], PRESET_OBJECT_NAMES_MAP[itr.first].c_str(),
						ImVec2((f32)iconWidth, (f32)iconHeight),
						ImVec2(std::get<1>(textureMap[itr.first]).x, std::get<1>(textureMap[itr.first]).y),
						ImVec2(std::get<2>(textureMap[itr.first]).x, std::get<2>(textureMap[itr.first]).y)))
					{
						printf("Selected object %s(%i)\n", PRESET_OBJECT_NAMES_MAP[itr.first].c_str(), itr.first);
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
