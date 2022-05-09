#pragma once
#include "IComponent.h"
#include "IPosition.h"
#include <Core/Types.h>
#include <Core/Entity.h>

/*
* Position Component. Handles postion of the the entity in the 3D world
*/

namespace NoxEngine {

	class PositionComponent : public IPosition, public IComponent
	{
		public:
			PositionComponent(f32 newx = 0.0f, f32 newy = 0.0f, f32 newz = 0.0f);
			void displayUI() override;
			static void exportLua() { };

			f32 get_x() const override { return x; }
			f32 get_y() const override { return y; }
			f32 get_z() const override { return z; }

			void set_x(f32 value) override { x = value; }
			void set_y(f32 value) override { y = value; }
			void set_z(f32 value) override { z = value; }

	};


}

