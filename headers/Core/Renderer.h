#pragma once

//System std lib include
#include <iostream>
#include <format>

// 3rd Party Include
#include <Core/Types.h>
// #include <glad/glad.h>

#include <3rdParty/stb/stb_image.h>
#include <3rdParty/stb/stb_image_write.h>

// Engine Include
#include <Core/Camera.h>
#include <Core/GLProgram.h>
#include <Core/Entity.h>

#include <Managers/Singleton.h>

#include <Components/ComponentType.h>
#include <Components/IRenderable.h>
#include <Components/ITransform.h>
#include <Managers/IReloadableFile.h>


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
		f32 lineWidth = 1.0f;
		//mat4 pos;
		mat4 transformation;

		String ambientTexturePath;
		String normalTexturePath;

		// Component type of the meshSrc
		ComponentType componentType;
	};
	
	extern GLenum GLRenderTypes[3];

	// keep this insync with the IRenderable one, a map would be overkill

	/*
	 * A class that renders 3D using OpenGL
	 * */
	class Renderer : public Singleton<Renderer>, public IReloadableFile {
		friend class Singleton<Renderer>;

		public:

		Renderer(i32 width, i32 height, Camera* camera);
		~Renderer();

		// Add object to renderer to render
		void addObject(Entity* ent, IRenderable* meshSrc, ComponentType componentType);
		void addPermObject(IRenderable *render, ITransform *trans = nullptr);
		void removeObject(Entity* ent, ComponentType componentType);
		void removeObject(u32 rendObjId);
		void clearObject();

		void addLights(Entity *ent);
		
		// Program handle
		inline void setProgram(GLProgram *programIncome) { program = programIncome;}
		// If the program has been changed -> call this function to update all the variables set up for the program
		void updateProgram();
		void updateBuffers();

		// Draw functions
		void draw();
		void fillBackground(f32 r, f32 g, f32 b);
		void fillBackground(i32 hex);

		// Get the texture of the framebuffer the renderer uses
		inline GLuint getTexture() { return textureToRenderTo; }
		inline GLuint getDepthTexture() { return depthStencilTexture; }
		void updateTextureSizes(u32 width, u32 height);

		// Functions updating parts of the shaders

		void updateProjection(i32 w, i32 h);

		inline void setFrameBufferToDefault() { curFBO = 0; setRenderTarget(); }
		inline void setFrameBufferToTexture() { curFBO = FBO; setRenderTarget(); }
		inline void setRenderTarget() { glBindFramebuffer(GL_FRAMEBUFFER, curFBO); }

		inline mat4 getProjMatr() { return projection; }
		inline mat4 getCameraMatr() { return camera->getCameraTransf(); }
		inline mat4 getCameraView() { return cam; };

		inline void setCamera(Camera *cam) { camera = cam; };
		inline Camera* getCamera() { return camera; };

		// Camera managment
		// Updates Camera with new camera
		void updateCamera(Camera* camera);

		// Updates the view transformation using the current camera
		void updateCamera();

		void _change_num_of_light(u32 num) { program->changeLightNum(num); }

		inline const Map<u32, RendObj>& getObjects() const { return objects; };

		void updateObjectTransformation(glm::mat4 transformation, u32 rendObjId);
		void changeTexture(Entity *ent);
		bool hasRendObj(u32 id);

		void setSkyboxImage(const String& file_path, u32 skyboxPosition);
		const char* getSkyboxImagePath(u32 skyboxPosition);
		void setSkyBoxImages(const Array<String> &skyboxImages);
		void drawSkyBox();
		void liveReloadFile(const char *file, LiveReloadEntry *entry);

		i32 getWidth() { return w; };
		i32 getHeight() { return h; };

		private:

		GLProgram *program;

		u32 cubemapTexture;
		u32 skyVAO;
		u32 skyVBO;

		// skybox images
		Array<String> skyboxImages;

		u32 w;
		u32 h; // Width and Height of the window/texture to render to
		mat4 projection; // The matrix used for projection
		mat4 cam; // The matrix used for camera

		// The cur camera
		Camera* camera;
		Map<u32, RendObj> objects;
		Array<RendObj> perm_objects;

		Array<Entity* > lightSources;

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
		GLuint depthStencilTexture;
		GLuint tex;
		GLuint curFBO;

		vec3 color;

		// Create Arrays of data
		void createVertexArray(IRenderable* mesh);
		void createNormalsArray(IRenderable* mesh);
		void createTexCoordArray(IRenderable* mesh);
		void createElementArray(IRenderable* mesh);

		RendObj createRendObject(IRenderable *mesh);

		// This atribute is needed for Normal Mapping. 
		// Basically, need to transform the normals in the map into tangent space (space of the primitive (triangle))
		// To do so:
		//  - callculate tangents to vertices and submit them in the shader
		//  - in shader create transformation matrices using them. 
		// More in detail in the report section on Normal Mapping
		void createTangents(IRenderable* mesh); 
		GLuint setTexture(const String texturePath, const char* uniName, i32 num);

		void setupSkybox();
		void skyboxLoadTexture(); 

		u32 nextObjectId;

		public:
			
		void _change_num_of_lights_remove_this(u32 num) { program->changeLightNum(num); updateProgram(); }

		void updateLightPos(u32 lightInd);
		void updateLightMaterial(u32 lightInd);
		u32 getNumLights() { return lightSources.size(); }
	};
}
