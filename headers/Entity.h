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
#include <iostream>

#include "IComponent.h"
//#include "RenderableComp.h"


class Entity
{
public:

	int ID; // The ID of the sp entity. Used to distiguish between them
	std::vector<IComponent*> components; // The components that make the entity
	uint16_t hasComp = 0; // defines what components does this entity have 

	Entity() {}

	// Gotta be careful. When comp are destroyed the subsystem have to know
	~Entity() {}

	// Add a component to the entity
	void addComp(IComponent *comp)
	{
		// Check that there is no comp with the same ID 
		for (auto c : components)
		{
			if (c->ID == comp->ID)
			{
				// If yes -> through error and do nothing
				std::cout << "The component " << comp->ID << " already exists in Entity " << ID << std::endl;
				return;
			}
		}

		// If no -> add
		components.push_back(comp);
		// And add to hasComp
		hasComp += pow(2, (comp->ID - 1));
		
	}


	// Gets the component with the ID provided. If no such comp -> through error and return
	IComponent * getComp(int askedID)
	{
		for (auto c : components)
		{
			if (c->ID == askedID)
			{
				return c;
			}
		}

		std::cout << "The component " << askedID << " does not exist in Entity " << ID << std::endl;
		return nullptr;
	}

	//IRenderable * getRendComp()
	//{
	//	for (auto c : components)
	//	{
	//		if (c->ID == 2)
	//		{
	//			return (IRenderable*)c;
	//		}
	//	}

	//	std::cout << "The component " << 2 << " does not exist in Entity " << ID << std::endl;
	//}

	//// Replace an existing component with new one.
	//// If there is no comp with the same id -> add
	//void replaceComp(IComponent comp)
	//{
	//	// Check if same-Id comp exists
	//	for (auto c : components)
	//	{
	//		if (c.ID == comp.ID)
	//		{
	//			// If yes -> through error and do nothing
	//			std::cout << "The component " << comp.ID << " already exists in Entity " << ID << std::endl;
	//			return;
	//		}
	//	}

	//	// If no -> add
	//	components.push_back(comp);
	//}

};