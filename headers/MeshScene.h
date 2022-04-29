#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <MeshNode2.h>
#include <Mesh2.h>

// Compiler is my biggest enemy
#define NOMINMAX
#include <assimp/scene.h>
#undef NOMINMAX

#include <Types.h>
#include <Utils.h>

namespace NoxEngine {

	class MeshScene {
	public:

		MeshScene(const aiScene* mesh);
		MeshScene();
		~MeshScene();

		void printAllNodes();
		void printAllMeshNodes();

		void update();
		void update(time_type dt);

		void updateCeilAndFloor();

		void flipUV();
		void setAnimationIndex(u32 num);

		// Reset the play time
		void resetAnimation();

		u32 getNumOfAnimations();
		bool hasAnimations();
		
		void updateNumTicks(u32 animationIndex, u32 num);
		void insertFrame(u32 animationIndex, u32 selectedFrame);
		void setFrameIndex(u32 index);

		// This is the root of the node nodeHierarchy
		// It contains all the child nodes
		MeshNode2 nodeHierarchy;
		// A reference to all the nodes
		std::vector<MeshNode2*> allNodes;
		// List of mesh objects
		std::vector<Mesh2*> meshes;

		u32 frameIndex;
		u32 animationIndex;

		time_type accumulator;
		time_type timeStep;

		i32 whichTickFloor;
		i32 whichTickCeil;

		// Animation data
		Array<aiAnimation*> animations;

		// Animation clip -> numTicks
		Array<i32> numTicks;
		// Animation clip -> duration
		Array<time_type> animationDuration;

		bool playAnimation = false;

	private:
		void extractGeometricInfo(const aiScene* scene);
		void extractTextureInfo(const aiScene* scene, Mesh2 *mesh);
		void extractAnimationInfo(const aiScene* scene);

		void createNodeHierarchy(aiNode* aiRootnode, MeshNode2* rootNode);
		void traverseTreeStructure(aiNode* node, MeshNode2* targetParent);
		void searchThroughTree(MeshNode2* targetNode);

		// Just some function for printing
		void loopAllNodes(MeshNode2 node, std::vector<MeshNode2>& list);
	};
}
