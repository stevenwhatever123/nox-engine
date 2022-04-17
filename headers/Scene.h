/*
 * A collection of entities in one scene
*/

#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "Entity.h"

class Scene
{
private:

	

public:

	std::vector<Entity*> entities;

	Scene()
	{
		entities = std::vector<Entity*>();
	}


	void addEntity(Entity* ent)
	{
		entities.push_back(ent);
	}

};
