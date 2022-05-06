/*
 * An interface: comunication with Renderer system of the engine.
 * Allows a class that implements this interface to be rendered by Renderer
 */
#pragma once

#include <Types.h>

namespace NoxEngine {

	class IPosition
	{
		public:
			f32 x;
			f32 y;
			f32 z;

			virtual f32 get_x() const { return x; }
			virtual f32 get_y() const { return y; }
			virtual f32 get_z() const { return z; }
			virtual void set_x(f32 value) { x = value; }
			virtual void set_y(f32 value) { y = value; }
			virtual void set_z(f32 value) { z = value; }
	};
}
