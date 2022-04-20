#include <Scene.h>

#include "PositionComponent.h"
#include "RenderableComponent.h"
#include <Entity.h>

using namespace NoxEngine;
using namespace NoxEngineGUI;

Scene::Scene(): entities(0), nEntitiesAdded(0), name("") {
}


Scene::Scene(String _name) : entities(0), nEntitiesAdded(0), name(_name) {
}


void Scene::addEntity(Entity* ent) {
	entities.push_back(ent);
	nEntitiesAdded++;
}


void Scene::addEntity(PresetObject obj) {

	// Form the placeholder name
	char entName[32];
	snprintf(entName, 32, "Game Object %i", nEntitiesAdded+1);

	// Create an empty entity. We will fill components in it
	Entity* ent = new Entity(nEntitiesAdded, entName);

	// Empty components created before the switch/case, allocate memory with `new` inside
	PositionComponent* pos;
	RenderableComponent* rend;

	// Check which preset object is needed, create components accordingly
	switch (obj) {

	// Object with only transform component
	case Transform:
		pos = new PositionComponent(0.0, 0.0, 0.0);
		ent->addComp(pos);
		break;

	// No need to add any component
	case EmptyGameObject:
	default:
		break;

	}

	// Finally add the entity to the scene
	this->addEntity(ent);
}