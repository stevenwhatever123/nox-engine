/*
 * An interface: comunication with Renderer system of the engine.
 * Allows a class that implements this interface to be rendered by Renderer
 */
#pragma once

#include <Types.h>

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
	};
}
