/*
 * An interface: comunication with Renderer system of the engine.
 * Allows a class that implements this interface to be rendered by Renderer
 */
#pragma once

#include <Core/Types.h>

namespace NoxEngine {

	class ITransform
	{
		public:
			f32 x;
			f32 y;
			f32 z;

			f32 rx;
			f32 ry;
			f32 rz;
			
			f32 sx;
			f32 sy;
			f32 sz;

			virtual f32 get_x() const { return x; }
			virtual f32 get_y() const { return y; }
			virtual f32 get_z() const { return z; }
			virtual void set_x(f32 value) { x = value; }
			virtual void set_y(f32 value) { y = value; }
			virtual void set_z(f32 value) { z = value; }

			virtual f32 get_rx() const { return rx; }
			virtual f32 get_ry() const { return ry; }
			virtual f32 get_rz() const { return rz; }
			virtual void set_rx(f32 value) { rx = value; }
			virtual void set_ry(f32 value) { ry = value; }
			virtual void set_rz(f32 value) { rz = value; }

			virtual f32 get_sx() const { return sx; }
			virtual f32 get_sy() const { return sy; }
			virtual f32 get_sz() const { return sz; }
			virtual void set_sx(f32 value) { sx = value; }
			virtual void set_sy(f32 value) { sy = value; }
			virtual void set_sz(f32 value) { sz = value; }

	};
}
