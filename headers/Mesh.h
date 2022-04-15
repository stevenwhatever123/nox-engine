#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "MeshNode.h"
#include "IRenderable.h"

// Compiler is my biggest enemy
#define NOMINMAX
#include <assimp/scene.h>
#undef NOMINMAX

#include "Types.h"

namespace NoxEngine {

	class Mesh : public IRenderable {
		public:

			Mesh(const aiScene *mesh);
			Mesh();
			~Mesh();

			void resizeNumOfMeshes(unsigned int i);
			void resizeNumOfAnimations(unsigned int i);
			void createNodeHierarchy(aiNode* aiRootnode, MeshNode* rootNode);
			void importAnimationData(aiScene* scene);

			void getAllNodesReferences();

			void printAllNodes();
			void printAllMeshNodes();

			void prepForRenderer();

			inline i32 getNumOfVertices() { return (i32)vertices[0].size(); }
			inline i32 getNumOfTexCoord() { return (i32)texCoord[0].size(); }
			inline i32 getNumOfNormals()  { return (i32)normals[0].size(); }
			inline i32 getNumOfElements() { return (i32)faceIndices[0].size() / 3; }

			void getArrayOfVertices(std::vector<f32>* v);
			void getArrayOfTexCoord(std::vector<f32>* tC);
			void getArrayOfNormals(std::vector<f32>* n);
			void getArrayOfElements(std::vector<i32>* el);


			MeshNode nodeHierarchy;
			std::vector<MeshNode*> allNodes;

			std::vector<std::vector<glm::vec3>> vertices;
			std::vector<std::vector<glm::vec3>> normals;
			std::vector<std::vector<u32>> faceIndices;
			std::vector<std::vector<glm::vec2>> texCoord;

			std::vector<std::vector<aiMaterial>> materials;
			std::vector<std::vector<aiTexture>> textures;

			// Animation data
			std::vector<aiAnimation*> animations;
			std::vector<std::vector<aiNodeAnim*>> nodeAnimations;
			std::vector<std::vector<glm::mat4>> nodeAnimTransformation;
			std::vector<std::vector<glm::mat4>> nodeAnimTranslationMatrices;
			std::vector<std::vector<glm::mat4>> nodeAnimRotationMatrices;
			std::vector<std::vector<glm::mat4>> nodeAnimScalingMatrices;

			const char* getNormalTexture() { return normTexName; }
			const char* getAmbientTexture() { return ambTexName;}

			const char* ambTexName = "S:/Masters/COMP5530M Group Project/Work/code/NoxEngine/assets/meshes/textures/Terracotta_Tiles_002_Base_Color.jpg";
			const char* normTexName = "S:/Masters/COMP5530M Group Project/Work/code/NoxEngine/assets/meshes/textures/Terracotta_Tiles_002_Normal.jpg";



			
			
		private:
			void copyNodesWithMeshes(aiNode* node, MeshNode* targetParent);
			void loopAllNodes(MeshNode node, std::vector<MeshNode>& list);
	};

}
