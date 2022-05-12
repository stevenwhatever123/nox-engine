// System/std includes
#include <cassert>

#include <Core/Entity.h>
#include <Core/GameState.h>
#include <Components/ComponentType.h>
#include <Components/IComponent.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AnimationComponent.h>
#include <Components/AudioSourceComponent.h>
#include <Components/AudioGeometryComponent.h>
#include <Components/ScriptComponent.h>

using namespace NoxEngine;
using NoxEngineUtils::Logger;


Entity::Entity(i32 _id, char* _name)
	: 
	id(_id), 
	hasComp(0),
	_isEnabled(~0),
	entityEnabled(true) {

	if (_name != nullptr) name = _name;
	else name = (char *)calloc(ENTITY_NAME_MAX_LEN, sizeof(char));

	filepath = nullptr;
}

Entity::Entity(Scene* scene, char* _name)
	:
	hasComp(0),
	_isEnabled(~0),
	entityEnabled(true) {

	assert(scene != nullptr);

	// Assign values to fields
	id = scene->nEntitiesAdded; 
	
	if (_name != nullptr) name = _name;
	else {
		// placeholder name
		name = (char*)calloc(ENTITY_NAME_MAX_LEN, sizeof(char));
		snprintf(name, ENTITY_NAME_MAX_LEN, "Entity %i", scene->nEntitiesAdded + 1);
	}
	filepath = nullptr;
}

Entity::Entity(Scene* scene, const char* _name)
	:
	hasComp(0),
	_isEnabled(~0),
	entityEnabled(true) {

	assert(scene != nullptr);

	// Assign values to fields
	id = scene->nEntitiesAdded;

	name = (char*)calloc(ENTITY_NAME_MAX_LEN, sizeof(char));
	memcpy(name, _name, ENTITY_NAME_MAX_LEN);

	filepath = nullptr;
}

Entity::Entity(Scene* scene, const char* _name, const char* _filepath)
	:
	hasComp(0),
	_isEnabled(~0),
	entityEnabled(true) {

	assert(scene != nullptr);

	// Assign values to fields
	id = scene->nEntitiesAdded;

	name = (char*)calloc(ENTITY_NAME_MAX_LEN, sizeof(char));
	filepath = (char*)calloc(ENTITY_NAME_MAX_LEN, sizeof(char));
	memcpy(name, _name, ENTITY_NAME_MAX_LEN);
	memcpy(filepath, _filepath, ENTITY_NAME_MAX_LEN);
}

Entity::Entity(Entity&& other) 
	: 
	id(other.id), 
	hasComp(other.hasComp), 
	components(other.components), 
	name(other.name), 
	_isEnabled(other._isEnabled),
	entityEnabled(true) {
}

Entity::~Entity() {

	free(name);

	// Emit removal signals for all existing components and let EventManager handle entity cleanup in subsystems
	for (const auto& itr : components) {
		SIGNAL_EVENT(EventNames::componentRemoved, this, std::type_index(itr.first));
	}

	// Mark the components as free
	components.clear();
}


bool Entity::containsComps(HasCompBitMask mask) {
	return !((hasComp & mask) ^ mask);
}

// TODO (Vincent): Extremely ugly solution, is there a better way?
//				   The problem is that a class cannot be stored in a map
void Entity::addComp(ComponentType type) {

	switch (type) {

	case TransformType:			addComp<TransformComponent>();			break;
	case RenderableType:		addComp<RenderableComponent>();			break;
	case AnimationType:			addComp<AnimationComponent>(); 			break;
	case AudioSourceType:		addComp<AudioSourceComponent>();		break;
	case AudioGeometryType:		addComp<AudioGeometryComponent>();		break;
	case ScriptType:			addComp<ScriptComponent>(); 			break;
	default:					LOG_DEBUG("Attempted to add invalid component type (%s), aborted", kComponentTypeNames[type].c_str());
	}
}


template <typename T> T* Entity::getComp() {

	// Invalid type, return nullptr
	if (kComponentTypeMap.find(typeid(T)) == kComponentTypeMap.end()) {
		//Logger::debug("Invalid component type requested (typeid = %s)", typeid(T).name());
		return nullptr;
	}

	// Found component. Cast it to type T
	if (auto res = components.find(typeid(T)); res != components.end()) {
		return res->second->CastType<T>();
	}

	// Component does not exist in this entity
	return nullptr;
}

template<typename T>
void Entity::addComp(T* comp) {

	assert(comp->id != ComponentType::AbstractType);

	if (kComponentTypeMap.find(typeid(T)) == kComponentTypeMap.end()) {
		return;
	}

	if (components.find(typeid(T)) != components.end()) {
		return;
	}

	components[typeid(T)] = comp;
	hasComp |= (1 << (comp->id - 1));

	addCompSignal<T>();
	comp->attachedToEntity(this);
}


bool Entity::isEnabled(u32 bit) {
	return _isEnabled & (1 << (bit - 1));
}


void Entity::tick(time_type dt, time_type currentTime) {
	const auto script = getComp<ScriptComponent>();
	if (script) {
		script->tick(dt, currentTime);
	}
}


// explicit template instantiation
template TransformComponent* Entity::getComp<TransformComponent>();
template RenderableComponent* Entity::getComp<RenderableComponent>();
template AnimationComponent* Entity::getComp<AnimationComponent>();
template AudioSourceComponent* Entity::getComp<AudioSourceComponent>();
template AudioGeometryComponent* Entity::getComp<AudioGeometryComponent>();
template ScriptComponent* Entity::getComp<ScriptComponent>();
