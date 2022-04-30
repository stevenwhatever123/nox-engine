#include "ComponentType.h"

// Component classes
#include <IComponent.h>
#include <TransformComponent.h>
#include <RenderableComponent.h>


namespace NoxEngine {

	// Give the extern maps/lists a definition in one compile unit
	std::map<std::type_index, ComponentType> kComponentTypeMap;
	std::map<ComponentType, String> kComponentTypeNames;
}


void NoxEngine::initComponentTypes() {
	
	kComponentTypeMap = {
		{ typeid(IComponent),			AbstractType },
		{ typeid(TransformComponent),	TransformType },
		{ typeid(RenderableComponent),	RenderableType }
		//{ typeid(LightSourceComponent),	LightSourceType },
		//{ typeid(AudioSourceComponent),	AudioSourceType },
		//{ typeid(ScriptComponent),	ScriptType },
	};

	kComponentTypeNames = {
		{ AbstractType,		"Abstract Component" },
		{ TransformType,	"Transform Component" },
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