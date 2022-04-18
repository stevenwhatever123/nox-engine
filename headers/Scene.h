/*
 * A collection of entities in one scene
*/
#pragma once

#include "Entity.h"
#include <Types.h>

namespace NoxEngine {
	class Scene
	{
		private:
		public:
			Array<Entity*> entities;
			Scene();
			void addEntity(Entity* ent);
	};
}
