#pragma once

#include <map>
#include <vector>
#include <string>

#include <cassert>


namespace NoxEngineGUI {

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

	extern std::map< PresetCategory, std::string > PRESET_CATEGORY_NAMES_MAP;
	extern std::map< PresetObject, std::string > PRESET_OBJECT_NAMES_MAP;
	extern std::vector< std::string > PRESET_CATEGORY_NAMES_LIST;
	extern std::vector< std::string > PRESET_OBJECT_NAMES_LIST;

	// Objects contained in a category
	extern std::map< PresetCategory, std::vector<PresetObject> > PRESET_OBJECTS;


	void initPresetObjectVars();
}