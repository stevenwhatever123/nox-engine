#include <MeshNode2.h>

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
glm::mat4 MeshNode2::getTransformation(u32 frameIndex, u32 animationIndex, f32 accumulator, f32 timeStep,
	i32 whichTickFloor, i32 whichTickCeil)
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

	glm::mat4 matrixFloor = nodeAnimTransformation[animationIndex][whichTickFloor];
	glm::mat4 matrixCeil = nodeAnimTransformation[animationIndex][whichTickCeil];

	// Linear interpolation
	return (matrixCeil * (float)ratio) + ((1.0f - (float)ratio) * matrixFloor);
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
glm::mat4 MeshNode2::getGlobalTransformation(u32 frameIndex, u32 animationIndex, f32 accumulator, f32 timeStep,
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

u32 MeshNode2::getNumOfAnimations()
{
	return nodeAnimations.size();
}

bool MeshNode2::hasAnimations()
{
	return nodeAnimations.size() > 0;
}

bool MeshNode2::hasMesh()
{
	return meshIndex.size() > 0;
}