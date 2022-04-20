/*
 * A collection of entities in one scene
*/
#pragma once

#include "EngineGUI/PresetObject.h"
#include <Types.h>

namespace NoxEngine {

	// Forward declaration
	class Entity;

	class Scene
	{
		private:
		public:
			String name;	// name of the scene
			Array<Entity*> entities;

			// A counter that increments by 1 everytime an entity is added
			// but does not decrement when an entity is removed
			// This is used to automatically give entities a unique ID
			// TODO: To optimize entity removal, use std::map for entities
			//       key,value=Entity*,Entity*? The address of the entity is
			//       then the unique ID of the entity
			u32 nEntitiesAdded;	

			Scene();
			Scene(String _name);
			void addEntity(Entity* ent);
			void addEntity(NoxEngineGUI::PresetObject obj);
			//void removeEntity(i32 entID);
	};
}
