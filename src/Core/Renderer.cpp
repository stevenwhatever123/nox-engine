#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <iterator>

#include <Core/Renderer.h>
#include <Utils/Utils.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AudioGeometryComponent.h>
#include <Managers/LiveReloadManager.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>


// TODO: update uniform submissions to use Shader class
// TODO: fix drawing to default buffer
// TODO: get some light going
// TODO: material class
// TODO(sharo): debug context and messages

using NoxEngineUtils::Logger;
using namespace NoxEngine;

Renderer::~Renderer()
{
    vertices.clear();
    normals.clear();
    texCoords.clear();
    elements.clear();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &TCBO);
    glDeleteBuffers(1, &EBO);

    // Remove shader
    // Remove framebuffer
    glDeleteFramebuffers(1, &FBO);
}

Renderer::Renderer(int width, int height, Camera* cam) :
	w(width),
	h(height),
	camera(cam),
	elements(0),
	projection(0),
	cam(0),
	objects(),
	VBO(0),
	NBO(0),
	TCBO(0),
	EBO(0),
	TANBO(0),
	FBO(0),
	numberOfVertices(0),
	numberOfNormals(0),
	numberOfElements(0),
	numOfTexCoords(0),
	numOfTangents(0),
	vertices(0),
	normals(0),
	texCoords(0),
	tangents(0),
	textureToRenderTo(0),
	tex(0),
	curFBO(0),
	color(0),
	nextObjectId(0)
{

	// Initialise OpenGl

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	// Create framebuffer  
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Gen texture for framebuffer    
    glGenTextures(1, &textureToRenderTo);
    glBindTexture(GL_TEXTURE_2D, textureToRenderTo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureToRenderTo, 0);

	glGenTextures(1, &depthStencilTexture);
	glBindTexture(GL_TEXTURE_2D, depthStencilTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG_DEBUG("Troubles with creating a framebuffer");
    }

	glBindTexture(GL_TEXTURE_2D, 0);

    // Generate buffer handlers
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TCBO);
    glGenBuffers(1, &TANBO);
    glGenBuffers(1, &EBO);

	setupSkybox();

}

void Renderer::updateBuffers() {

	program->use();
	program->printAttribInfo();
	// Bind vert attribute handle
	glBindVertexArray(VAO);

	// Update GPU containers
	
	// Vertice positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

	int positionAtr = program->getAtrributeLocation("position");
	glVertexAttribPointer(positionAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, positionAtr);

	// Normal positions
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);

	int normalAtr = program->getAtrributeLocation("normal");
	glVertexAttribPointer(normalAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, normalAtr);

	// Tex Coords positions
	glBindBuffer(GL_ARRAY_BUFFER, TCBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vec2), texCoords.data(), GL_STATIC_DRAW);

	int texCoordAtr = program->getAtrributeLocation("texCoord");
	glVertexAttribPointer(texCoordAtr, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, texCoordAtr);

    // Tangents
    glBindBuffer(GL_ARRAY_BUFFER, TANBO);
    glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(vec3), tangents.data(), GL_STATIC_DRAW);

    int tangentAtr = program->getAtrributeLocation("tangent");
    glVertexAttribPointer(tangentAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexArrayAttrib(VAO, tangentAtr);

    // Elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(i32), elements.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}


RendObj Renderer::createRendObject(IRenderable *mesh) {

	RendObj newObj;
	newObj.meshSrc = mesh;

	newObj.renderType = mesh->glRenderType;
	newObj.startInd = (i32)elements.size();
	newObj.has_texture = mesh->has_texture;
	newObj.has_normal = mesh->has_normal;
	newObj.lineWidth = mesh->lineWidth;

	// Generate textures for the object
	if(mesh->has_texture) {
		newObj.ambientTexture = setTexture(mesh->getAmbientTexture(), "AmbTexture", 1);
		newObj.normalTexture = setTexture(mesh->getNormalTexture(), "NormTexture", 2);
	}

	// Generate the arrays
	createVertexArray(mesh);

	if(mesh->has_texture) createTexCoordArray(mesh);
	if(mesh->has_normal) createNormalsArray(mesh);

	if(mesh->has_normal && mesh->has_texture) createTangents(mesh);

	createElementArray(mesh);

	newObj.endInd = i32(elements.size());
	newObj.transformation = mat4(1.0f);

	return newObj;
}

void Renderer::addObject(Entity *ent, IRenderable *meshSrc, ComponentType componentType) {
    // Add a mesh to the container
	RendObj newObj = createRendObject(meshSrc);
	newObj.ent = ent;
	newObj.componentType = componentType;

	objects[nextObjectId] = newObj;

	// give the IRenderable a reference to this rendObj
	meshSrc->rendObjId = nextObjectId++;
}

void Renderer::addPermObject(IRenderable *mesh, ITransform *trans)
{
    // Add a mesh to the container
    RendObj newObj = createRendObject(mesh);
	perm_objects.push_back(newObj);
}

void Renderer::removeObject(Entity* ent, ComponentType componentType) {

    // Note: This calls the destructor on RendObj
   // objects.erase(
   //     std::remove_if(objects.begin(), objects.end(), 
			//[ent, useAnimation](RendObj obj) { 
			//		return obj.ent == ent && obj.useAnimation == useAnimation; 
			//}),
   //     objects.end()
   // );

	auto itr = objects.begin();
	auto endItr = objects.end();
	for (; itr != endItr;) {
		if (itr->second.ent == ent && itr->second.componentType == componentType) objects.erase(itr);
		else itr++;
	}

    LOG_DEBUG("Renderer object count: %i\n", objects.size());
}

void Renderer::removeObject(u32 rendObjId) {

	objects.erase(rendObjId);
	LOG_DEBUG("Renderer object count: %i\n", objects.size());
}

GLuint Renderer::setTexture(const String texturePath, const char* uniName, int num) {

	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0 + num);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	else
	{
		LOG_DEBUG("Failed to load file %s ", texturePath.c_str());
	}
	stbi_image_free(data);

	GLuint textureLoc = program->getUniformLocation(uniName);
	glUniform1i(textureLoc, num);

	return tex;
}

void Renderer::clearObject()
{
	objects.clear();
}

void Renderer::draw() {

	// Render
	program->use();

	setFrameBufferToTexture();	
    glDepthFunc(GL_LESS);
	glBindVertexArray(VAO);

	for (u32 i = 0; i < perm_objects.size(); i++)
	{
		// If the object has a position and it's enabled, use it
		mat4 worldMat = mat4(1.0f);
		program->set4Matrix("toWorld", worldMat);
		program->set4Matrix("modelMatrix", perm_objects[i].transformation);

		// Activate and bind textures of the object
		if(perm_objects[i].has_texture) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, perm_objects[i].ambientTexture);
		}

		if(perm_objects[i].has_texture) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, perm_objects[i].normalTexture);
		}

		//if(perm_objects[i].renderType == GL_LINES) {
		//	glLineWidth(perm_objects[i].lineWidth);
		//}

		glDrawElements(perm_objects[i].renderType, (perm_objects[i].endInd - perm_objects[i].startInd), GL_UNSIGNED_INT, (void*)(perm_objects[i].startInd * sizeof(i32)));


		//glLineWidth(1.0f);
	}

	for (u32 i = 0; i < objects.size(); i++) {

		// We don't want to render something that has been removed or doesn't exist
		if (objects[i].ent == nullptr)
			continue;

		Entity* ent = objects[i].ent;

		// Skip if the entity is not enabled
		if (!ent->isEntityEnabled()) continue;

		// Renderable
		if (objects[i].componentType == ComponentType::RenderableType) {
			if (!ent->isEnabled<RenderableComponent>()) continue;
		}
		// AudioGeometry
		else if (objects[i].componentType == ComponentType::AudioGeometryType) {
			if (!ent->isEnabled<AudioGeometryComponent>() || !ent->getComp<AudioGeometryComponent>()->render) continue;
		}
		// If the object has a position and it's enabled, use it
		glm::mat4 worldMat = glm::mat4(1.0f);
		if (objects[i].ent->containsComps<TransformComponent>() && objects[i].ent->isEnabled<TransformComponent>()) {
			ITransform* pos = objects[i].ent->getComp<TransformComponent>()->CastType<ITransform>();
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos->x, pos->y, pos->z));
			glm::mat4 rotation = glm::eulerAngleXYZ(pos->rx, pos->ry, pos->rz);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(pos->sx, pos->sy, pos->sz));
			//worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(pos->x, pos->y, pos->z));
			worldMat = translation * rotation * scale;
		}

		program->set4Matrix("toWorld", worldMat);
        program->set4Matrix("modelMatrix", objects[i].transformation);

        // Activate and bind textures of the object
		if(objects[i].has_texture) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, objects[i].ambientTexture);
		} 

		if(objects[i].has_normal) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, objects[i].normalTexture);
		}

		// Draw the object
		glDrawElements(objects[i].renderType, (objects[i].endInd - objects[i].startInd), GL_UNSIGNED_INT, (void*)(objects[i].startInd * sizeof(i32)));
		// glDrawArrays(GL_TRIANGLES, 0, 3);
	}


	
	glBindVertexArray(0);
	setFrameBufferToDefault();
}

void Renderer::fillBackground(f32 r, f32 g, f32 b) {

	// Set background color
	
	program->use();

	f32 current_clear_color[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, current_clear_color);

	setFrameBufferToTexture();
	glClearColor(r,g,b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setFrameBufferToDefault();

	glClearColor(
			current_clear_color[0],
			current_clear_color[1],
			current_clear_color[2],
			current_clear_color[3] );

}

void Renderer::fillBackground(i32 hex) {

	// Set background color
	
	program->use();

	f32 current_clear_color[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, current_clear_color);

	f32 red =   (f32)((hex & 0xFF000000) >> 24)/255.0f;
	f32 green = (f32)((hex & 0x00FF0000) >> 16)/255.0f;
	f32 blue =  (f32)((hex & 0x0000FF00) >>  8)/255.0f;
	f32 alpha = (f32)((hex & 0x000000FF) >>  0)/255.0f;

	setFrameBufferToTexture();
	glClearColor(red , green , blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
	setFrameBufferToDefault();

	glClearColor(
			current_clear_color[0],
			current_clear_color[1],
			current_clear_color[2],
			current_clear_color[3] );

}

void Renderer::updateProjection(i32 width, i32 height) {
	w = width;
	h = height;
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	program->set4Matrix("toProjection", projection);
}

void Renderer::createVertexArray(IRenderable* mesh)
{
	numberOfVertices += mesh->getNumOfVertices();
	const auto v = mesh->getVertices();
    copy(v.begin(), v.end(), back_inserter(vertices));
}

void Renderer::createTexCoordArray(IRenderable* mesh)
{
	numOfTexCoords += mesh->getNumOfTexCoord();
    const auto meshTexCoords = mesh->getTexCoords();
    copy(meshTexCoords.begin(), meshTexCoords.end(), back_inserter(texCoords));
}

void Renderer::createNormalsArray(IRenderable* mesh)
{
	numberOfNormals += mesh->getNumOfNormals();
	const auto meshNormals = mesh->getNormals();
    copy(meshNormals.begin(), meshNormals.end(), back_inserter(normals));
}

void Renderer::createElementArray(IRenderable* mesh)
{

	int vert_shift = (numberOfVertices - mesh->getNumOfVertices());
	int el_shift   =  numberOfElements;

	if(mesh->use_indices ) {
		
		const auto indices = mesh->getIndices();
		numberOfElements += (i32)indices.size();

		i32 *data = elements.data();

		copy(indices.begin(), indices.end(), back_inserter(elements));

	} else {
		// REMEMBER TO MULTIPLY BY 3 !!!
		// Steven: This line is causing problem as the number of elements is not the same as
		// number of faces.
		numberOfElements += mesh->getNumOfFaces() * 3;
		const auto faces = mesh->getFaces();
		for(i32 i = 0; i < faces.size(); i++) {
			elements.push_back(faces[i][0]);
			elements.push_back(faces[i][1]);
			elements.push_back(faces[i][2]);
		}
	}

    for (u32 i = el_shift; i < elements.size(); i++)
    {
        elements[i] += vert_shift;
    }

}

void Renderer::createTangents(IRenderable* mesh)
{

    Array<vec3> v = mesh->getVertices();
    Array<vec2> tc = mesh->getTexCoords();
    Array<vec3> n = mesh->getNormals();
    Array<ivec3> elem = mesh->getFaces();


	if(mesh->use_indices)
	{
		Array<vec3> newTangents(mesh->getNumOfVertices());
		copy(newTangents.begin(), newTangents.end(), back_inserter(tangents));
		numOfTangents += (i32)tangents.size();

		return;
	}


    i32 i = (i32) v[elem[0][0]].x;

    Array<vec3> newTangents(mesh->getNumOfVertices());

    for (auto itr = newTangents.begin(); itr != newTangents.end(); ++itr) {
		itr->r = 0;
		itr->b = 0;
		itr->g = 0;
	}

	tangents.resize(newTangents.size());
	numOfTangents = (i32) tangents.size();

    // For each triangle in the mesh
    for (int i = 0; i < elem.size(); i+=3)
    {
        // Get the positions of the vertices, their tex coordinates and the normal coordinates
        // Positions
        int el_ind1 = elem[i][0];
        int el_ind2 = elem[i][1];
        int el_ind3 = elem[i][2];

        vec3 pos1 = v[el_ind1];
        vec3 pos2 = v[el_ind2];
        vec3 pos3 = v[el_ind3];

        // Texture coordinates
        vec2 uv1 = tc[el_ind1];
        vec2 uv2 = tc[el_ind2];
        vec2 uv3 = tc[el_ind3];;

        vec3 edge1 = pos2 - pos1;
        vec3 edge2 = pos3 - pos1;
        vec2 deltaUV1 = uv2 - uv1;
        vec2 deltaUV2 = uv3 - uv1;

        vec3 tangent;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        // Save the tangents to each vertice of the triangle
        newTangents[el_ind1] += tangent;
        newTangents[el_ind2] += tangent;
		newTangents[el_ind3] += tangent;
    }

    // Once all of the tangents have been calculated, some would have summed tangents.
    // Devide by the number of triangles the vertice is the part of to find the average
    std::vector<int> verticeTrianglesCount;
    verticeTrianglesCount.resize(mesh->getNumOfVertices());
    for (auto itr = verticeTrianglesCount.begin(); itr != verticeTrianglesCount.end(); ++itr) *itr = 0;

    for (int i = 0; i < elem.size(); i++)
    {
        verticeTrianglesCount[elem[i][0]]++;
        verticeTrianglesCount[elem[i][1]]++;
        verticeTrianglesCount[elem[i][2]]++;
    }

    for (int i = 0; i < newTangents.size(); i++)
    {
        newTangents[i] /= verticeTrianglesCount[i];
    }

    // Add calculated tangents to the container
    copy(newTangents.begin(), newTangents.end(), back_inserter(tangents));
    numOfTangents += (i32)tangents.size();
}

void Renderer::updateCamera()
{
    program->set4Matrix("toCamera", camera->getCameraTransf());
}

void Renderer::updateCamera(Camera* cam)
{
    camera = cam;
    program->set4Matrix("toCamera", camera->getCameraTransf());
}

void Renderer::useProgram()
{
	program->use();
	// Set up Projection matrix
	// int toProjectionLoc = program->getUniformLocation("toProjection");
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
	program->set4Matrix("toProjection", projection);

	updateCamera();

    program->set3Float("cameraPosition", camera->GetCameraPosition());
    program->set3Float("lightPosition", 0.0f, 60.0f, 0.0f);
    program->set4Matrix("toWorld", mat4(1.0f));
    program->set4Matrix("modelMatrix", mat4(1.0f));

}

void Renderer::updateLightPos(float x, float y, float z) 
{
    program->use();
    program->set3Float("lightPosition",x, y, z);
}

void Renderer::updateObjectTransformation(mat4 transformation, u32 rendObjId) {

	const auto obj = objects.find(rendObjId);
	if (obj != objects.end()) {
		obj->second.transformation = transformation;
	}
}

void Renderer::changeTexture(Entity* ent)
{
	for (u32 i = 0; i < objects.size(); i++)
	{
		if (objects[i].ent == ent)
		{
			RenderableComponent* rendComp = objects[i].ent->getComp<RenderableComponent>();
			//rendComp->getAmbientTexture() doesn't return anything here atm, dunno why
			//objects[i].ambientTexture = setTexture(rendComp->ambientTexture, "AmbTexture", 1);
			objects[i].ambientTexture = setTexture(rendComp->getAmbientTexture(), "AmbTexture", 1);
			objects[i].ambientTexturePath = rendComp->getAmbientTexture();
			objects[i].normalTexturePath = rendComp->getNormalTexture();

			// Reset texture path for RenderableComponent as other object may use the same reference
			rendComp->ambientTexture = "";
			rendComp->normalTexture = "";
		}
	}
}

bool Renderer::hasRendObj(u32 id)
{
	return objects.find(id) != objects.end();
}

void Renderer::setupSkybox() {
	//set vertex data
	float vertices_temp[] = {

		 -1.0, -1.0, -1.0,
		  1.0, -1.0, -1.0,
		  1.0, -1.0,  1.0,
		 -1.0, -1.0, -1.0,
		  1.0, -1.0,  1.0,
		 -1.0, -1.0,  1.0,

		 -1.0, -1.0, -1.0,
		 -1.0,  1.0, -1.0,
          1.0,  1.0, -1.0,
		 -1.0, -1.0, -1.0,
          1.0,  1.0, -1.0,
          1.0, -1.0, -1.0,

		 -1.0, -1.0, -1.0,
		 -1.0, -1.0,  1.0,
		 -1.0,  1.0,  1.0,
		 -1.0, -1.0, -1.0,
		 -1.0,  1.0,  1.0,
		 -1.0,  1.0, -1.0,

		  1.0, -1.0, -1.0,
		  1.0,  1.0, -1.0,
		  1.0, -1.0,  1.0,
		  1.0,  1.0, -1.0,
		  1.0,  1.0,  1.0,
		  1.0, -1.0,  1.0,

		 -1.0, -1.0,  1.0,
		  1.0, -1.0,  1.0,
		 -1.0,  1.0,  1.0,
		  1.0, -1.0,  1.0,
		  1.0,  1.0,  1.0,
		 -1.0,  1.0,  1.0,

		 -1.0,  1.0,  1.0,
		  1.0,  1.0,  1.0,
		  1.0,  1.0, -1.0,
		 -1.0,  1.0,  1.0,
		  1.0,  1.0, -1.0,
		 -1.0,  1.0, -1.0
	};

	glGenVertexArrays(1, &skyVAO);
	glBindVertexArray(skyVAO);

	glGenBuffers(1, &skyVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sizeof(vertices_temp)/sizeof(vertices_temp[0]), &vertices_temp, GL_STATIC_DRAW);

	glBindVertexArray(0);

	Array<String> images {
		"assets/skybox/1/right.jpg",
		"assets/skybox/1/left.jpg",
		"assets/skybox/1/top.jpg",
		"assets/skybox/1/down.jpg",
		"assets/skybox/1/front.jpg",
		"assets/skybox/1/back.jpg"
	};

	setSkyBoxImages(images);

}

void Renderer::liveReloadFile(const char *file, LiveReloadEntry *entry) {

	i32 image_index = -1;

	for(u32 i = 0; i < skyboxImages.size(); i++) {
		if(skyboxImages[i] == file) {
			image_index = i;
			break;
		}
	}

	if(image_index > -1 && image_index < 6) {
		i32 width;
		i32 height;
		i32 channels;

		skyboxImages[image_index] = file;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		stbi_set_flip_vertically_on_load(false); // flip loaded texture's on the y-axis.
		u8 *TextureData = stbi_load(file, &width, &height, &channels, 0);

		if(TextureData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + image_index, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
			stbi_image_free(TextureData);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			entry->changed = 0;
		}

	}
}

void Renderer::setSkyBoxImages(const Array<String> &_skyboxImages)
{
	skyboxImages = _skyboxImages;

	for(u32 i = 0; i < skyboxImages.size(); i++) {
		LiveReloadManager::Instance()->addLiveReloadEntry(skyboxImages[i].c_str(), static_cast<IReloadableFile*>(this));
	}

	skyboxLoadTexture();
}

void Renderer::skyboxLoadTexture()
{
    glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    i32 width;
	i32 height;
	i32 channels;
	unsigned char* TextureData = NULL;

	for(u32 i = 0; i < 6; i++ ) {
		TextureData = stbi_load(skyboxImages[i].c_str(), &width, &height, &channels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
		stbi_image_free(TextureData);
	}

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Renderer::drawSkyBox()
{
    // draw scene
    glm::mat4 view = camera->getCameraTransf();;

	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;

    program->use();
    setFrameBufferToTexture();

    // draw skybox 
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    program->set4Matrix("view", view);
    program->set4Matrix("projection", projection);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Reset everything back to normal
    glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    setFrameBufferToDefault();

    glBindVertexArray(0);

}

const char* Renderer::getSkyboxImagePath(u32 skyboxPosition) {
	// positions are in this order: +x, -x, +y, -y, +z, -z
	return skyboxImages[skyboxPosition].c_str();
}

void Renderer::setSkyboxImage(const String& image_path, u32 skyboxPosition) {
	// positions are in this order: +x, -x, +y, -y, +z, -z

	i32 width;
	i32 height;
	i32 channels;


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	stbi_set_flip_vertically_on_load(false); // flip loaded texture's on the y-axis.
	u8 *TextureData = stbi_load(image_path.c_str(), &width, &height, &channels, 0);

	if(TextureData) {

		if(skyboxPosition >= 0 && skyboxPosition < 6 && skyboxImages[skyboxPosition] != image_path) {
			LiveReloadManager::Instance()->removeLiveReloadEntry(skyboxImages[skyboxPosition].c_str(), static_cast<IReloadableFile*>(this));
			LiveReloadManager::Instance()->addLiveReloadEntry(image_path.c_str(), static_cast<IReloadableFile*>(this));
		}

		skyboxImages[skyboxPosition] = image_path;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + skyboxPosition, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
		stbi_image_free(TextureData);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}
	
}
