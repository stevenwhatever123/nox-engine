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
			f32 get_x() { return x; }
			f32 get_y() { return y; }
			f32 get_z() { return z; }
			void set_x(f32 value) { x = value; }
			void set_y(f32 value) { y = value; }
			void set_z(f32 value) { z = value; }
	};
}
