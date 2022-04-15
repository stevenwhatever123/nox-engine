#pragma once

// Core GUI
#include "EngineGUI.h"

// Preset Object classes
// TODO: add


namespace NoxEngine {

	namespace EngineGUI {

		//struct PresetObjectParams {
		//};

		enum PresetObjectCategory {
			Geometry, Cards, Lights, Audio, UI, All
		};

		static std::map< PresetObjectCategory, std::string > PRESET_OBJECT_CATEGORY_NAMES {
			{ Geometry, "Geometry" },
			{ Cards,	"Cards" },
			{ Lights,	"Lights" },
			{ Audio,	"Audio" },
			{ UI,		"UI" },
			{ All,		"All Categories" }
		};

		static std::map< PresetObjectCategory, std::vector<std::string> > PRESET_OBJECTS {
			{ Geometry, { "Empty Game Object", "Cube", "Sphere" }},
			{ Cards,	{ "Rectangle Card" }},
			{ Lights,	{ "Light Source" }},
			{ Audio,	{ "Audio Source" }},
			{ UI,		{} }
			// "All" is just the values in the map, so we don't define them
		};

		static PresetObjectCategory selectedCategory = Geometry;

		

		void updatePresetObjectPanel(const GUIParams* params);

	}
}