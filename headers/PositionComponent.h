#pragma once

#include <IComponent.h>
#include <IPosition.h>
#include <Types.h>

/*
* Position Component. Handles postion of the the entity in the 3D world
*/

namespace NoxEngine {

	class PositionComponent : public IPosition, public IComponent
	{
		public:

			PositionComponent(f32 newx, f32 newy, f32 newz);
	};
}

