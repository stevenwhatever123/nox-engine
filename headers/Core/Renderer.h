#pragma once

#include <Core/Types.h>
#include <Components/ComponentType.h>
#include <Components/IRenderable.h>
#include <Components/IPosition.h>
#include <Managers/Singleton.h>

#include "Camera.h"
#include "GLProgram.h"
#include "Entity.h"

namespace NoxEngine {

	// The objects to render
	struct RendObj
	{
		Entity* ent;
		IRenderable* meshSrc;	// Mesh data source (can be IRenderable, or IAudioGeometry)
		u32 renderType;
		i32 has_texture;
		i32 has_normal;
		i32 startInd;
		i32 endInd; // Start and end indixes in the a united element array 
		u32 normalTexture;
		u32 ambientTexture; // Texture handlers
		//mat4 pos;
		mat4 transformation;

		// Probably a better idea to store the type of the meshSrc, e.g. type_index meshType;
		ComponentType componentType;
	};
	
	/*
	 * A class that renders 3D using OpenGL
	 * */
	class Renderer : public Singleton<Renderer> {
		friend class Singleton<Renderer>;

		
		public:
		/*
		 * Constructor.
		 * param:
		 *        width - the width of the window to render to
		 *        height - the height of the window to render to
		 *        type - the type of promitive this renderer handles.
		 *        camera - a camera to render from
		 */
		Renderer(i32 width, i32 height, Camera* camera);
		~Renderer();

		// Add object to renderer to render
		void addObject(Entity* ent, IRenderable* meshSrc, ComponentType componentType);
		void removeObject(Entity* ent, ComponentType componentType);
		void clearObject();
		
		inline void setProgram(GLProgram *programIncome) { program = programIncome;}
		void useProgram();
		void updateBuffers();

		// Draw functions
		void draw();
		void fillBackground(f32 r, f32 g, f32 b);
		void fillBackground(i32 hex);

		// Get the texture the renderer rendered to
		GLuint getTexture() { return textureToRenderTo; }


		// Functions updating parts of the shaders

		//void updateLocalTransf(int frame_index);
		void updateProjection(i32 w, i32 h);

		inline void setFrameBufferToDefault() { curFBO = 0; setRenderTarget(); }
		inline void setFrameBufferToTexture() { curFBO = FBO; setRenderTarget(); }
		inline void setRenderTarget() { glBindFramebuffer(GL_FRAMEBUFFER, curFBO); }

		inline mat4 getProjMatr() { return projection; }
		inline mat4 getCameraMatr() { return camera->getCameraTransf(); }

		// Camera managment
		// Updates Camera with new camera
		void updateCamera(Camera* camera);

		// Updates the view transformation using the current camera
		void updateCamera();
		void updateLightPos(f32 x, f32 y, f32 z);

		void updateObjectTransformation(glm::mat4 transformation, u32 rendObjId);

		private:
		// The shaders
		GLProgram *program;

		i32 w;
		i32 h; // Width and Height of the window/texture to render to
		mat4 projection; // The matrix used for projection
		mat4 cam; // The matrix used for camera

		// The cur camera
		Camera* camera;
		std::map<u32, RendObj> objects;


		// Global buffers of attributes
		GLuint VBO;
		GLuint NBO;
		GLuint TCBO;
		GLuint VAO;
		GLuint EBO;
		GLuint TANBO;
		GLuint FBO;

		i32 numberOfVertices;
		i32 numberOfNormals;
		i32 numberOfElements;
		i32 numOfTexCoords;
		i32 numOfTangents;

		Array<vec3> vertices;
		Array<vec3> normals;
		Array<vec2> texCoords;
		Array<vec3> tangents;
		Array<i32> elements;

		// Buffer and texture to render to.
		GLuint textureToRenderTo;
		GLuint tex;
		GLuint curFBO;

		vec3 color;

		// Create Arrays of data
		void createVertexArray(IRenderable* mesh);
		void createNormalsArray(IRenderable* mesh);
		void createTexCoordArray(IRenderable* mesh);
		void createElementArray(IRenderable* mesh);

		// This atribute is needed for Normal Mapping. 
		// Basically, need to transform the normals in the map into tangent space (space of the primitive (triangle))
		// To do so:
		//  - callculate tangents to vertices and submit them in the shader
		//  - in shader create transformation matrices using them. 
		// More in detail in the report section on Normal Mapping
		void createTangents(IRenderable* mesh); 
		GLuint setTexture(const String texturePath, const char* uniName, i32 num);


	private:
		u32 nextObjectId;
	};

}
