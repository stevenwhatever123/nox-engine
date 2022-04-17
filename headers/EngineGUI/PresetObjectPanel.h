#pragma once

// Core GUI
#include "EngineGUI.h"

// Preset Object classes
// TODO: add

#include <GameState.h>


namespace NoxEngineGUI {

	//struct PresetObjectParams {
	//};

	enum PresetCategory {
		Geometry, Cards, Lights, Audio, UI, All
	};

	enum PresetObject {
		// Geometry
		EmptyGameObject, 
		Transform,
		Cube, 
		Sphere,

		// Cards
		RectangleCard,
		Hand,
		Deck,

		// Lights
		PointLight, 
		AreaLight,

		// Audio
		AudioSourceComponent,

		// UI
		Text2D, Text3D, 
		Button
	};

	static std::map< PresetCategory, std::string > PRESET_CATEGORY_NAMES_MAP;
	static std::map< PresetObject,   std::string > PRESET_OBJECT_NAMES_MAP;
	static std::vector< std::string > PRESET_CATEGORY_NAMES_LIST;
	static std::vector< std::string > PRESET_OBJECT_NAMES_LIST;

	// Objects contained in a category
	static std::map< PresetCategory, std::vector<PresetObject> > PRESET_OBJECTS;

	// Selected category in the combo box / dropdown list
	static PresetCategory selectedCategory = PresetCategory::Geometry;

		

	void initPresetObjectPanel();
	void updatePresetObjectPanel(NoxEngine::GameState* params);

}