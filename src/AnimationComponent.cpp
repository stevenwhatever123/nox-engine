#include <AnimationComponent.h>

using namespace NoxEngine;

AnimationComponent::AnimationComponent(const MeshScene& scene, u32 index)
{
	frameIndex = 0;
	animationIndex = 0;
	accumulator = 0;
	timeStep = 0;
	whichTickFloor = 0;
	whichTickCeil = 0;
	playAnimation = false;

	numTicks = scene.numTicks;
	animationDuration = scene.animationDuration;

	node = scene.allNodes[index];
}

// Frame based update function
void AnimationComponent::update()
{
	if (hasAnimations())
	{
		u32 numOfTicks = numTicks[animationIndex];
		if (frameIndex < numOfTicks - 1)
		{
			frameIndex++;
		}
	}
}

// Semi-fixed time based update function
void AnimationComponent::update(time_type dt)
{
	if (hasAnimations())
	{
		accumulator += dt;

		u32 numOfTicks = numTicks[animationIndex];
		if (frameIndex < numOfTicks - 1)
		{
			// Interpolate time into tick
			time_type duration = animationDuration[animationIndex];
			timeStep = duration / numOfTicks;


			while (accumulator >= timeStep)
			{
				if (frameIndex < numOfTicks - 1)
					frameIndex++;

				accumulator -= timeStep;
			}

			// Remaining accumulated time
			//const time_type alpha = accumulator / timeStep;
			//frameIndex = frameIndex * alpha + frameIndex * (1.0 - alpha);

			updateCeilAndFloor();
		}
	}
}

void AnimationComponent::updateCeilAndFloor()
{
	if (hasAnimations())
	{
		u32 numOfTicks = numTicks[animationIndex];

		if ((accumulator / timeStep) > 0)
		{
			whichTickFloor = frameIndex;
			if (frameIndex < (numOfTicks - 1))
			{
				whichTickCeil = frameIndex + 1;
			}
			else
			{
				whichTickCeil = frameIndex;
			}
		}
	}
}

void AnimationComponent::resetAnimation()
{
	frameIndex = 0;
	accumulator = 0;
	timeStep = 0;
	whichTickCeil = 0;
	whichTickFloor = 0;
}

u32 AnimationComponent::getNumOfAnimations() const
{
	return (u32)numTicks.size();
}

bool AnimationComponent::hasAnimations()
{
	return getNumOfAnimations() > 0;
}