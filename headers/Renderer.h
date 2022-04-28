#pragma once

//System std lib include
#include <iostream>

// 3rd Party Include
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_write.h>

// Engine Include
#include <IRenderable.h>
#include <Camera.h>
#include <GLProgram.h>
#include <Singleton.h>
#include <IPosition.h>

namespace NoxEngine {

	// The objects to render
	struct RendObj
	{
		IRenderable* objPtr; // A pointer to the object
		GLuint renderType;
		i32 has_texture;
		i32 has_normal;
		i32 startInd;
		i32 endInd; // Start and end indixes in the a united element array 
		GLuint normalTexture;
		GLuint ambientTexture; // Texture handlers
		glm::mat4 pos;

		glm::mat4 transformation;
	};
	
	extern GLenum GLRenderTypes[3];

	// keep this insync with the IRenderable one, a map would be overkill

	/*
	 * A class that renders 3D using OpenGL
	 * */
	class Renderer : public Singleton<Renderer> {



		friend class Singleton<Renderer>;

		private:
		// The shaders
		GLProgram *program;

		i32 w;
		i32 h; // Width and Height of the window/texture to render to
		glm::mat4 projection; // The matrix used for projection
		glm::mat4 cam; // The matrix used for camera

		// The cur camera
		Camera* camera;
		std::vector<RendObj> objects;


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

		glm::vec3 color;

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
		GLuint setTexture(const String texturePath, const char* uniName, int num);

		public:

		/*
		 * Constructor.
		 * param:
		 *        width - the width of the window to render to
		 *        height - the height of the window to render to
		 *        type - the type of promitive this renderer handles.
		 *        camera - a camera to render from
		 */
		Renderer(int width, int height, Camera* camera);

		~Renderer();

		// Add object to renderer to render
		void addObject(IRenderable *mesh, IPosition *pos);
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
		void updateProjection(int w, int h);

		inline void setFrameBufferToDefault() { curFBO = 0; setRenderTarget(); }
		inline void setFrameBufferToTexture() { curFBO = FBO; setRenderTarget(); }
		inline void setRenderTarget() { glBindFramebuffer(GL_FRAMEBUFFER, curFBO); }

		glm::mat4 getProjMatr() { return projection; }
		glm::mat4 getCameraMatr() { return camera->getCameraTransf(); }

		// Camera managment
		// Updates Camera with new camera
		void updateCamera(Camera* camera);

		// Updates the view transformation using the current camera
		void updateCamera();
		void updateLightPos(float x, float y, float z);

		void updateObjectTransformation(glm::mat4 transformation, IRenderable* pRenderable);
	};
}
