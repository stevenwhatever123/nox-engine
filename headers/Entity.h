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
#include <ComponentType.h>

namespace NoxEngine {

	// Forward declares
	class IComponent;
	class Scene;

	class Entity
	{
		// make Scene a friend so that Entities can only be created in Scene
		friend class Scene;

	public:
		// A human-readable identifier - not necessarily unique
		String name;


	private:
		// unique entity id
		i32 id; 

		// The components that make the entity
		// Note (Vincent): An entity can only contain one component of each type.
		//                 In the case where multiple of the same components is desired (e.g. multiple BoneComponents),
		//                 create a container component (e.g. SkeletonComponent) and attach it
		// https://stackoverflow.com/questions/20720360/ecs-can-an-entity-have-more-than-one-component-of-given-type
		// TODO (Vincent): Change to std::map<ComponentType,IComponent*> for log(N) search
		Array<IComponent*> components; 
			
		// Whether this entity has a component type
		HasCompBitMask hasComp;

		// TODO (Vincent): Can an entity not contain another entity?
		//Entity* parent;


	protected:
		// Constructors usable by Scene
		Entity(i32 _id);
		Entity(i32 _id, String _name);


	public:
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

		// Gets the component with the type provided. If no such comp -> through error and return
		IComponent * getComp(ComponentType id);
	};
}
