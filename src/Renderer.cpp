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

Renderer::Renderer(int width, int height, Camera* cam) : w(width), h(height), camera(cam), elements(0) {

	// Initialise OpenGl

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

	// Bind vert attribute handle
	glBindVertexArray(VAO);

	// Update GPU containers
	// Vertice positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * numberOfVertices * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	int positionAtr = program->getAtrributeLocation("position");
	glVertexAttribPointer(positionAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexArrayAttrib(VAO, positionAtr);

	// Normal positions
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * numOfTexCoords * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

	int normalAtr = program->getAtrributeLocation("normal");
	glVertexAttribPointer(normalAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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

    // Tangents
    glBindBuffer(GL_ARRAY_BUFFER, TANBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * numOfTangents* sizeof(GLfloat), tangents.data(), GL_STATIC_DRAW);

    int tangentAtr = program->getAtrributeLocation("tangent");
    glVertexAttribPointer(tangentAtr, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexArrayAttrib(VAO, tangentAtr);

    // Elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numberOfElements * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}


void Renderer::addObject(IRenderable *mesh, IPosition *pos)
{

    // Add a mesh to the container
    RendObj newObj;
    newObj.objPtr = mesh;
    newObj.startInd = (i32)elements.size();

    newObj.pos = glm::translate(glm::mat4(1.0f), glm::vec3(pos->x, pos->y, pos->z));
    
    // Generate textures for the object
    newObj.ambientTexture = setTexture(mesh->getAmbientTexture(), "AmbTexture", 1);
    newObj.normalTexture = setTexture(mesh->getNormalTexture(), "NormTexture", 2);

	// Generate the arrays
	createVertexArray(mesh);
	createTexCoordArray(mesh);
	createNormalsArray(mesh);
    createTangents(mesh);
	createElementArray(mesh);

	newObj.endInd = i32(elements.size());

    newObj.transformation = glm::mat4(1.0f);

	objects.push_back(newObj);

}

GLuint Renderer::setTexture(const char* texturePath, const char* uniName, int num) {

	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0 + num);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
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
		Logger::debug("Failed to load file %s ", texturePath);
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
        program->set4Matrix("toWorld", objects[i].pos);

        program->set4Matrix("modelMatrix", objects[i].transformation);

        // Activate and bind textures of the object
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, objects[i].ambientTexture);

        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, objects[i].normalTexture);

        // Draw the object
        glDrawElements(GL_TRIANGLES, (objects[i].endInd - objects[i].startInd), GL_UNSIGNED_INT, (void*)(objects[i].startInd * sizeof(unsigned int))); // IMPORTANT (void*)(6*3 * sizeof(unsigned int))

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



void Renderer::updateProjection(int width, int height) {

	w = width;
	h = height;

	program->use();

	glViewport(0, 0, w, h);

    // Set up Projection matrix
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);

    //int toProjectionLoc = shader->getUniformLocation("toProjection");
    //glUniformMatrix4fv(toProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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

	std::vector<f32> v;

    mesh->getArrayOfVertices(&v);

    copy(&v.data()[0], &v.data()[3 * mesh->getNumOfVertices()], back_inserter(vertices));

}



void Renderer::createTexCoordArray(IRenderable* mesh)
{
	numOfTexCoords += mesh->getNumOfTexCoord();

    std::vector<float>* tC = new std::vector<float>;
    mesh->getArrayOfTexCoord(tC);

    copy(&tC->data()[0],
        &tC->data()[2 * mesh->getNumOfTexCoord()],
        back_inserter(texCoords));

    delete tC;
}

void Renderer::createNormalsArray(IRenderable* mesh)
{
	numberOfNormals += mesh->getNumOfNormals();

    std::vector<float>* n = new std::vector<float>;
    mesh->getArrayOfNormals(n);

    copy(&n->data()[0],
        &n->data()[3 * mesh->getNumOfNormals()],
        back_inserter(normals));

    delete n;
}



void Renderer::createElementArray(IRenderable* mesh)
{

	int vert_shift = (numberOfVertices - mesh->getNumOfVertices());
	int el_shift = 3 * numberOfElements;

	numberOfElements += mesh->getNumOfElements();

    std::vector<int>* elem = new std::vector<int>;
    mesh->getArrayOfElements(elem);

    copy(&elem->data()[0],
        &elem->data()[3 * mesh->getNumOfElements()],
        back_inserter(elements));

    delete elem;

    for (unsigned int i = el_shift; i < elements.size(); i++)
    {
        elements.at(i) += vert_shift;
    }

}





void Renderer::createTangents(IRenderable* mesh)
{
    std::vector<float> v;
    mesh->getArrayOfVertices(&v);

    std::vector<float> tc;
    mesh->getArrayOfTexCoord(&tc);

    std::vector<float> n;
    mesh->getArrayOfNormals(&n);

    std::vector<int> elem; 
    mesh->getArrayOfElements(&elem);

    i32 i = (i32)v.at(elem.at(0));

    std::vector<float> newTangents;
    newTangents.resize(mesh->getNumOfVertices()*3);

    for (auto itr = newTangents.begin(); itr != newTangents.end(); ++itr) {
		*itr = 0.0f;
	}

    // For each triangle in the mesh
    for (int i = 0; i < elem.size(); i+=3)
    {
        // Get the positions of the vertices, their tex coordinates and the normal coordinates

        // Positions
        int el_ind1 = elem.at(i);
        int el_ind2 = elem.at(i + 1);
        int el_ind3 = elem.at(i + 2);


        glm::vec3 pos1(v.at(el_ind1 * 3), v.at(el_ind1 * 3 + 1), v.at(el_ind1 * 3 + 2));
        glm::vec3 pos2(v.at(el_ind2 * 3), v.at(el_ind2 * 3 + 1), v.at(el_ind2 * 3 + 2));
        glm::vec3 pos3(v.at(el_ind3 * 3), v.at(el_ind3 * 3 + 1), v.at(el_ind3 * 3 + 2));

        // Texture coordinates
        glm::vec2 uv1(tc[el_ind1 * 2], tc[el_ind1 * 2 + 1]);
        glm::vec2 uv2(tc[el_ind2 * 2], tc[el_ind2 * 2 + 1]);
        glm::vec2 uv3(tc[el_ind3 * 2], tc[el_ind3 * 2 + 1]);

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;


        glm::vec3 tangent;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);



        // Save the tangents to each vertice of the triangle
        newTangents[el_ind1 * 3] += tangent.x;
		newTangents[el_ind1 * 3 + 1] += tangent.y;
		newTangents[el_ind1 * 3 + 2] += tangent.z;

        newTangents[el_ind2 * 3] += tangent.x;
		newTangents[el_ind2 * 3 + 1] += tangent.y;
		newTangents[el_ind2 * 3 + 2] += tangent.z;

        newTangents[el_ind3 * 3] += tangent.x;
		newTangents[el_ind3 * 3 + 1] += tangent.y;
		newTangents[el_ind3 * 3 + 2] += tangent.z;
    }


    // Once all of the tangents have been calculated, some would have summed tangents.
    // Devide by the number of triangles the vertice is the part of to find the average
    std::vector<int> verticeTrianglesCount;
    verticeTrianglesCount.resize(mesh->getNumOfVertices());
    for (auto itr = verticeTrianglesCount.begin(); itr != verticeTrianglesCount.end(); ++itr) *itr = 0;

    for (int i = 0; i < elem.size(); i++)
    {
        verticeTrianglesCount[elem[i]]++;
    }

    for (int i = 0; i < newTangents.size() / 3; i++)
    {
        newTangents[i * 3] = newTangents[i * 3] / verticeTrianglesCount[i];
        newTangents[i * 3 + 1] = newTangents[i * 3 + 1] / verticeTrianglesCount[i];
        newTangents[i * 3 + 2] = newTangents[i * 3 + 2] / verticeTrianglesCount[i];
    }

    // Add calculated tangents to the container
    copy(&newTangents.data()[0], &newTangents.data()[newTangents.size()], back_inserter(tangents));
    numOfTangents = (i32)tangents.size()/3;
}











void Renderer::updateCamera()
{
    //int toCameraLoc = shader->getUniformLocation("toCamera");
    //glUniformMatrix4fv(toCameraLoc, 1, GL_FALSE, glm::value_ptr(camera->getCameraTransf()));
    program->set4Matrix("toCamera", camera->getCameraTransf());
}


void Renderer::updateCamera(Camera* cam)
{
    camera = cam;
    //int toCameraLoc = shader->getUniformLocation("toCamera");
    //glUniformMatrix4fv(toCameraLoc, 1, GL_FALSE, glm::value_ptr(camera->getCameraTransf()));

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
    program->set3Float("cameraPosition", camera->currCamPos.x, camera->currCamPos.y, camera->currCamPos.z);
    // Set up light position
    program->set3Float("lightPosition", 0.0f, 60.0f, 0.0f);

    program->set4Matrix("toWorld", glm::mat4(1.0f));

    program->set4Matrix("modelMatrix", glm::mat4(1.0f));
    
}

void Renderer::updateLightPos(float x, float y, float z) 
{
    program->use();
    program->set3Float("lightPosition",x, y, z);
}

void Renderer::updateObjectTransformation(glm::mat4 transformation, IRenderable* pRenderable)
{
    for (u32 i = 0; i < objects.size(); i++)
    {
        if (objects[i].objPtr == pRenderable)
        {
            objects[i].transformation = transformation;
            //program->set4Matrix("modelMatrix", transformation);
            //std::cout << "Welcome to the Matrix" << "\n";
        }
    }
}
