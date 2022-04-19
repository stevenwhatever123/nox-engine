#include <Scene.h>

using namespace NoxEngine;

Scene::Scene(): entities(0) {
}

void Scene::addEntity(Entity* ent) {
	entities.push_back(ent);
}
