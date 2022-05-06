#include <Scene.h>

#include <GameManager.h>
#include "PositionComponent.h"
#include "RenderableComponent.h"
#include <Entity.h>

using namespace NoxEngine;
using namespace NoxEngineGUI;

Scene::Scene(String _name) : entities(0), nEntitiesAdded(0), name(_name), gm(GameManager::Instance()) {
}


Scene::~Scene() {
	
	// TODO (Vincent): Delete entities

	gm->scheduleUpdateECS();
}


void Scene::addEntity(Entity* ent) {

	assert(ent->id <= nEntitiesAdded);	// soft check on unique entity ID

	// Add to entities list
	entities.push_back(ent);
	nEntitiesAdded++;
}


void Scene::addEntity(PresetObject obj) {

	Entity* ent = new Entity(this);
	
	// Empty components created before the switch/case, allocate memory with `new` inside
	PositionComponent* pos;
	RenderableComponent* rend;

	// Check which preset object is needed, create components accordingly
	switch (obj) {

	// Object with only transform component
	case Transform:
		pos = new PositionComponent(0.0, 0.0, 0.0);
		ent->addComp<PositionComponent>(pos);
		break;

	// No need to add any component
	case EmptyEntity:
	default:
		break;

	}

	// Finally add the entity to the scene
	this->addEntity(ent);
}


//void Entity::removeEntity(u32 entID) {
//	gm->scheduleUpdateECS();
//}
