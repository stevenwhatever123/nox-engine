#include <Components/ComponentType.h>

// Component classes
#include <Components/IComponent.h>
#include <Components/PositionComponent.h>
#include <Components/RenderableComponent.h>
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
		{ typeid(ScriptComponent),	ScriptType }
		//{ typeid(LightSourceComponent),	LightSourceType },
		//{ typeid(AudioSourceComponent),	AudioSourceType },
	};

	kComponentTypeNames = {
		{ AbstractType,		"Abstract Component" },
		{ PositionType,		"Position Component" },
		{ RenderableType,	"Renderable Component" },
		{ LightSourceType,	"Light Source Component" },
		{ AudioSourceType,	"Audio Source Component" },
		{ ScriptType,		"Script Component" }
	};

	// assert every type has a name
	for (int i = 0; i < ComponentType::ComponentTypeCount; i++) {
		assert(kComponentTypeNames.find(static_cast<ComponentType>(i)) != kComponentTypeNames.end() && "A component type must have a name as a string - define it!");
	}
}
