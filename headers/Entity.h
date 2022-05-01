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
 * 
 * Vincent: Variadic template pattern referenced from https://stackoverflow.com/questions/31326458/recursive-variadic-template-cant-deduce-argument
*/
#pragma once

#define ENTITY_NAME_MAX_LEN 256

#include <map>
#include <typeindex>

// Engine Includes
#include <Types.h>
#include <Utils.h>
#include <ComponentType.h>

#include <EventManager.h>
#include <EventNames.h>


namespace NoxEngine {

	// Forward declares
	class IComponent;
	class Scene;

	class Entity
	{
		// make Scene a friend so that Entities can only be created in Scene
		friend class Scene;

	private:
		// recursive variadic template witchery
		template <typename...> struct sequence { };

		// Signals for event manager
		template <typename T> void addCompSignal()	  { SIGNAL_EVENT(EventNames::componentAdded,   this, std::type_index(typeid(T))); }
		template <typename T> void removeCompSignal() { SIGNAL_EVENT(EventNames::componentRemoved, this, std::type_index(typeid(T))); }


	public:
		// unique entity id
		i32 id;

		// A human-readable identifier - not necessarily unique
		char* name;

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

		// Flag for removal
		bool remove;


	protected:
		// Constructors usable by Scene
		Entity(i32 _id, char* _name = nullptr);

		// Move constructor
		Entity(Entity&& other);


	public:
		// Automatically give a unique ID and a placeholder name to an entity
		// TODO: Change GameManager to Singleton, so it doesn't need a param
		Entity(Scene *scene, char* _name = nullptr);
		Entity(Scene* scene, const char* _name);

		// Gotta be careful. When comp are destroyed the subsystem have to know
		// TODO (Vincent): delete the components array and let the specialized component destroyer remove the reference in the subsystem?
		~Entity();

		// Check whether this entity has the specified components
		bool containsComps(HasCompBitMask mask);
		// 0 component base case
		bool containsComps__helper(sequence<>) {
			return true;
		}
		template <typename T, typename... U> bool containsComps__helper(sequence<T, U...>) {
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			return containsComps(1 << (tmp->id - 1)) && containsComps__helper(sequence<U...>{});
		}
		// func signature to call
		template <typename... T> bool containsComps() { return containsComps__helper(sequence<T...>{}); }

			
		// Add component(s) to the entity (call default constructors)
		void addComp(ComponentType type);

		// Add an instantiated component to the entity
		// hasComp is updated here
		template <typename T> void addComp(T* comp);


		// 0 component base case
		void addComps__helper(sequence<>) {}
		template <typename T, typename... U> void addComps__helper(sequence<T, U...>) {
			T* comp = new T();
			addComp<T>(comp);

			// remaining types
			addComps__helper(sequence<U...>{});
		}
		// func signature to call
		template <typename T>    void addComp()  { return addComps__helper(sequence<T>{}); }
		template <typename... T> void addComps() { return addComps__helper(sequence<T...>{}); }


		// Remove all specified components (even if it doesn't have the component type)
		//bool removeComps(HasCompBitMask mask);
		// base case: remove 1 component
		// Returns the number of components removed

		// 0 component base case
		size_t removeComps__helper(sequence<>) { return 0; }
		template <typename T, typename... U> size_t removeComps__helper(sequence<T, U...>) {
			// Update bitmask
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			hasComp &= ~(1 << (tmp->id - 1));

			removeCompSignal<T>();

			// remove component
			return components.erase(typeid(T)) + removeComps__helper(sequence<U...>{});
		}
		// func signature to call
		template <typename T>    size_t removeComp()  { return removeComps__helper(sequence<T>{}); }
		template <typename... T> size_t removeComps() { return removeComps__helper(sequence<T...>{}); }


		// Gets the component with the type provided. If no such comp -> through error and return
		template <typename T> T *getComp();


		// Setter: entity-level enable
		inline void setEntityEnabled(bool aEnabled) { entityEnabled = aEnabled; }
		// Enable/disable components in this entity
		void setEnabled(HasCompBitMask aEnabled) { _isEnabled = aEnabled; };

		// 0 component base case
		void setEnabled__helper(bool aEnabled, sequence<>) {}
		template <typename T, typename... U> void setEnabled__helper(bool aEnabled, sequence<T, U...>) {
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			HasCompBitMask mask = (1 << (tmp->id - 1));
			if (aEnabled) _isEnabled |= mask;
			else          _isEnabled &= ~mask;

			// remaining types
			setEnabled__helper(aEnabled, sequence<U...>{});	
		}
		// func signature to call
		template <typename... T> void setEnabled(bool aEnabled) { return setEnabled__helper(aEnabled, sequence<T...>{}); }


		// Getter: is the entity enabled?
		inline bool isEntityEnabled() { return entityEnabled; }
		// Check whether a component is enabled. This does not take entity-level enabled-ness into account.
		bool isEnabled(u32 bit);
		template <typename T> bool isEnabled() {
			// TODO (Vincent): Fix w/ auto component id
			T* tmp = new T();
			return isEnabled(tmp->id);
		}
			static void exportLua();
			i32 get_id() { return id; }
			i32 set_id(i32 value) { id = value; }


	};
}
