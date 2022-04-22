#pragma once

#include <map>
#include <typeindex>

#include <Types.h>


namespace NoxEngine {

	// Note (Vincent): This implies the developer cannot create new component types
	enum ComponentType : i32 {
		AbstractType = 0,
		PositionType,		// TODO: change to Transform (pos, rot, scale)
		RenderableType,
		LightSourceType,
		AudioSourceType,
		ScriptType,
		ComponentTypeCount
	};

	enum ComponentTypeFlag : HasCompBitMask {
		AbstractFlag		= 0,
		PositionFlag		= 1 << (PositionType		- 1),
		RenderableFlag		= 1 << (RenderableType		- 1),
		LightSourceFlag		= 1 << (LightSourceType		- 1),
		AudioSourceFlag		= 1 << (AudioSourceType		- 1),
		ScriptFlag			= 1 << (ScriptType			- 1),
	};

	extern std::map<std::type_index, ComponentType> kComponentTypeMap;


	// Fill the kComponentTypeMap
	void initComponentTypes();
}