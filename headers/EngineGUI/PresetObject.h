#pragma once

#include <Core/Types.h>
#include <Components/IComponent.h>
#include <Components/RenderableComponent.h>
#include <Core/Mesh.h>
#include <Components/TransformComponent.h>
#include <Utils/FBXFileLoader.h>
#include <Core/MeshScene.h>
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
		AudioSource,

		// UI
		Text2D, Text3D,
		Button,


		PresetObjectEND
	};

	extern std::map< PresetCategory, String > PRESET_CATEGORY_NAMES_MAP;
	extern std::map< PresetObject, String > PRESET_OBJECT_NAMES_MAP;
	// Tuple: 0: texture index, 1: uv0, 2:uv1
	// For vec2: x: u coordinate, y: v coordinate
	extern std::map< PresetObject, std::tuple<i32, vec2, vec2> > PRESET_OBJECT_TEXTCOORDS_MAP;
	extern Array< String > PRESET_CATEGORY_NAMES_LIST;
	extern Array< String > PRESET_OBJECT_NAMES_LIST;

	extern std::map< PresetObject, std::map<NoxEngine::ComponentType, NoxEngine::IComponent *>> PRESET_OBJECT_COMPONENTS;

	// Objects contained in a category
	extern std::map< PresetCategory, Array<PresetObject> > PRESET_OBJECTS;
	void initPresetObjectVars();

	void loadPresetModels();
	void initPresetObject();
}
