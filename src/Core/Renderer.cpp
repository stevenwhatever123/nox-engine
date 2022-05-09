#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <iterator>

#include <Core/Renderer.h>
#include <Utils/Utils.h>
#include <Components/PositionComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/IRenderable.h>
#include <Core/Types.h>

#include <3rdParty/stb/stb_image.h>

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
	objects(0),
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
	color(0)
{

	// Initialise OpenGl

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Create framebuffer  
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Gen texture for framebuffer    
    glGenTextures(1, &textureToRenderTo);
    glBindTexture(GL_TEXTURE_2D, textureToRenderTo);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // Attach tex to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureToRenderTo, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Troubles with creating a framebuffer\n");
    }


    // Generate buffer handlers
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &TCBO);
    glGenBuffers(1, &TANBO);

    glGenBuffers(1, &EBO);
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


void Renderer::addObject(Entity *ent)
{

    // Add a mesh to the container
    RendObj newObj;
    newObj.ent = ent;

	IRenderable* mesh = ent->getComp<RenderableComponent>()->CastType<IRenderable>();

	newObj.renderType = mesh->glRenderType;
    newObj.startInd = (i32)elements.size();
	newObj.has_texture = mesh->has_texture;
	newObj.has_normal = mesh->has_normal;
    
    // Generate textures for the object
    newObj.ambientTexture = setTexture(mesh->getAmbientTexture(), "AmbTexture", 1);
    newObj.normalTexture = setTexture(mesh->getNormalTexture(), "NormTexture", 2);

	// Generate the arrays
	createVertexArray(mesh);

	if(mesh->has_texture) createTexCoordArray(mesh);
	if(mesh->has_normal)  createNormalsArray(mesh);

	if(mesh->has_normal && mesh->has_texture) createTangents(mesh);

	createElementArray(mesh);

	newObj.endInd = i32(elements.size());
    newObj.transformation = mat4(1.0f);
	objects.push_back(newObj);

    LOG_DEBUG("Renderer object count: %i\n", objects.size());
}

void Renderer::removeObject(Entity* ent) {

    // Note: This calls the destructor on RendObj
    objects.erase(
        std::remove_if(objects.begin(), objects.end(), [ent](RendObj obj) { return obj.ent == ent; }),
        objects.end()
    );
    
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

	glBindVertexArray(VAO);

	for (u32 i = 0; i < objects.size(); i++)
	{
		// Skip if the entity/RenderableComponent is not enabled
		if (!objects[i].ent->isEntityEnabled()) continue;
		if (!objects[i].ent->isEnabled<RenderableComponent>()) continue;

		// If the object has a position and it's enabled, use it
		mat4 worldMat = mat4(1.0f);
		if (objects[i].ent->containsComps<PositionComponent>() && objects[i].ent->isEnabled<PositionComponent>()) {
			IPosition* pos = objects[i].ent->getComp<PositionComponent>()->CastType<IPosition>();
			worldMat = translate(mat4(1.0f), vec3(pos->x, pos->y, pos->z));
		}

		program->set4Matrix("toWorld", worldMat);
        program->set4Matrix("modelMatrix", objects[i].transformation);

        // Activate and bind textures of the object
		if(objects[i].has_texture) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, objects[i].ambientTexture);
		}

		if(objects[i].has_texture) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, objects[i].normalTexture);
		}

		// Draw the object
		glDrawElements(objects[i].renderType, (objects[i].endInd - objects[i].startInd), GL_UNSIGNED_INT, (void*)(objects[i].startInd * sizeof(i32)));
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
	glClear(GL_COLOR_BUFFER_BIT);
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



void Renderer::updateProjection(int width, int height) {

	w = width;
	h = height;

	program->use();

	glViewport(0, 0, w, h);

    // Set up Projection matrix
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);

    //int toProjectionLoc = shader->getUniformLocation("toProjection");
    //glUniformMatrix4fv(toProjectionLoc, 1, GL_FALSE, value_ptr(projection));

    program->set4Matrix("toProjection", projection);

	glBindFramebuffer(GL_FRAMEBUFFER, curFBO);

	// Gen texture for framebuffer

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureToRenderTo);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Attach tex to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureToRenderTo, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Troubles with creating a framebuffer\n");
	}

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
		numberOfElements += mesh->getNumOfFaces();
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
	// Set up color for mesh
	// glUniform3f(program->getUniformLocation("Color"), color.x, color.y, color.z);

    // Set up camera position
    program->set3Float("cameraPosition", camera->GetCameraPosition());
    // Set up light position
    program->set3Float("lightPosition", 0.0f, 60.0f, 0.0f);
    program->set4Matrix("toWorld", mat4(1.0f));
    program->set4Matrix("modelMatrix", mat4(1.0f));
}

void Renderer::updateLightPos(float x, float y, float z) 
{
    program->use();
    program->set3Float("lightPosition",x, y, z);
}

void Renderer::updateObjectTransformation(mat4 transformation, IRenderable* pRenderable)
{
	for (u32 i = 0; i < objects.size(); i++)
	{
		IRenderable* rend = objects[i].ent->getComp<RenderableComponent>()->CastType<IRenderable>();
		if (rend == pRenderable)
		{
			objects[i].transformation = transformation;
			//program->set4Matrix("modelMatrix", transformation);
			//std::cout << "Welcome to the Matrix" << "\n";
		}
	}
}

