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
	};
}
