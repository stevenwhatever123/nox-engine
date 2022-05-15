#include "EngineGUI/PresetObject.h"

namespace NoxEngineGUI {
	// Give the extern maps/lists a definition in one compile unit
	std::map< PresetCategory, std::string > PRESET_CATEGORY_NAMES_MAP;
	std::map< PresetObject, std::string > PRESET_OBJECT_NAMES_MAP;
	std::map< PresetObject, std::tuple<i32, vec2, vec2> > PRESET_OBJECT_TEXTCOORDS_MAP;
	std::vector< std::string > PRESET_CATEGORY_NAMES_LIST;
	std::vector< std::string > PRESET_OBJECT_NAMES_LIST;
	std::map< PresetCategory, std::vector<PresetObject> > PRESET_OBJECTS;

	std::map< PresetObject, std::map<NoxEngine::ComponentType, NoxEngine::IComponent *>> PRESET_OBJECT_COMPONENTS;
}


void NoxEngineGUI::initPresetObjectVars() {

	// Give categories names
	PRESET_CATEGORY_NAMES_MAP = {
		{ PresetCategory::Geometry, "Geometry" },
		{ PresetCategory::Cards,	"Cards" },
		{ PresetCategory::Lights,	"Lights" },
		{ PresetCategory::Audio,	"Audio" },
		{ PresetCategory::UI,		"UI" },
		{ PresetCategory::All,		"All Categories" },
		{ PresetCategory::PresetCategoryEND,		"END" }
	};

	// Give objects names
	PRESET_OBJECT_NAMES_MAP = {
		{ PresetObject::EmptyEntity,		"Empty Entity" },
		{ PresetObject::Transform,			"Transform Component" },
		{ PresetObject::Cube,				"Cube" },
		{ PresetObject::Sphere,				"Sphere" },
		{ PresetObject::RectangleCard,		"Rectangle Card" },
		{ PresetObject::Hand,				"Hand" },
		{ PresetObject::Deck,				"Deck" },
		{ PresetObject::PointLight,			"Point Light" },
		{ PresetObject::AreaLight,			"Area Light" },
		{ PresetObject::AudioSource,		"Audio Source Component" },
		{ PresetObject::Text2D,				"2D Text" },
		{ PresetObject::Text3D,				"3D Text" },
		{ PresetObject::Button,				"Button" },
		{ PresetObject::PresetObjectEND,	"END" },
	};

	// Give objects textCoords
	PRESET_OBJECT_TEXTCOORDS_MAP = {
		{ PresetObject::EmptyEntity,		std::make_tuple(0, vec2(0, 0), vec2(0, 0))},
		{ PresetObject::Transform,			std::make_tuple(0, vec2(0, 0), vec2(128.0f / 2688.0f, 123.0f / 1664.0f))},
		{ PresetObject::Cube,				std::make_tuple(0, vec2(2176.0f / 2688.0f, 636.0f / 1664.0f), vec2(2304.0f / 2684.0f, 764.0f / 1664.0f))},
		{ PresetObject::Sphere,				std::make_tuple(0, vec2(896.0f/ 2688.0f, 124.0f / 1664.0f), vec2(1024.0f / 2688.0f, 252.0f / 1664.0f))},
		{ PresetObject::RectangleCard,		std::make_tuple(0, vec2(128.0f / 2688.0f, 764.0f / 1664.0f), vec2(256.0f / 2688.0f, 892.0f / 1664.0f))},
		{ PresetObject::Hand,				std::make_tuple(0, vec2(768.0f / 2688.0f, 1148.0f / 1664.0f), vec2(896.0f / 2688.0f, 1276.0f / 1664.0f))},
		{ PresetObject::Deck,				std::make_tuple(0, vec2(512.0f / 2688.0f, 636.0f / 1664.0f), vec2(640.0f / 2688.0f, 764.0f / 1664.0f))},
		{ PresetObject::PointLight,			std::make_tuple(0, vec2(0.0f / 2688.0f, 380.0f / 1664.0f), vec2(128.0f / 2688.0f, 508.0f / 1664.0f))},
		{ PresetObject::AreaLight,			std::make_tuple(0, vec2(1792.0f / 2688.0f, 1276.0f / 1664.0f), vec2(1920.0f / 2688.0f, 1404.0f / 1664.0f))},
		{ PresetObject::AudioSource,		std::make_tuple(1, vec2(0.0f / 1024.0f, 700.0f / 2048.0f), vec2(100.0f / 1024.0f, 800.0f / 2048.0f))},
		{ PresetObject::Text2D,				std::make_tuple(1, vec2(400.0f / 1024.0f, 1700.0f / 2048.0f), vec2(500.0f / 1024.0f, 1800.0f / 2048.0f))},
		{ PresetObject::Text3D,				std::make_tuple(1, vec2(400.0f / 1024.0f, 1800.0f / 2048.0f), vec2(500.0f / 1024.0f, 1900.0f / 2048.0f))},
		{ PresetObject::Button,				std::make_tuple(0, vec2(2176.0f / 2688.0f, 0.0f / 1664.0f), vec2(2304.0f / 2688.0f, 124.0f / 1664.0f))},
		{ PresetObject::PresetObjectEND,	std::make_tuple(0, vec2(896.0f / 2688.0f, 124.0f / 1664.0f), vec2(1024.0f / 2688.0f, 252.0f / 1664.0f))},
	};

	// Create lists of names
	for (auto itr : PRESET_CATEGORY_NAMES_MAP) {
		PRESET_CATEGORY_NAMES_LIST.push_back(itr.second);
	}
	for (auto itr : PRESET_OBJECT_NAMES_MAP) {
		PRESET_OBJECT_NAMES_LIST.push_back(itr.second);
	}

	// Form relationship between category and preset objects
	PRESET_OBJECTS = {
		{ PresetCategory::Geometry, {
			PresetObject::EmptyEntity,
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
			PresetObject::AudioSource
		}},
		{ PresetCategory::UI, {
			PresetObject::Text2D,
			PresetObject::Text3D,
			PresetObject::Button }
		}
		// "All" is just the values in the map, so we don't define them
	};

	initPresetObject();

	for (int i = 0; i <= PresetCategory::PresetCategoryEND; i++) {
		assert(PRESET_CATEGORY_NAMES_MAP.find(static_cast<PresetCategory>(i)) != PRESET_CATEGORY_NAMES_MAP.end() && "A preset category must have a name as a string - define it!");
		//printf("%s   %s\n", PRESET_CATEGORY_NAMES_LIST[i].c_str(), PRESET_CATEGORY_NAMES_MAP[static_cast<PresetCategory>(i)].c_str());
		//assert(strcmp(PRESET_CATEGORY_NAMES_LIST[i].c_str(), PRESET_CATEGORY_NAMES_MAP[static_cast<PresetCategory>(i)].c_str()) != 0 && "A preset category must have a name as a string - define it!");
	}
	for (int i = 0; i < PresetObject::PresetObjectEND; i++) {
		assert(PRESET_OBJECT_NAMES_MAP.find(static_cast<PresetObject>(i)) != PRESET_OBJECT_NAMES_MAP.end() && "A preset object must have a name as a string - define it!");
		//assert(PRESET_OBJECT_NAMES_LIST[i] != PRESET_OBJECT_NAMES_MAP[static_cast<PresetObject>(i)] && "A preset object must have a name as a string - define it!");
	}

	// TODO: assert each object appears exactly once in PRESET_OBJECTS
}

void NoxEngineGUI::initPresetObject()
{
	// Steven: Seems like we cannot initialise like the way we did above
	// Transform Preset
	PRESET_OBJECT_COMPONENTS[PresetObject::Transform] = {
		{NoxEngine::TransformType,		new NoxEngine::TransformComponent(0, 0, 0)}
	};
	// Cube Preset
	PRESET_OBJECT_COMPONENTS[PresetObject::Cube] = {
		{NoxEngine::TransformType,		new NoxEngine::TransformComponent(0, 0, 0)},
		{NoxEngine::RenderableType,		new NoxEngine::RenderableComponent(0.0f, 0.0f, 0.0f, "")}
	};
	// Sphere Preset
	String file_name = "assets/meshes/sphere.fbx";
	NoxEngine::MeshScene sphereScene = NoxEngine::readFBX(file_name.c_str());
	PRESET_OBJECT_COMPONENTS[PresetObject::Sphere] = {
		{NoxEngine::TransformType,		new NoxEngine::TransformComponent(0, 0, 0)},
		{NoxEngine::RenderableType,		new NoxEngine::Mesh(*sphereScene.meshes[0])}
	};
	// Card Preset
	file_name = "assets/meshes/card.fbx";
	NoxEngine::MeshScene cardScene = NoxEngine::readFBX(file_name.c_str());
	PRESET_OBJECT_COMPONENTS[PresetObject::RectangleCard] = {
		{NoxEngine::TransformType,		new NoxEngine::TransformComponent(0, 0, 0)},
		{NoxEngine::RenderableType,		new NoxEngine::Mesh(*cardScene.meshes[0])}
	};
}