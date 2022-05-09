#include "EngineGUI/PresetObject.h"

namespace NoxEngineGUI {
	// Give the extern maps/lists a definition in one compile unit
	std::map< PresetCategory, std::string > PRESET_CATEGORY_NAMES_MAP;
	std::map< PresetObject, std::string > PRESET_OBJECT_NAMES_MAP;
	std::vector< std::string > PRESET_CATEGORY_NAMES_LIST;
	std::vector< std::string > PRESET_OBJECT_NAMES_LIST;
	std::map< PresetCategory, std::vector<PresetObject> > PRESET_OBJECTS;
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
		{ PresetObject::AudioSourceComponent,"Audio Source Component" },
		{ PresetObject::Text2D,				"2D Text" },
		{ PresetObject::Text3D,				"3D Text" },
		{ PresetObject::Button,				"Button" },
		{ PresetObject::PresetObjectEND,				"END" },
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
			PresetObject::AudioSourceComponent
		}},
		{ PresetCategory::UI, {
			PresetObject::Text2D,
			PresetObject::Text3D,
			PresetObject::Button }
		}
		// "All" is just the values in the map, so we don't define them
	};


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
