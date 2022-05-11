#pragma once
#include <Core/Types.h>
#include <Core/MeshNode.h>

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
		//MeshNode2 *node;

		// A vector storing the maximum frame for every animation
		// AKA where the clip is supposed to end in which frame
		Array<u32> maximumFrame;

		// Animation clip -> keyframe transformation
		Array<Array<mat4>> transformation;
		Array<Array<mat4>> translationMatrices;

		Array<Array<glm::vec3>> eulerAngleXYZ;
		Array<Array<mat4>> rotationMatrices;
		Array<Array<mat4>> scalingMatrices;
	};
}