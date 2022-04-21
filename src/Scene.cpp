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
}


void Scene::addEntity(Entity* ent) {

	assert(ent->id <= nEntitiesAdded);	// soft check on unique entity ID

	/*   Register to the appropriate subsystem based on the components it has   */ 
	if ((ent->hasComp & 0b11) == 0b11) {

		IRenderable* irend = ent->getComp(ComponentType::RenderableType)->CastType<IRenderable>();
		IPosition* ipos = ent->getComp(ComponentType::PositionType)->CastType<IPosition>();

		gm->GetRenderer()->addObject(
			irend,
			ipos
		);

		gm->GetRenderer()->updateBuffers();
	}

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
		ent->addComp(pos);
		break;

	// No need to add any component
	case EmptyEntity:
	default:
		break;

	}

	// Finally add the entity to the scene
	this->addEntity(ent);
}