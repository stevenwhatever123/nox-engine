#pragma once

#include <vector>

#include "IPosition.h"
#include "IComponent.h"

/*
* Position Component. Handles postion of the the entity in the 3D world
*/
class PositionComp : public IPosition, public IComponent
{
public:

	void* CastType(const int castID)
	{
		switch (castID)
		{
		case 2:
			return static_cast<IPosition*>(this); break;
		case 1:
			return static_cast<IComponent*>(this); break;
		default:
			return nullptr;
		}
	}


	PositionComp(float newx, float newy, float newz)
	{
		ID = 1;
		x = newx; y = newy; x = newx;
	}

};