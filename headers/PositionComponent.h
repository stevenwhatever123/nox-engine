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

			PositionComponent(f32 newx = 0.0f, f32 newy = 0.0f, f32 newz = 0.0f);

			void displayUI() override;

	};
}

