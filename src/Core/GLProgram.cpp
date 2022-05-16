#include <glad/glad.h>
#include <Core/GLProgram.h>
#include <Managers/IOManager.h>
#include <Utils/MemAllocator.h>
#include <assert.h>

using namespace NoxEngine;

u32 GLProgram::compileShader(String& filename, i32 shaderType) {
	u32 id = glCreateShader(shaderType);
	TempResourceData temp = IOManager::Instance()->ReadEntireFileTemp(filename);
	const char *data = (const char*)temp.data;

	glShaderSource(id, 1, &data, NULL);
	glCompileShader(id);

	i32 result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		i32 infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *temp_buf = (char*)StackMemAllocator::Instance()->allocate(infoLogLength);
		glGetShaderInfoLog(id, infoLogLength, NULL, temp_buf);
		LOG_DEBUG("Error Compiling Shader: \n%s", temp_buf);
		StackMemAllocator::Instance()->free((u8*)temp_buf);

		return 0;
	}

	return id;
}

GLuint GLProgram::compileShaderFromString(std::string shaderData, GLenum shaderType) {
	GLuint id = glCreateShader(shaderType);

	const char* data = &shaderData[0];

	glShaderSource(id, 1, &data, NULL);
	glCompileShader(id);

	GLint result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		i32 infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* temp_buf = (char*)StackMemAllocator::Instance()->allocate(infoLogLength);
		glGetShaderInfoLog(id, infoLogLength, NULL, temp_buf);
		LOG_DEBUG("Error Compiling Shader: \n%s", temp_buf);
		StackMemAllocator::Instance()->free((u8*)temp_buf);
	}

	return id;
}


u32 GLProgram::makeProgram(Array<ShaderFile> shaders) {
	u32 id = glCreateProgram();
	for(i32 i = 0; i < shaders.size(); i++) {
		if(shaders[i].id == 0) continue;
		glAttachShader(id, shaders[i].id);
	}


	glLinkProgram(id);

	// Check the program
	i32 result;
	i32 length; 
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char *buffer = (char*)StackMemAllocator::Instance()->allocate(length);
		glGetProgramInfoLog(id, length, NULL, buffer);
		LOG_DEBUG("Error Linking program: \n%s", buffer);
		StackMemAllocator::Instance()->free((u8*)buffer);
	}

	return id;
}

GLProgram::GLProgram(Array<ShaderFile> shaders)
{
	for(i32 i = 0; i < shaders.size(); i++) {
		shaders[i].id = compileShader(shaders[i].filename, shaders[i].shader_type);
	}

	_id = makeProgram(shaders);
	_shaders = shaders;
}

void GLProgram::use()
{
	glUseProgram(_id);
}


void GLProgram::setBool(const std::string& name, bool value) const
{
	glProgramUniform1i(_id, glGetUniformLocation(_id, name.c_str()), (int)value);
}

void GLProgram::setInt(const std::string& name, int value) const
{
	glProgramUniform1i(_id, glGetUniformLocation(_id, name.c_str()), value);
}

void GLProgram::setFloat(const std::string& name, float value) const
{
	glProgramUniform1f(_id, glGetUniformLocation(_id, name.c_str()), value);
}

void GLProgram::set3Float(const std::string& name, float x, float y, float z) const
{
    glProgramUniform3f(_id, glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void GLProgram::set4Float(const std::string& name, float x, float y, float z, float w) const
{
	glProgramUniform4f(_id, glGetUniformLocation(_id, name.c_str()), x, y, z, w);
}

void GLProgram::set4Matrix(const std::string &name, glm::mat4 mat) const
{
  glProgramUniformMatrix4fv(_id, glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

int GLProgram::getUniformLocation(const std::string& name)
{
	return glGetUniformLocation(_id, name.c_str());
}

int GLProgram::getAtrributeLocation(const std::string& name)
{
	return glGetAttribLocation(_id, name.c_str());
}

void GLProgram::printAttribInfo()
{
	i32 numActiveAttribs = 0;
	glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);

	LOG_DEBUG("Program: %d has %d active attribs.", _id, numActiveAttribs);

	char *mem = (char*)StackMemAllocator::Instance()->allocate(1024);
	i32 size;
	i32 type;
	for (i32 i = 0; i < numActiveAttribs; i++)
	{
		glGetActiveAttrib(_id, i, 1024, NULL, &size, (GLenum*)&type, mem);
		LOG_DEBUG("Attribute %s idx %d", mem, i);
		StackMemAllocator::Instance()->free((u8*)mem);
	}
}

void GLProgram::changeLightNum(i32 num_of_light)
{
	numOfLights = num_of_light;
	for (u32 i = 0; i < _shaders.size(); i++)
	{
		// Read shader content
		TempResourceData temp = IOManager::Instance()->ReadEntireFileTemp(_shaders[i].filename);
		std::string shaderData = std::string((const char*)temp.data);

		// Check if the shaders attached to this program have num_of_lights defined
		size_t found = shaderData.find("#define NUM_OF_LIGHTS");

		// If not -> assume that the shaders don't have multiple lights and escape
		if (found == std::string::npos)
		{
			// TODO: find it/ask it LOG MESSAGE HERE
			return;
		}

		// If has -> change the num of lights

		std::string result;
		result = "#version 450 core\n#define NUM_OF_LIGHTS " + std::to_string(num_of_light);
		num_of_light;
		result += "\n";

		std::string restOf = shaderData.substr(found);
		found = restOf.find("\n");
		result += restOf.substr(found + 1);

		// Create shaders
		_shaders[i].id = compileShaderFromString(result, _shaders[i].shader_type);

	}

	// Make the program
	GLuint newId = makeProgram(_shaders);

	// Delete existing program
	glDeleteProgram(_id);

	// Store new one
	_id = newId;

	return;

}

