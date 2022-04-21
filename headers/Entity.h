/*
 * Entity
 * A container class. What the object of this class is defined by the combinantion of the components.
 * 
 * Component IDs: 0 - unknown component (should not be. Smth went wrong)
 *				  1 - Position component (contains position of teh entity in the 3D scene) - bitmask 0000 0000 0000 0001 2^0
 *				  2 - Mesh component (contains geometry of the object. Used by Renderer) - bitmask 0000 0000 0000 0010 2^1
 *				  3 - Light component (contains the light properties of the entity. Used by Renderer) - bitmask 0000 0000 0000 0100 2^2
 *				  4 - Audio component (contains audio of the entity. Used by Audio) - bitmask 0000 0000 0000 1000 2^3
*/
#pragma once

// Engine Includes
#include <Types.h>
#include <Utils.h>

namespace NoxEngine {

	// Forward declares
	class IComponent;
	class Scene;

	class Entity
	{
		public:
			
			i32 id; 

			// The components that make the entity
			// TODO: change to array of arrays
			Array<IComponent*> components; 
			
			// TODO (Vincent): potential flaw - what if an entity needs more than 1 of the same type of components (e.g. multiple audio sources)
			HasCompBitMask hasComp;

			// A human-readable identifier - not necessarily unique
			String name; 

			// TODO (Vincent): Can an entity not contain another entity?
			//Entity* parent;

	public:
			Entity(i32 _id);
			Entity(i32 _id, String _name);

			// Automatically give a unique ID and a placeholder name to an entity
			// TODO: Change GameManager to Singleton, so it doesn't need a param
			Entity(Scene *scene, String _name = "");

			// Move constructor
			Entity(Entity&& other);

			// Gotta be careful. When comp are destroyed the subsystem have to know
			// TODO (Vincent): delete the components array and let the specialized component destroyer remove the reference in the subsystem?
			~Entity();
			
			// Add a component to the entity
			void addComp(IComponent *comp);
			// Gets the component with the ID provided. If no such comp -> through error and return
			IComponent * getComp(i32 id);
	};
}
