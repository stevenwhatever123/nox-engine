#include "Components/ComponentType.h"

// Component classes
#include <Components/IComponent.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AnimationComponent.h>
#include <Components/ScriptComponent.h>


namespace NoxEngine {

	// Give the extern maps/lists a definition in one compile unit
	std::map<std::type_index, ComponentType> kComponentTypeMap;
	std::map<ComponentType, String> kComponentTypeNames;
}


void NoxEngine::initComponentTypes() {
	
	kComponentTypeMap = {
		{ typeid(IComponent),			AbstractType },
		{ typeid(TransformComponent),	TransformType },
		{ typeid(RenderableComponent),	RenderableType },
		{ typeid(AnimationComponent),	AnimationType},
		{ typeid(ScriptComponent),		ScriptType }
		//{ typeid(LightSourceComponent),	LightSourceType },
		//{ typeid(AudioSourceComponent),	AudioSourceType },
	};

	kComponentTypeNames = {
		{ AbstractType,		"Abstract Component" },
		{ TransformType,	"Transform Component" },
		{ RenderableType,	"Renderable Component" },
		{ AnimationType,	"Animation Component" },
		{ ScriptType,		"Script Component" },
		{ LightSourceType,	"Light Source Component" },
		{ AudioSourceType,	"Audio Source Component" }
	};

	// assert every type has a name
	for (int i = 0; i < ComponentType::ComponentTypeCount; i++) {
		assert(kComponentTypeNames.find(static_cast<ComponentType>(i)) != kComponentTypeNames.end() && "A component type must have a name as a string - define it!");
	}
}
