#include <MeshNode2.h>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/euler_angles.hpp>

MeshNode2::MeshNode2(){ }

MeshNode2::~MeshNode2(){ }

// The idle transformation
glm::mat4 MeshNode2::getTransformation()
{
	return transformation;
}

// Frame based transformation
glm::mat4 MeshNode2::getTransformation(u32 frameIndex, u32 animationIndex)
{
	if (!hasAnimations())
		return getTransformation();

	// Frame based way to update transformation
	return nodeAnimTransformation[animationIndex][frameIndex];
}

// Linear interpolated transformation
glm::mat4 MeshNode2::getTransformation(u32 frameIndex, u32 animationIndex, time_type accumulator, time_type timeStep, i32 whichTickFloor, i32 whichTickCeil)
{
	if (!hasAnimations())
		return getTransformation();

	glm::mat4 defaultTransformation(1);

	//if (getNumOfAnimations() < 1)
	//{
	//	getTransformation();
	//}

	// Interpolate between two keyframe
	f32 ratio = (f32)(accumulator / timeStep);
	if (accumulator == 0 || timeStep == 0)
		ratio = 0;

	glm::mat4 matrixFloor = nodeAnimTransformation[animationIndex][whichTickFloor];
	glm::mat4 matrixCeil = nodeAnimTransformation[animationIndex][whichTickCeil];

	//// Interpolate Translation
	//glm::mat4 matrixFloorTranslation = nodeAnimTranslationMatrices[animationIndex][whichTickFloor];
	//glm::mat4 matrixCeilTranslation = nodeAnimTranslationMatrices[animationIndex][whichTickCeil];
	//glm::mat4 interpolatedTranslation(1);
	//for (u32 i = 0; i < 4; i++)
	//{
	//	for (u32 j = 0; j < 4; j++)
	//	{
	//		interpolatedTranslation[i][j] = ((matrixFloorTranslation[i][j] * (1.0f - ratio))
	//			+ (matrixCeilTranslation[i][j] * (float)ratio));
	//	}
	//}

	//// Interpolate Rotation
	//glm::mat4 matrixFloorRotation = nodeAnimRotationMatrices[animationIndex][whichTickFloor];
	//glm::mat4 matrixCeilRotation= nodeAnimRotationMatrices[animationIndex][whichTickCeil];
	//glm::quat qFloor = glm::quat(matrixFloorRotation);
	//glm::quat qCeil = glm::quat(matrixCeilRotation);
	//glm::quat q = glm::slerp(qFloor, qCeil, ratio);
	//glm::mat4 interpolatedRotation = glm::mat4_cast(q);


	//// Interpolate Scaling
	//glm::mat4 matrixFloorScaling = nodeAnimScalingMatrices[animationIndex][whichTickFloor];
	//glm::mat4 matrixCeilScaling= nodeAnimScalingMatrices[animationIndex][whichTickCeil];
	//glm::mat4 interpolatedScale(1);
	//// Turns out glm does not support scale matrix interpolation, so we're doing our own
	//for(u32 i = 0; i < 4; i++)
	//{
	//	for (u32 j = 0; j < 4; j++)
	//	{
	//		interpolatedScale[i][j] = ((matrixFloorScaling[i][j] * (1.0f - ratio)) 
	//			+ (matrixCeilScaling[i][j] * (float)ratio));
	//	}
	//}

	// Linear interpolation
	return ((float)ratio * matrixCeil) + (matrixFloor * (1.0f - (float)ratio));
	//return interpolatedTranslation * interpolatedRotation * interpolatedScale;
}

// Idle global transformation
glm::mat4 MeshNode2::getGlobalTransformation()
{
	glm::mat4 transformatiom_temp(1);
	bool isRoot = this->parent == nullptr;
	if (isRoot)
	{
		return getTransformation();
	}

	transformatiom_temp = getTransformation() * transformatiom_temp;
	MeshNode2* currentNode = parent;
	while (!isRoot)
	{
		transformatiom_temp = currentNode->getTransformation() * transformatiom_temp;

		if (currentNode->parent == nullptr)
		{
			isRoot = true;
			break;
		}

		currentNode = currentNode->parent;
	}

	return transformatiom_temp;
}

// Frame based global transformation
glm::mat4 MeshNode2::getGlobalTransformation(u32 frameIndex, u32 animationIndex)
{
	glm::mat4 transformatiom_temp(1);
	bool isRoot = this->parent == nullptr;
	if (isRoot)
	{
		return getTransformation(frameIndex, animationIndex);
	}

	transformatiom_temp = getTransformation(frameIndex, animationIndex) * transformatiom_temp;
	MeshNode2* currentNode = parent;
	while (!isRoot)
	{
		transformatiom_temp = currentNode->getTransformation(frameIndex, animationIndex) * transformatiom_temp;

		if (currentNode->parent == nullptr)
		{
			isRoot = true;
			break;
		}

		currentNode = currentNode->parent;
	}

	return transformatiom_temp;
}

// Linear interpolated global transformation
glm::mat4 MeshNode2::getGlobalTransformation(u32 frameIndex, u32 animationIndex, time_type accumulator, time_type timeStep,
	i32 whichTickFloor, i32 whichTickCeil)
{
	glm::mat4 transformatiom_temp(1);
	bool isRoot = this->parent == nullptr;
	if (isRoot)
	{
		return getTransformation(frameIndex, animationIndex, accumulator, timeStep, whichTickFloor, whichTickCeil);
	}

	transformatiom_temp = getTransformation(frameIndex, animationIndex, accumulator, timeStep,
		whichTickFloor, whichTickCeil) * transformatiom_temp;
	MeshNode2* currentNode = parent;
	while (!isRoot)
	{
		transformatiom_temp = 
			currentNode->getTransformation(frameIndex, animationIndex, accumulator, timeStep, 
				whichTickFloor, whichTickCeil)
			* transformatiom_temp;

		if (currentNode->parent == nullptr)
		{
			isRoot = true;
			break;
		}

		currentNode = currentNode->parent;
	}

	return transformatiom_temp;
}

void MeshNode2::setupEulerAngle()
{
	for (u32 i = 0; i < nodeAnimTransformation.size(); i++)
	{
		eulerAngleXYZ[i].resize(nodeAnimTransformation[i].size());
		for (u32 j = 0; j < nodeAnimTransformation[i].size(); j++)
		{
			glm::mat4 rotationMatrix = nodeAnimRotationMatrices[i][j];
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

void MeshNode2::convertEulerAngleToMatrix()
{
	for (u32 i = 0; i < nodeAnimTransformation.size(); i++)
	{
		for (u32 j = 0; j < nodeAnimTransformation[i].size(); j++)
		{
			nodeAnimRotationMatrices[i][j] = glm::eulerAngleXYZ(eulerAngleXYZ[i][j].x, 
				eulerAngleXYZ[i][j].y, eulerAngleXYZ[i][j].z);
		}
	}
}

void MeshNode2::updateTransformation()
{
	convertEulerAngleToMatrix();

	for (u32 i = 0; i < nodeAnimTransformation.size(); i++)
	{
		for (u32 j = 0; j < nodeAnimTransformation[i].size(); j++)
		{
			nodeAnimTransformation[i][j] = nodeAnimTranslationMatrices[i][j]
				* nodeAnimRotationMatrices[i][j] * nodeAnimScalingMatrices[i][j];
		}
	}
}

void MeshNode2::updateMaximumFrame(u32 animationIndex, u32 i)
{
	if (hasAnimations())
	{
		maximumFrame[animationIndex] = i;
	}
	else
	{
		maximumFrame.resize(animationIndex + 1, 0);
		nodeAnimTransformation.resize(i);
		nodeAnimTranslationMatrices.resize(i);
		nodeAnimRotationMatrices.resize(i);
		nodeAnimScalingMatrices.resize(i);
		eulerAngleXYZ.resize(i);

		maximumFrame[animationIndex] = i;
	}
}

void MeshNode2::updateAnimationSize(u32 animationIndex, u32 num)
{
	if (hasAnimations())
	{
		u32 currentSize = (u32)nodeAnimTransformation[animationIndex].size();
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

				glm::mat4 lastFrameTransformation = nodeAnimTransformation[animationIndex][currentSize - 1];
				glm::mat4 lastFrameTranslation = nodeAnimTranslationMatrices[animationIndex][currentSize - 1];
				glm::mat4 lastFrameRotation = nodeAnimRotationMatrices[animationIndex][currentSize - 1];
				glm::mat4 lastFrameScaling = nodeAnimScalingMatrices[animationIndex][currentSize - 1];

				nodeAnimTransformation[animationIndex].resize(num);
				nodeAnimTranslationMatrices[animationIndex].resize(num);
				nodeAnimRotationMatrices[animationIndex].resize(num);
				nodeAnimScalingMatrices[animationIndex].resize(num);
				eulerAngleXYZ[animationIndex].resize(num);

				for (u32 i = currentSize; i < num; i++)
				{
					nodeAnimTransformation[animationIndex][i] = lastFrameTransformation;
					nodeAnimTranslationMatrices[animationIndex][i] = lastFrameTranslation;
					nodeAnimRotationMatrices[animationIndex][i] = lastFrameRotation;
					nodeAnimScalingMatrices[animationIndex][i] = lastFrameScaling;
				}

				setupEulerAngle();
			}
		}
	}
	else
	{
		updateMaximumFrame(animationIndex, num);

		nodeAnimTransformation[animationIndex].resize(num);
		nodeAnimTranslationMatrices[animationIndex].resize(num);
		nodeAnimRotationMatrices[animationIndex].resize(num);
		nodeAnimScalingMatrices[animationIndex].resize(num);
		eulerAngleXYZ[animationIndex].resize(num);

		for (u32 i = 0; i < num; i++)
		{
			nodeAnimTransformation[animationIndex][i] = getTransformation();
			nodeAnimTranslationMatrices[animationIndex][i] = getTransformation();
			nodeAnimRotationMatrices[animationIndex][i] = getTransformation();
			nodeAnimScalingMatrices[animationIndex][i] = getTransformation();
		}

		setupEulerAngle();
	}
}

void MeshNode2::insertFrameAfter(u32 animationIndex, u32 selectedFrame)
{
	if (hasAnimations())
	{
		u32 animationSize = (u32)nodeAnimTransformation[animationIndex].size();
		updateMaximumFrame(animationIndex, animationSize + 1);

		glm::mat4 lastFrameTransformation = nodeAnimTransformation[animationIndex][selectedFrame];
		glm::mat4 lastFrameTranslation = nodeAnimTranslationMatrices[animationIndex][selectedFrame];
		glm::mat4 lastFrameRotation = nodeAnimRotationMatrices[animationIndex][selectedFrame];
		glm::mat4 lastFrameScaling = nodeAnimScalingMatrices[animationIndex][selectedFrame];

		nodeAnimTransformation[animationIndex].insert(
			(nodeAnimTransformation[animationIndex].begin() + (selectedFrame + 2))
			, lastFrameTransformation);
		nodeAnimTranslationMatrices[animationIndex].insert(
			(nodeAnimTranslationMatrices[animationIndex].begin() + (selectedFrame + 2))
			, lastFrameTranslation);
		nodeAnimRotationMatrices[animationIndex].insert(
			(nodeAnimRotationMatrices[animationIndex].begin() + (selectedFrame + 2))
			, lastFrameRotation);
		nodeAnimScalingMatrices[animationIndex].insert(
			(nodeAnimScalingMatrices[animationIndex].begin() + (selectedFrame + 2))
			, lastFrameScaling);

		setupEulerAngle();
	}
}

u32 MeshNode2::getNumOfAnimations()
{
	return (u32)nodeAnimTransformation.size();
}

bool MeshNode2::hasAnimations()
{
	return nodeAnimTransformation.size() > 0;
}

bool MeshNode2::hasMesh()
{
	return meshIndex.size() > 0;
}
