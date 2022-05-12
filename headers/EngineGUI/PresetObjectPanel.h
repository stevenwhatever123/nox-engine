#pragma once

// Core GUI
#include "EngineGUI.h"

// Preset Object classes
#include "PresetObject.h"

#include <GameState.h>


namespace NoxEngineGUI {

	//struct PresetObjectParams {
	//};

	// Selected category in the combo box / dropdown list
	static PresetCategory selectedCategory = PresetCategory::Geometry;

	void initPresetObjectPanel();
	void updatePresetObjectPanel(NoxEngine::GameState* params);

}