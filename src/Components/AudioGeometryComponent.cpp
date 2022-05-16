#include <Components/AudioGeometryComponent.h>

#include "Core/MeshScene.h"

#include <algorithm>
#include <iterator>

using namespace NoxEngine;


AudioGeometryComponent::AudioGeometryComponent() {

	//id = ComponentType::AudioGeometryType;

	has_texture = 0;
	has_normal = 0;
	use_indices = 1;

	geometryId = -1;
	render = false;

	shape = Shape::Undefined;
	glRenderType = GL_LINES;

	v1 = vec3(0);
	v2 = vec3(0);
	v3 = vec3(0);
	v4 = vec3(0);

	meshScene = nullptr;
}

void AudioGeometryComponent::clearMesh() {
	vertices.clear();
	faces.clear();
	indices.clear();
}

bool AudioGeometryComponent::loadMesh(MeshScene* meshScene) {

	clearMesh();

	// Store a reference to the MeshScene
	this->meshScene = meshScene;

	// Copy all vertices over, reuse faces and indices
	for (MeshNode *node : meshScene->allNodes) {

		// No mesh in this node, skip
		if (node->meshIndex.size() <= 0) continue;
		
		// Apply the first frame transformation to the vertices (one by one... quite bad)
		const Mesh* mesh = meshScene->meshes[node->meshIndex[0]];
		glm::mat4 transformation;
		if (node->hasAnimations()) {
			transformation = node->getGlobalTransformation(
				meshScene->frameIndex, meshScene->animationIndex,
				meshScene->accumulator, meshScene->timeStep,
				meshScene->whichTickFloor, meshScene->whichTickCeil);
		}
		else {
			transformation = node->getTransformation();
		}
		

		// Deep copy over vertices and apply transformation
		//u32 startIdx = vertices.size();
		//Array<vec3> newVertices = mesh->getVertices();
		//copy(newVertices.begin(), newVertices.end(), back_inserter(vertices));
		//for (; startIdx < vertices.size(); startIdx++) {
		//	vertices[startIdx] = transformation * vec4(vertices[startIdx], 1.0f);
		//}

		for (const vec3 vertex : mesh->getVertices()) {
			vertices.push_back( transformation * vec4(vertex, 1.0f) );
		}

		// Reuse indices
		faces.insert(faces.end(), mesh->getFaces().begin(), mesh->getFaces().end());
		indices.insert(indices.end(), mesh->getIndices().begin(), mesh->getIndices().end());
	
		// Indicate there's no animation for this mesh node, and use identity transformation
		node->nodeAnimTransformation.clear();
		node->transformation = mat4(1.0f);
	}

	// Indices are never present using our FBX loader as of (2022-May-09), so we convert face indices to line indices here
	if (indices.size() == 0) {
		return generateIndicesFromTriangles(faces);
	}

	return true;
}

bool AudioGeometryComponent::generateBoundingBox(const Array<vec3>& verts, mat4 transformation) {

	// no vertices - cannot generate bounding box
	if (verts.size() == 0) return false;

	clearMesh();

	// find the min/max x,y,z respectively (more efficient to find both min and max in 1 pass)
	vec3 l = verts[0], u = verts[0];
	for (const vec3 v : verts) {
		l.x = std::min(l.x, v.x);
		l.y = std::min(l.y, v.y);
		l.z = std::min(l.z, v.z);
		u.x = std::max(u.x, v.x);
		u.y = std::max(u.y, v.y);
		u.z = std::max(u.z, v.z);
	}

	// Given min/max vertices, generate the 6 other corners and generate faces / indices for lines as follows:
	//      6_______ 5
	//      /|     /| 
	//     / |    / |
	//   7/__|__4/__|1
	//    |  /2  |  /
	//    | /    | /
	//    |/_____|/
	//   3       0

	vertices = {
		{ u.x, l.y, l.z },
		{ u.x, l.y, u.z },
		{ l.x, l.y, u.z },
		l,					// min
		{ u.x, u.y, l.z },
		u,					// max
		{ l.x, u.y, u.z },
		{ l.x, u.y, l.z }
	};

	// scale vertices by transformation
	for (auto &vertex : vertices) {
		vertex = transformation * vec4(vertex, 1.0f);
	}

	faces = {
		{1,2,3},
		{7,6,5},
		{4,5,1},
		{5,6,2},
		{2,6,7},
		{0,3,7},
		{0,1,3},
		{4,7,5},
		{0,4,1},
		{1,5,2},
		{3,2,7},
		{4,0,7}
	};

	return generateIndicesFromTriangles(faces);
}

// Assume the 4 vertices is in CCW, and is planar
// TODO: planar test
// TODO: cross product to ensure CCW
bool AudioGeometryComponent::generatePlane(vec3 _v1, vec3 _v2, vec3 _v3, vec3 _v4, mat4 transformation) {

	clearMesh();

	vertices = { _v1, _v2, _v3, _v4 };
	faces = {
		{0, 1, 2},
		{0, 2, 3}
	};

	return generateIndicesFromTriangles(faces);
}



bool AudioGeometryComponent::generateIndicesFromTriangles(const Array<ivec3>& _faces) {

	u32 nExistingIndices = (u32)indices.size();
	indices.resize(indices.size() + _faces.size() * 6);

	for (u32 f = 0; f < _faces.size(); f++) {
		ivec3 face = _faces[f];
		u32 start = nExistingIndices + f * 6;

		// indices form 3 lines: (0,1), (1,2), (2,0)
		indices[start + 0] = indices[start + 5] = face[0];
		indices[start + 1] = indices[start + 2] = face[1];
		indices[start + 3] = indices[start + 4] = face[2];
	}

	return true;
}
