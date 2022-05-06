#pragma once

#include <Core/Types.h>
#include <map>


// TODO: Move this to ECS
namespace NoxEngineGUI {

	enum PresetCategory {
		Geometry, Cards, Lights, Audio, UI, All,
		PresetCategoryEND
	};

	enum PresetObject {
		// Geometry
		EmptyEntity,
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
		Button,

		PresetObjectEND
	};

	extern std::map< PresetCategory, String > PRESET_CATEGORY_NAMES_MAP;
	extern std::map< PresetObject, String > PRESET_OBJECT_NAMES_MAP;
	extern Array< String > PRESET_CATEGORY_NAMES_LIST;
	extern Array< String > PRESET_OBJECT_NAMES_LIST;

	// Objects contained in a category
	extern std::map< PresetCategory, Array<PresetObject> > PRESET_OBJECTS;
	void initPresetObjectVars();
}
