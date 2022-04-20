#include <Entity.h>
#include <GameState.h>
#include <IComponent.h>

using namespace NoxEngine;
using NoxEngineUtils::Logger;


Entity::Entity(i32 _id) : id(_id), components(0), hasComp(0), name("") {

}

Entity::Entity(i32 _id, String _name) : id(_id), components(0), hasComp(0), name(_name) {

}

Entity::~Entity() {
	// Gotta be careful. When comp are destroyed the subsystem have to know
}

void Entity::addComp(IComponent *comp)
{
	assert(comp->id != 0);

	for (auto c : components)
	{
		if (c->id == comp->id)
		{
			Logger::debug("Component (ID: %d) already exists in Entity ", comp->id);
			return;
		}
	}

	components.push_back(comp);
	hasComp += pow(2, (comp->id - 1));
}


IComponent * Entity::getComp(i32 compId)
{
	for (auto c : components)
	{
		if (c->id == compId)
		{
			return c;
		}
	}

	Logger::debug("The component (ID: %d) does not exist in Entity (ID: %d)", compId, id);
	return nullptr;
}

