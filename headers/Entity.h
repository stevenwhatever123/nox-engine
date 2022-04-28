/*
 * Entity
 * A container class. What the object of this class is defined by the combinantion of the components.
 * 
 * Note that an entity is merely a container of a list of components. This implies:
 * - An entity does not directly contain another entity (i.e. no entity hierarchy)
 * - An entity can contain at most 1 of each type of components. 
 *   In the case where multiple of the same type of component is desired (e.g. a collection of CardComponents),
 *   the user would have to create a new component (e.g. DeckComponent) and attach this to the entity instead.
 * 
 * Component IDs: 0 - unknown component (should not be. Smth went wrong)
 *				  1 - Position component (contains position of teh entity in the 3D scene) - bitmask 0000 0000 0000 0001 2^0
 *				  2 - Mesh component (contains geometry of the object. Used by Renderer) - bitmask 0000 0000 0000 0010 2^1
 *				  3 - Light component (contains the light properties of the entity. Used by Renderer) - bitmask 0000 0000 0000 0100 2^2
 *				  4 - Audio component (contains audio of the entity. Used by Audio) - bitmask 0000 0000 0000 1000 2^3
*/
#pragma once

#define ENTITY_NAME_MAX_LEN 256

#include <map>
#include <typeindex>

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
		char *name;


	public:
		// unique entity id
		i32 id; 

		// The components that make the entity
		// Note (Vincent): An entity can only contain one component of each type.
		//                 In the case where multiple of the same components is desired (e.g. multiple BoneComponents),
		//                 create a container component (e.g. SkeletonComponent) and attach it
		// https://stackoverflow.com/questions/20720360/ecs-can-an-entity-have-more-than-one-component-of-given-type
		// Map key reference: https://stackoverflow.com/questions/46968176/how-to-avoid-downcasting-in-entity-component-system-c
		std::map<std::type_index, IComponent*> components; 
			
		// Whether this entity has a component type
		// TODO (Vincent): Potentially switch to std::bitset or a bit field struct
		// bitset is used here: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
		//					    https://stackoverflow.com/questions/3998091/bit-field-vs-bitset
		// bit field appears to be better for flags: https://www.reddit.com/r/cpp/comments/nou1rt/shocked_by_the_size_of_stdbitset/
		HasCompBitMask hasComp;

		// bitmask for which components are enabled/disabled. 1 = enabled, 0 = disabled
		HasCompBitMask _isEnabled;

		// Is the entire entity enabled/disabled?
		bool entityEnabled;


	protected:
		// Constructors usable by Scene
		Entity(i32 _id, char* _name = nullptr);


	public:
		// Automatically give a unique ID and a placeholder name to an entity
		// TODO: Change GameManager to Singleton, so it doesn't need a param
		Entity(Scene *scene, char* _name = nullptr);
		Entity(Scene* scene, const char* _name);

		// Move constructor
		Entity(Entity&& other);

		// Gotta be careful. When comp are destroyed the subsystem have to know
		// TODO (Vincent): delete the components array and let the specialized component destroyer remove the reference in the subsystem?
		~Entity();

		// Check whether this entity has the specified components
		bool containsComps(HasCompBitMask mask);
		// base case: check for 1 component
		template <typename T> bool containsComp() {
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			return containsComps(1 << (tmp->id - 1));
		}
		template <typename T, typename... Types> bool containsComps() {
			return containsComp<T>() && containsComps<Types>();
		}

			
		// Add component(s) to the entity (call default constructors)
		void addComp(ComponentType type);

		// Add an instantiated component to the entity
		// hasComp is updated here
		template <typename T> void addComp(T* comp);

		// Base case: add 1 component
		template <typename T> void addComp() {
			T* comp = new T();
			addComp<T>(comp);
		}
		template <typename T, typename... Types> void addComps() {
			addComp<T>();			// add base case
			addComps<Types>();		// deal with the remaining cases
		}


		// Remove all specified components (even if it doesn't have the component type)
		//bool removeComps(HasCompBitMask mask);
		// base case: remove 1 component
		// Returns the number of components removed
		template <typename T> size_t removeComp() {
			// Update bitmask
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			hasComp &= ~(1 << (tmp->id - 1));

			// remove component
			return components.erase(typeid(T));
		}
		template <typename T, typename... Types> size_t removeComps() {
			return removeComp<T>() + removeComp<Types>();
		}


		// Gets the component with the type provided. If no such comp -> through error and return
		template <typename T> T *getComp();


		// Setter: entity-level enable
		inline void setEntityEnabled(bool aEnabled) { 
			entityEnabled = aEnabled;
		}
		// Enable/disable components in this entity
		void setEnabled(HasCompBitMask aEnabled) { _isEnabled = aEnabled; };
		// base case: enable 1
		template <typename T> void setEnabled(bool aEnabled) {
			// change enable-ness on entity level
			entityEnabled = aEnabled;

			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			HasCompBitMask mask = (1 << (tmp->id - 1));
			if (aEnabled) _isEnabled |= mask;
			else          _isEnabled &= ~mask;
		}
		template <typename T, typename Types> void setEnabled(bool aEnabled) {
			setEnabled<T>(aEnabled);		// base case
			setEnabled<Types>(aEnabled);	// remaining types
		}


		// Getter: is the entity enabled?
		inline bool isEntityEnabled() { return entityEnabled; }
		// Check whether a component is enabled
		bool isEnabled(u32 bit);
		template <typename T> bool isEnabled() {
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			HasCompBitMask maskSingleBit = (1 << (tmp->id - 1));
			return _isEnabled & maskSingleBit;
		}
	};
}
