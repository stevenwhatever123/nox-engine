#include "ComponentType.h"

// Component classes
#include <IComponent.h>
#include <PositionComponent.h>
#include <RenderableComponent.h>


namespace NoxEngine {

	// Give the extern maps/lists a definition in one compile unit
	std::map<std::type_index, ComponentType> kComponentTypeMap;
}


void NoxEngine::initComponentTypes() {
	
	kComponentTypeMap = {
		{ typeid(IComponent),			AbstractType },
		{ typeid(PositionComponent),	PositionType },
		{ typeid(RenderableComponent),	RenderableType }
		//{ typeid(LightSourceComponent),	LightSourceType },
		//{ typeid(AudioSourceComponent),	AudioSourceType },
	};
}