/*
 * An interface: communication with Entity.
 * Allows a class that implements this interface to be used as a component of Entity. 
 * Typically, a class implementing this would also implement an interface comunicating with a subsystem of engine
 * 
 * Each component has an ID allowing to distinguish between them
*/
#pragma once

#include <ComponentType.h>

namespace NoxEngine {

	class IComponent
	{
		public:
			ComponentType id;
			// This function is implemented to be able to downcast classes stored as IComponent to their respective actual classes
			// TODO (Vincent): fix this witchcraft
			template <class T>
			inline T* CastType() { 
				void* ptr = dynamic_cast<T*>(this);
				return reinterpret_cast<T*>(ptr); 
			};

			virtual ~IComponent() {};
	};
}

