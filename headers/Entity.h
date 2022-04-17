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

// System/std includes
#include <iostream>
#include <cassert>

// Engine Includes
#include <IComponent.h>
#include <Types.h>
#include <Utils.h>

namespace NoxEngine {

	class Entity
	{
		public:

			i32 id; // The ID of the sp entity. Used to distiguish between them
			Array<IComponent*> components; // The components that make the entity
			HasCompBitMask hasComp;

			Entity();
			// Gotta be careful. When comp are destroyed the subsystem have to know
			~Entity();
			
			// Add a component to the entity
			void addComp(IComponent *comp);
			// Gets the component with the ID provided. If no such comp -> through error and return
			IComponent * getComp(i32 id);

	};
}
