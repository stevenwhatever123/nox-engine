#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <MeshNode.h>
#include <IRenderable.h>
#include <assimp/scene.h>

#include <Types.h>
#include <Utils.h>
#include <ScriptsManager.h>

namespace NoxEngine {

	class Mesh : public IRenderable {
		public:

			Mesh(const aiScene *mesh);
			Mesh();
			~Mesh();

			static void exportLua() {
				auto lua_state = ScriptsManager::Instance()->get_lua_state();
				luaL_openlibs(lua_state);

				luabridge::getGlobalNamespace(lua_state).
					beginNamespace("game").
					beginClass<Mesh>("Mesh").
					addConstructor<void(*)(void)>().
					endClass().
					endNamespace();
			}
			void resizeNumOfMeshes(u32 i);
			void resizeNumOfAnimations(u32 i);

			void createNodeHierarchy(aiNode* aiRootnode, MeshNode* rootNode);
			void calculateCenterPosition();

			void printAllNodes();
			void printAllMeshNodes();

			void update();
			void update(time_type dt);

			void generateAnimation(vec3 targetPosition);
			void resetFrameIndex();

			void flipUV();

			void setAnimationIndex(u32 num);

			u32 getNumOfAnimations();
			mat4 getGlobalTransformation(MeshNode currentNode);
			mat4 getCurrentFrameNodeTransformation(MeshNode *node);

			u32 frameIndex;
			u32 animationIndex;

			time_type accumulator;
			time_type timeStep;

			i32 whichTickFloor;
			i32 whichTickCeil;

			bool hasBones;
			vec3 centerPosition;


			// The root node of the hierarchy, it contains all the child nodes
			MeshNode nodeHierarchy;
			// A reference to all the nodes so we don't have to traversal the tree every time
			Array<MeshNode*> allNodes;

			Array<Array<vec3>> vertices;
			Array<Array<vec3>> normals;
			Array<Array<ivec3>>  faceIndices;
			Array<Array<vec2>> texCoord;

			Array<Array<aiMaterial>> materials;
			Array<Array<aiTexture>> textures;

			// Animation data
			Array<aiAnimation*> animations;
			Array<Array<aiNodeAnim*>> nodeAnimations;

			// Animation clip -> node -> keyframe transformation
			Array<Array<Array<mat4>>> nodeAnimTransformation;
			Array<Array<Array<mat4>>> nodeAnimTranslationMatrices;
			Array<Array<Array<mat4>>> nodeAnimRotationMatrices;
			Array<Array<Array<mat4>>> nodeAnimScalingMatrices;

			// Animation clip -> node -> numTicks
			Array<unsigned int> numTicks;
			// Animation clip -> duration
			Array<time_type> animationDuration;


			// IRenderable funcs
			inline i32 getNumOfVertices() { return (i32)vertices[0].size(); }
			inline i32 getNumOfTexCoord() { return (i32)texCoord[0].size(); }
			inline i32 getNumOfNormals()  { return (i32)normals[0].size(); }
			inline i32 getNumOfFaces()    { return (i32)faceIndices[0].size(); }

			const Array<vec3>& getVertices () const;
			const Array<vec2>& getTexCoords() const;
			const Array<vec3>& getNormals  () const;
			const Array<ivec3>& getFaces   () const;
			const Array<i32>& getIndices   () const { return indices; };

			const String getNormalTexture() { return normTexName; }
			const String getAmbientTexture() { return ambTexName;}

			const char* ambTexName = "assets/meshes/textures/Terracotta_Tiles_002_Base_Color.jpg";
			const char* normTexName = "assets/meshes/textures/Terracotta_Tiles_002_Normal.jpg";

		private:
			void extractGeometricInfo(const aiScene* scene);
			void extractTextureInfo(const aiScene* scene);
			void extractAnimationInfo(const aiScene* scene);

			void traverseTreeStructure(aiNode* node, MeshNode* targetParent);
			void loopAllNodes(MeshNode node, Array<MeshNode>& list);
	};

}
