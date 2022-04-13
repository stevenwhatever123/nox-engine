#include "GLProgram.h"
#include <IOManager.h>
#include <MemAllocator.h>
#include <assert.h>

namespace NoxEngine {

	GLuint GLProgram::compileShader(std::string& filename, GLenum shaderType) {
		GLuint id = glCreateShader(shaderType);
		auto temp = IOManager::Instance()->ReadEntireFileTemp(filename);
		const char *data = (const char*)temp.data;

		glShaderSource(id, 1, &data, NULL);
		glCompileShader(id);

		GLint result = GL_FALSE;

		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result != GL_TRUE) {
			i32 infoLogLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			char *temp_buf = (char*)StackMemAllocator::Instance()->allocate(infoLogLength);
			glGetShaderInfoLog(id, infoLogLength, NULL, temp_buf);
			Logger::debug("Error Compiling Shader: \n%s", temp_buf);
			StackMemAllocator::Instance()->free((u8*)temp_buf);
		}

		return id;
	}

	GLuint GLProgram::makeProgram(std::vector<ShaderFile> shaders) {
		GLuint id = glCreateProgram();
		for(i32 i = 0; i < shaders.size(); i++) {
			assert(shaders[i].id != 0);
			glAttachShader(id, shaders[i].id);
		}


		glLinkProgram(id);

		// Check the program
		GLint result;
		GLint length; 
		glGetProgramiv(id, GL_LINK_STATUS, &result);
		if (result != GL_TRUE) {
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			char *buffer = (char*)StackMemAllocator::Instance()->allocate(length);
			glGetProgramInfoLog(id, length, NULL, buffer);
			Logger::debug("Error Linking program: \n%s", buffer);
			StackMemAllocator::Instance()->free((u8*)buffer);
		}

		for(i32 i = 0; i < shaders.size(); i++) {
			glDetachShader(id, shaders[i].id);
			glDeleteShader(shaders[i].id);
			shaders[i].id = 0;
		}
		return id;
	}

	GLProgram::GLProgram(std::vector<ShaderFile> shaders)
	{
		for(i32 i = 0; i < shaders.size(); i++) {
			shaders[i].id = compileShader(shaders[i].filename, shaders[i].shader_type);
		}

		_id = makeProgram(shaders);
	}

	void GLProgram::use()
	{
		glUseProgram(_id);
	}


	void GLProgram::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
	}

	void GLProgram::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
	}

	void GLProgram::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
	}

	void GLProgram::set4Float(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
	}

	int GLProgram::getUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(_id, name.c_str());
	}

	int GLProgram::getAtrributeLocation(const std::string& name)
	{
		return glGetAttribLocation(_id, name.c_str());
	}

}
