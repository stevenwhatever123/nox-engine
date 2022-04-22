// System/std includes
#include <iostream>
#include <cassert>

#include <Entity.h>
#include <GameState.h>
#include <ComponentType.h>
#include <IComponent.h>
#include <PositionComponent.h>
#include <RenderableComponent.h>

using namespace NoxEngine;
using NoxEngineUtils::Logger;


Entity::Entity(i32 _id)
	:
	id(_id),
	hasComp(0), 
	name("") {

}

Entity::Entity(i32 _id, String _name) 
	: 
	id(_id), 
	hasComp(0), 
	name(_name) {

}

Entity::Entity(Scene* scene, String _name)
	:
	hasComp(0) {

	assert(scene != nullptr);

	// Form the placeholder name
	char entName[32];
	snprintf(entName, 32, "Game Object %i", scene->nEntitiesAdded + 1);

	// Assign values to fields
	id = scene->nEntitiesAdded;
	name = _name == "" ? entName : _name;

}

Entity::Entity(Entity&& other) : id(other.id), hasComp(other.hasComp), components(other.components), name(other.name) {
}

Entity::~Entity() {
	// Gotta be careful. When comp are destroyed the subsystem have to know
}


bool Entity::containsComps(HasCompBitMask mask) {
	return (hasComp & mask) == mask;
}


template <typename T>
void Entity::addComp(T *comp) {

	assert(comp->id != ComponentType::AbstractType);

	// Invalid type, don't add
	if (kComponentTypeMap.find(typeid(T)) == kComponentTypeMap.end()) {
		Logger::debug("Attempted to add invalid component type (%s), aborted", typeid(T).name());
		return;
	}

	if (components.find(typeid(T)) != components.end()) {
		Logger::debug("Component (ID: %d) already exists in Entity ", comp->id);
		return;
	}

	components[typeid(T)] = comp;
	hasComp |= ( 1 << (comp->id-1) );
}


template <typename T>
T *Entity::getComp() {

	// Invalid type, return nullptr
	if (kComponentTypeMap.find(typeid(T)) == kComponentTypeMap.end()) {
		Logger::debug("Invalid component type requested (typeid = %s)", typeid(T).name());
		return nullptr;
	}

	// Found component. Cast it to type T
	if (auto res = components.find(typeid(T)); res != components.end()) {
		return res->second->CastType<T>();
	}

	// Component does not exist in this entity
	return nullptr;
}


// explicit template instantiation
template void Entity::addComp<PositionComponent>(PositionComponent*);
template void Entity::addComp<RenderableComponent>(RenderableComponent*);

template PositionComponent* Entity::getComp<PositionComponent>();
template RenderableComponent* Entity::getComp<RenderableComponent>();
