#pragma once

#include "IComponent.h"
#include "ITransform.h"
#include "Core/Types.h"

/*
* Position Component. Handles postion of the the entity in the 3D world
*/

namespace NoxEngine {

	class TransformComponent : public ITransform, public IComponent
	{
		public:

			TransformComponent(f32 newx = 0.0f, f32 newy = 0.0f, f32 newz = 0.0f);

			static void exportLua() { };

			virtual Entity* getParentEntity() override { return parent; };
			virtual void attachedToEntity(Entity* ent) override { parent = ent; };

			f32 get_x() const override { return x; }
			f32 get_y() const override { return y; }
			f32 get_z() const override { return z; }

			void set_x(f32 value) override { x = value; }
			void set_y(f32 value) override { y = value; }
			void set_z(f32 value) override { z = value; }

			f32 get_rx() const override { return rx; }
			f32 get_ry() const override { return ry; }
			f32 get_rz() const override { return rz; }

			void set_rx(f32 value) override { rx = value; }
			void set_ry(f32 value) override { ry = value; }
			void set_rz(f32 value) override { rz = value; }

			f32 get_sx() const override { return sx; }
			f32 get_sy() const override { return sy; }
			f32 get_sz() const override { return sz; }

			void set_sx(f32 value) override { sx = value; }
			void set_sy(f32 value) override { sy = value; }
			void set_sz(f32 value) override { sz = value; }

	};
}

