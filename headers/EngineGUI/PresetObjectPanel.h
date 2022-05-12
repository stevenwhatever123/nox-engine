#pragma once

// Core GUI
#include "EngineGUI.h"

// Preset Object classes
#include "PresetObject.h"
#include <Core/GameState.h>

namespace NoxEngineGUI {

	//struct PresetObjectParams {
	//};

	// Selected category in the combo box / dropdown list
	static PresetCategory selectedCategory = PresetCategory::Geometry;

	static i32 iconWidth = 40;
	static i32 iconHeight = 40;
	static std::vector<ImTextureID> allTextures;

	void loadAtlasTexture(const char* path);

	void initIcons();
	void initPresetObjectPanel();
	void updatePresetObjectPanel(NoxEngine::GameState* params);

}
