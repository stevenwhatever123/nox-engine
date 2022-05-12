#include <Components/AnimationComponent.h>

using namespace NoxEngine;

AnimationComponent::AnimationComponent()
{
	//id = ComponentType::AnimationType;

	frameIndex = 0;
	animationIndex = 0;
	accumulator = 0.0f;
	timeStep = 0.0f;
	whichTickFloor = 0;
	whichTickCeil = 0;
	playAnimation = false;

	animationName.resize(1);
	animationName[0] = "animation";
	numTicks.resize(1);
	numTicks[0] = 1;
	animationDuration.resize(1);
	animationDuration[0] = 1;

	transformation.resize(1);
	transformation[0].resize(1);
	transformation[0][0] = mat4(1);

	translationMatrices.resize(1);
	translationMatrices[0].resize(1);
	translationMatrices[0][0] = mat4(1);

	eulerAngleXYZ.resize(1);
	eulerAngleXYZ[0].resize(1);
	eulerAngleXYZ[0][0] = vec3(0);

	rotationMatrices.resize(1);
	rotationMatrices[0].resize(1);
	rotationMatrices[0][0] = mat4(1);

	scalingMatrices.resize(1);
	scalingMatrices[0].resize(1);
	scalingMatrices[0][0] = mat4(1);

	maximumFrame.resize(1);
	maximumFrame[0] = 1;

	//node = nullptr;
}

AnimationComponent::AnimationComponent(const MeshScene& scene, MeshNode *node)
{
	//id = ComponentType::AnimationType;

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

	transformation.resize(node->nodeAnimTransformation.size());
	translationMatrices.resize(node->nodeAnimTranslationMatrices.size());
	eulerAngleXYZ.resize(node->eulerAngleXYZ.size());
	rotationMatrices.resize(node->nodeAnimRotationMatrices.size());
	scalingMatrices.resize(node->nodeAnimScalingMatrices.size());

	maximumFrame.resize(node->nodeAnimTransformation.size());

	for (u32 i = 0; i < scene.numTicks.size(); i++)
	{
		animationName[i] = scene.animations[i]->mName.C_Str();
	}

	for (u32 i = 0; i < node->nodeAnimations.size(); i++)
	{
		transformation[i].resize(node->nodeAnimTransformation[i].size());
		translationMatrices[i].resize(node->nodeAnimTranslationMatrices[i].size());
		eulerAngleXYZ[i].resize(node->eulerAngleXYZ[i].size());
		rotationMatrices[i].resize(node->nodeAnimRotationMatrices[i].size());
		scalingMatrices[i].resize(node->nodeAnimScalingMatrices[i].size());

		transformation[i] = node->nodeAnimTransformation[i];
		translationMatrices[i] = node->nodeAnimTranslationMatrices[i];
		eulerAngleXYZ[i] = node->eulerAngleXYZ[i];
		rotationMatrices[i] = node->nodeAnimRotationMatrices[i];
		scalingMatrices[i] = node->nodeAnimScalingMatrices[i];

		maximumFrame[i] = (u32)node->nodeAnimTransformation[i].size();
	}

	numTicks = scene.numTicks;
	animationDuration = scene.animationDuration;

	//this->node = node;
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
	u32 numOfTicks = maximumFrame[animationIndex];

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
	if (num <= (u32)numTicks[animationIndex])
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

	updateAnimationSize(animationIndex, num);
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

	insertFrameAfter(animationIndex, selectedFrame);
}

void AnimationComponent::setupEulerAngle()
{
	for (u32 i = 0; i < transformation.size(); i++)
	{
		eulerAngleXYZ[i].resize(transformation[i].size());
		for (u32 j = 0; j < transformation[i].size(); j++)
		{
			glm::mat4 rotationMatrix = rotationMatrices[i][j];
			//float rotation[3] = { atan2(rotationMatrix[2][1], rotationMatrix[2][2]),
			//					atan2(-rotationMatrix[2][0], std::sqrt(rotationMatrix[2][1] * rotationMatrix[2][1] + rotationMatrix[2][2] * rotationMatrix[2][2])),
			//					atan2(rotationMatrix[1][0], rotationMatrix[0][0]) };
			glm::vec3 rotation(0);
			glm::extractEulerAngleXYZ(rotationMatrix, rotation.x, rotation.y, rotation.z);

			eulerAngleXYZ[i][j].x = rotation.x;
			eulerAngleXYZ[i][j].y = rotation.y;
			eulerAngleXYZ[i][j].z = rotation.z;
		}
	}
}

void AnimationComponent::convertEulerAngleToMatrix()
{
	for (u32 i = 0; i < transformation.size(); i++)
	{
		for (u32 j = 0; j < transformation[i].size(); j++)
		{
			rotationMatrices[i][j] = glm::eulerAngleXYZ(eulerAngleXYZ[i][j].x,
				eulerAngleXYZ[i][j].y, eulerAngleXYZ[i][j].z);
		}
	}
}

void AnimationComponent::updateTransformation()
{
	convertEulerAngleToMatrix();

	for (u32 i = 0; i < transformation.size(); i++)
	{
		for (u32 j = 0; j < transformation[i].size(); j++)
		{
			transformation[i][j] = rotationMatrices[i][j] * scalingMatrices[i][j] * translationMatrices[i][j];
		}
	}
}

mat4 AnimationComponent::getFrameTransformation()
{
	return transformation[animationIndex][frameIndex];
}

mat4 AnimationComponent::getTransformation()
{
	if(editing)
		return transformation[animationIndex][frameIndex];

	// Interpolate between two keyframe
	f32 ratio = (f32)(accumulator / timeStep);
	if (accumulator == 0 || timeStep == 0)
		ratio = 0;

	glm::mat4 matrixFloor = transformation[animationIndex][whichTickFloor];
	glm::mat4 matrixCeil = transformation[animationIndex][whichTickCeil];

	// Linear interpolation
	return ((float)ratio * matrixCeil) + (matrixFloor * (1.0f - (float)ratio));
}

void AnimationComponent::updateMaximumFrame(u32 animationIndex, u32 i)
{
	maximumFrame.resize(animationIndex + 1, 0);
	transformation.resize(i);
	translationMatrices.resize(i);
	rotationMatrices.resize(i);
	scalingMatrices.resize(i);
	eulerAngleXYZ.resize(i);

	maximumFrame[animationIndex] = i;
}

void AnimationComponent::updateAnimationSize(u32 animationIndex, u32 num)
{
	u32 currentSize = (u32)transformation[animationIndex].size();
	if (num > 0)
	{
		if (num < currentSize)
		{
			updateMaximumFrame(animationIndex, num);
			setupEulerAngle();
			return;
		}
		else
		{
			updateMaximumFrame(animationIndex, num);

			glm::mat4 lastFrameTransformation = transformation[animationIndex][currentSize - 1];
			glm::mat4 lastFrameTranslation = translationMatrices[animationIndex][currentSize - 1];
			glm::mat4 lastFrameRotation = rotationMatrices[animationIndex][currentSize - 1];
			glm::mat4 lastFrameScaling = scalingMatrices[animationIndex][currentSize - 1];

			transformation[animationIndex].resize(num);
			translationMatrices[animationIndex].resize(num);
			rotationMatrices[animationIndex].resize(num);
			scalingMatrices[animationIndex].resize(num);
			eulerAngleXYZ[animationIndex].resize(num);

			for (u32 i = currentSize; i < num; i++)
			{
				transformation[animationIndex][i] = lastFrameTransformation;
				translationMatrices[animationIndex][i] = lastFrameTranslation;
				rotationMatrices[animationIndex][i] = lastFrameRotation;
				scalingMatrices[animationIndex][i] = lastFrameScaling;
			}

			setupEulerAngle();
		}
	}
}

void AnimationComponent::insertFrameAfter(u32 animationIndex, u32 selectedFrame)
{
	u32 animationSize = (u32)transformation[animationIndex].size();
	//updateMaximumFrame(animationIndex, animationSize + 1);
	maximumFrame[animationIndex] = animationSize + 1;

	glm::mat4 lastFrameTransformation = transformation[animationIndex][selectedFrame];
	glm::mat4 lastFrameTranslation = translationMatrices[animationIndex][selectedFrame];
	glm::mat4 lastFrameRotation = rotationMatrices[animationIndex][selectedFrame];
	glm::mat4 lastFrameScaling = scalingMatrices[animationIndex][selectedFrame];

	transformation[animationIndex].insert(
		(transformation[animationIndex].begin() + (selectedFrame + 1))
		, lastFrameTransformation);
	translationMatrices[animationIndex].insert(
		(translationMatrices[animationIndex].begin() + (selectedFrame + 1))
		, lastFrameTranslation);
	rotationMatrices[animationIndex].insert(
		(rotationMatrices[animationIndex].begin() + (selectedFrame + 1))
		, lastFrameRotation);
	scalingMatrices[animationIndex].insert(
		(scalingMatrices[animationIndex].begin() + (selectedFrame + 1))
		, lastFrameScaling);

	setupEulerAngle();
}
