#include <Core/Scene.h>

#include <Managers/GameManager.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/EmissionComponent.h>
#include <Core/Entity.h>

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
	TransformComponent* transform;
	EmissionComponent* em;

	// Check which preset object is needed, create components accordingly
	switch (obj) {

		// Object with only transform component
		case Transform:
			transform = new TransformComponent(0.0, 0.0, 0.0);
			ent->addComp<TransformComponent>(transform);
			break;

		// In case it is light 
		// Create pos comp and light comp
		case PointLight:
			transform = new TransformComponent(0.0, 0.0, 0.0);
			em = new EmissionComponent();
			ent->addComp<TransformComponent>(transform);
			ent->addComp<EmissionComponent>(em);
			break;


		case EmptyEntity:
		default:
			break;

	}
	this->addEntity(ent);
}
