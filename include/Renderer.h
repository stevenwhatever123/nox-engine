#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


#include"stb_image.h"
#include"stb_image_write.h"


#include "IRenderable.h"
#include "Camera.h"
#include "Shader.h"
#include "Singleton.h"

namespace NoxEngine {

	// The objects to render
	struct RendObj
	{
		IRenderable* objPtr; // A pointer to the object
		i32 startInd, endInd; // Start and end indixes in the a united element array 
		GLuint normalTexture, ambientTexture; // Texture handlers
	};


	/*
	 * A class that renders 3D using OpenGL
	 * */
	class Renderer : public Singleton<Renderer> {

		friend class Singleton<Renderer>;

		private:
		// The shaders
		Shader *shader;

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

		int numberOfVertices, numberOfNormals, numberOfElements,  numOfTexCoords;

		std::vector<GLfloat> vertices;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> texCoords;
		std::vector<GLint> elements;

		// Buffer and texture to render to.
		GLuint textureToRenderTo, tex;
		GLuint FBO;
		GLuint curFBO = 0;

		glm::vec3 color;

		// Create Arrays of data
		void createVertexArray(IRenderable* mesh);
		void createNormalsArray(IRenderable* mesh);
		void createTexCoordArray(IRenderable* mesh);
		void createElementArray(IRenderable* mesh);

		void setUpShader();

		GLuint setTexture(const char* texturePath, const char* uniName, int num);

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
		void addObject(IRenderable *mesh);

		void updateBuffers();


		// Draw functions
		void draw();
		void fillBackground(float r, float g, float b);



		// Get the texture the renderer rendered to
		GLuint getTexture() { return textureToRenderTo; }


		// Functions updating parts of the shaders

		//void updateLocalTransf(int frame_index);

		void updateProjection(int w, int h);

		inline void setFrameBufferToDefault() { curFBO = 0; }
		inline void setFrameBufferToTexture() { curFBO = FBO; }


		glm::mat4 getProjMatr() { return projection; }

		glm::mat4 getCameraMatr() { return camera->getCameraTransf(); }



		// Camera managment
		// Updates Camera with new camera
		void updateCamera(Camera* camera);

		// Updates the view transformation using the current camera
		void updateCamera();


	};
}
