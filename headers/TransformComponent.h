#pragma once

#include "IComponent.h"
#include "ITransform.h"
#include "Types.h"

/*
* Position Component. Handles postion of the the entity in the 3D world
*/

namespace NoxEngine {

	class TransformComponent : public ITransform, public IComponent
	{
		public:

			TransformComponent(f32 newx = 0.0f, f32 newy = 0.0f, f32 newz = 0.0f);

			void displayUI() override;

	};
}

