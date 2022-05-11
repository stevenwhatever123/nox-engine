#include <Components/ComponentType.h>

// Component classes
#include <Components/IComponent.h>
#include <Components/PositionComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AudioSourceComponent.h>
#include <Components/AudioGeometryComponent.h>
#include <Components/ScriptComponent.h>

namespace NoxEngine {

	// Give the extern maps/lists a definition in one compile unit
	std::map<std::type_index, ComponentType> kComponentTypeMap;
	std::map<ComponentType, String> kComponentTypeNames;
}


void NoxEngine::initComponentTypes() {
	
	kComponentTypeMap = {
		{ typeid(IComponent),			AbstractType },
		{ typeid(PositionComponent),	PositionType },
		{ typeid(RenderableComponent),	RenderableType },
		//{ typeid(LightSourceComponent),	LightSourceType },
		{ typeid(AudioSourceComponent),	AudioSourceType },
		//{ typeid(AudioListenerComponent),	AudioListenerType },
		{ typeid(AudioGeometryComponent),	AudioGeometryType }
		//{ typeid(ScriptComponent),	ScriptType },
	};

	kComponentTypeNames = {
		{ AbstractType,			"Abstract Component" },
		{ PositionType,			"Position Component" },
		{ RenderableType,		"Renderable Component" },
		{ LightSourceType,		"Light Source Component" },		// TODO: Implement
		{ AudioSourceType,		"Audio Source Component" },
		{ AudioListenerType,	"Audio Listener Component" },	// TODO: Implement
		{ AudioGeometryType,	"Audio Geometry Component" },
		{ ScriptType,			"Script Component" }
	};

	// assert every type has a name
	for (int i = 0; i < ComponentType::ComponentTypeCount; i++) {
		assert(kComponentTypeNames.find(static_cast<ComponentType>(i)) != kComponentTypeNames.end() && "A component type must have a name as a string - define it!");
	}
}
