#include <AnimationComponent.h>

using namespace NoxEngine;

AnimationComponent::AnimationComponent()
{
	id = ComponentType::AnimationType;

	frameIndex = 0;
	animationIndex = 0;
	accumulator = 0.0f;
	timeStep = 0.0f;
	whichTickFloor = 0;
	whichTickCeil = 0;
	playAnimation = false;

	animationName.resize(0);
	numTicks.resize(0);
	animationDuration.resize(0);

	node = nullptr;
}

AnimationComponent::AnimationComponent(const MeshScene& scene, MeshNode2 *node)
{
	id = ComponentType::AnimationType;

	frameIndex = 0;
	animationIndex = 0;
	accumulator = 0.0f;
	timeStep = 0.0f;
	whichTickFloor = 0;
	whichTickCeil = 0;
	playAnimation = false;

	//Some resizing
	animationName.resize(scene.numTicks.size());
	numTicks.resize(scene.numTicks.size());
	animationDuration.resize(scene.animationDuration.size());

	for (u32 i = 0; i < scene.numTicks.size(); i++)
	{
		animationName[i] = scene.animations[i]->mName.C_Str();
	}

	numTicks = scene.numTicks;
	animationDuration = scene.animationDuration;

	this->node = node;
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
	u32 numOfTicks = numTicks[animationIndex];

	//reset if finish playing
	if (frameIndex >= numOfTicks - 1)
	{
		resetAnimation();
		playAnimation = false;
	}

	if (playAnimation)
	{
		accumulator += dt;

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

	updateCeilAndFloor();
}

void AnimationComponent::setAnimationIndex(u32 num)
{
	if (num > getNumOfAnimations() - 1)
		return;

	if (num < 0)
		return;

	animationIndex = num;
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

void AnimationComponent::updateNumTicks(u32 animationIndex, u32 num)
{
	// Don't do anything if the number is the same
	if (num == numTicks[animationIndex])
		return;

	if (hasAnimations())
	{
		numTicks[animationIndex] = num;
	}
	else
	{
		numTicks.resize(animationIndex, 0);
		numTicks[animationIndex] = num;
	}

	node->updateAnimationSize(animationIndex, num);
}

void AnimationComponent::insertFrame(u32 animationIndex, u32 selectedFrame)
{
	if (hasAnimations())
	{
		numTicks[animationIndex] = numTicks[animationIndex] + 1;
	}
	else
	{
		numTicks.resize(animationIndex, 0);
		numTicks[animationIndex] = 1;
	}

	node->insertFrameAfter(animationIndex, selectedFrame);
}