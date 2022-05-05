#pragma once
#include <Types.h>
#include <MeshNode2.h>

namespace NoxEngine
{
	class IAnimation
	{
	public:
		u32 frameIndex;
		u32 animationIndex;

		time_type accumulator;
		time_type timeStep;

		i32 whichTickFloor;
		i32 whichTickCeil;

		Array<String> animationName;

		// Animation clip -> numTicks
		Array<i32> numTicks;
		// Animation clip -> duration
		Array<time_type> animationDuration;

		bool playAnimation = false;

		// The associated node
		MeshNode2 *node;
	};
}