#define STB_IMAGE_IMPLEMENTATION
#include <Renderer.h>
#include <Utils.h>


// TODO: update uniform submissions to use Shader class
// TODO: fix drawing to default buffer
// TODO: get some light going
// TODO: material class
// TODO(sharo): debug context and messages

using NoxEngineUtils::Logger;
using namespace NoxEngine;

Renderer::~Renderer() {
	vertices.clear();
	normals.clear();
	texCoords.clear();

	elements.clear();
	glDeleteVertexArrays(1, &(VAO));
	glDeleteBuffers(1, &(VBO));
	glDeleteBuffers(1, &(NBO));
	glDeleteBuffers(1, &(TCBO));
	glDeleteBuffers(1, &(EBO));

	// Remove shader
	// Remove framebuffer
	glDeleteFramebuffers(1, &FBO);
}

Renderer::Renderer(int width, int height, Camera* cam) : w(width), h(height), camera(cam) {

	// Initialise OpenGl

	// Note(sharo): we already initialize glew when we create a window
	// glewInit();

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

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Troubles with creating a framebuffer\n");
	}

	// Generate buffer handlers
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TCBO);
	glGenBuffers(1, &EBO);
}

void Renderer::updateBuffers() {

	// Bind vert attribute handle
	glBindVertexArray(VAO);

	// Update GPU containers
	// Vertice positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * numberOfVertices * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	int positionAtr = program->getAtrributeLocation("position");
	glVertexAttribPointer(positionAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, positionAtr);

	// Normal positions. At the moment uses textCoord because the normals are submitted with texture
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * numOfTexCoords * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	int normalAtr = program->getAtrributeLocation("normal");
	glVertexAttribPointer(normalAtr, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, normalAtr);

	// Tex Coords positions
	glBindBuffer(GL_ARRAY_BUFFER, TCBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * numOfTexCoords * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	int texCoordAtr = program->getAtrributeLocation("texCoord");
	glVertexAttribPointer(texCoordAtr, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, texCoordAtr);

	// Elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numberOfElements * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void Renderer::addObject(IRenderable *mesh) {

	// Add a mesh to the container
	RendObj newObj;
	newObj.objPtr = mesh;
	newObj.startInd = i32(elements.size());

	// Generate textures for the object
	newObj.ambientTexture = setTexture(mesh->getAmbientTexture(), "AmbTexture", 1);
	newObj.normalTexture = setTexture(mesh->getNormalTexture(), "NormTexture", 2);
	std::cout << "Texture " << glGetError() << std::endl; fflush(NULL);

	// Generate the arrays
	createVertexArray(mesh);
	createTexCoordArray(mesh);
	createNormalsArray(mesh);

	createElementArray(mesh);
	newObj.endInd = i32(elements.size());

	objects.push_back(newObj);

}

GLuint Renderer::setTexture(const char* texturePath, const char* uniName, int num) {

	GLuint tex;
	glGenTextures(1, &tex);
	std::cout << "1 " << glGetError() << std::endl; fflush(NULL);

	glActiveTexture(GL_TEXTURE0 + num);
	std::cout << "2 " << glGetError() << std::endl; fflush(NULL);
	glBindTexture(GL_TEXTURE_2D, tex);
	std::cout << "3" << glGetError() << std::endl; fflush(NULL);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	std::cout << "4 " << glGetError() << std::endl; fflush(NULL);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	std::cout << "5 " << glGetError() << std::endl; fflush(NULL);
	// Load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flip loaded texture's on the y-axis.

	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(data);

	GLuint textureLoc = program->getUniformLocation(uniName);
	glUniform1i(textureLoc, num);

	return tex;
}



void Renderer::draw() {

	// Render
	glBindFramebuffer(GL_FRAMEBUFFER, curFBO);

	glClearColor(0.0f, 0.5f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	program->use();

	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		// Activate and bind textures of the object
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, objects[i].ambientTexture);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, objects[i].normalTexture);

		// Draw the object
		glDrawElements(GL_TRIANGLES, (objects[i].endInd - objects[i].startInd), GL_UNSIGNED_INT, (void*)(objects[i].startInd * sizeof(unsigned int))); // IMPORTANT (void*)(6*3 * sizeof(unsigned int))

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);

}

void Renderer::fillBackground(float r, float g, float b) {

	// Set background color
	program->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(r,g,b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}



void Renderer::updateProjection(int width, int height) {

	w = width;
	h = height;

	program->use();

	glViewport(0, 0, w, h);

	// Set up Projection matrix
	int toProjectionLoc = program->getUniformLocation("toProjection");

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)w / (GLfloat)h, 0.1f, 200.0f);
	glUniformMatrix4fv(toProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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

	copy(&mesh->getArrayOfVertices()[0], &mesh->getArrayOfVertices()[3 * mesh->getNumOfVertices()],
			back_inserter(vertices));
}



void Renderer::createTexCoordArray(IRenderable* mesh)
{
	numOfTexCoords += mesh->getNumOfTexCoord();

	copy(&mesh->getArrayOfTexCoord()[0], &mesh->getArrayOfTexCoord()[2 * mesh->getNumOfTexCoord()],
			back_inserter(texCoords));
}

void Renderer::createNormalsArray(IRenderable* mesh)
{
	numberOfNormals += mesh->getNumOfNormals();

	copy(&mesh->getArrayOfNormals()[0], &mesh->getArrayOfNormals()[3 * mesh->getNumOfNormals()],
			back_inserter(normals));
}



void Renderer::createElementArray(IRenderable* mesh)
{

	int vert_shift = (numberOfVertices - mesh->getNumOfVertices());
	int el_shift = 3 * numberOfElements;

	numberOfElements += mesh->getNumOfElements();

	copy(&mesh->getArrayOfElements()[0],
			&mesh->getArrayOfElements()[3 * mesh->getNumOfElements()],
			back_inserter(elements));

	for (unsigned int i = el_shift; i < elements.size(); i++)
	{
		elements.at(i) += vert_shift;
	}

}


void Renderer::updateCamera()
{
	int toCameraLoc = program->getUniformLocation("toCamera");
	glUniformMatrix4fv(toCameraLoc, 1, GL_FALSE, glm::value_ptr(camera->getCameraTransf()));
}


void Renderer::updateCamera(Camera* cam)
{
	camera = cam;
	int toCameraLoc = program->getUniformLocation("toCamera");
	glUniformMatrix4fv(toCameraLoc, 1, GL_FALSE, glm::value_ptr(camera->getCameraTransf()));
}

void Renderer::useProgram()
{
	program->use();
	// Set up Projection matrix
	int toProjectionLoc = program->getUniformLocation("toProjection");
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)w / (GLfloat)h, 0.1f, 200.0f);
	glUniformMatrix4fv(toProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	// Set up Camera
	// updateCamera();
	// Set up color for mesh
	glUniform3f(program->getUniformLocation("Color"), color.x, color.y, color.z);

}
